#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "material_3d.h"
#include "transform.h"
#include "material_container.h"
#include "vertex_container.h"
namespace vg {
    using namespace std;
material_3d::material_3d() {
  strcpy(_pt._uf_model_name, "model");
  strcpy(_pt._uf_view_name, "view");
  strcpy(_pt._uf_proj_name, "projection");
  _pt._cam = {{-400, 400, 0}, {-400, 400, 0}, {0, 1, 0}};
  _pt._pj = {20, 10, 10000};
  _pt._vertex_name[0] = '\0';
  _pt._material_name[0] = '\0';

}

material_3d::~material_3d() {
  if (_gpu_outbuff != 0) {
    glDeleteBuffers(1, &_gpu_outbuff);
    _gpu_outbuff = 0;
  }
}

void material_3d::link() {
  int imatch = 0;
  auto imt = shader::_mp_sd_materials.find(_pt._material_name);
  if (imt != shader::_mp_sd_materials.end()) {
    _ps_mtl = imt->second;
    imatch++;
  }

  auto iprm = vertex::_mp_vertex_units.find(_pt._vertex_name);
  if (iprm != vertex::_mp_vertex_units.end()) {
    _ps_prm = iprm->second;
    if (_pt._with_feedback) {
      glBindBuffer(GL_ARRAY_BUFFER, _ps_prm->_vbo);
      int buff_len=0;
      glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buff_len);

      glGenBuffers(1, &_gpu_outbuff);
      glBindBuffer(GL_ARRAY_BUFFER, _gpu_outbuff);
      glBufferData(GL_ARRAY_BUFFER, buff_len, nullptr, GL_STATIC_READ);
    }
    imatch++;
  }
  _matched = imatch == 2;
}
bool material_3d::get_output_vertex(vector<float> &overtex) {
  if (!_pt._with_feedback) {
    return false;
  }
  glBindBuffer(GL_ARRAY_BUFFER, _ps_prm->_vbo);
  int buff_len = 0;
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buff_len);
  overtex.resize(buff_len/sizeof(float));
  glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0,
                    buff_len, &overtex[0]);
  return true;
}
void material_3d::draw() {

  if (_matched) {
    if (!_ps_mtl->valid() || !_ps_prm) {
      return;
    }

    static GLuint draw_model[] = {
        GL_POINTS,    GL_LINES,          GL_LINE_LOOP,    GL_LINE_STRIP,
        GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN,
    };
    glm::mat4 model;
    // auto& aftr = _pt._trans._translation;
    // auto& afsc = _pt._trans._scale;
    // auto& afrt = _pt._trans._rotation;
    for (auto &item : _vchilds) {
      transform *ptrans_item = (transform *)item.get();
      ptrans_item->apply(model);
    }
    glm::mat4 view;
    auto &cam_pos = _pt._cam._position;
    auto &cam_dir = _pt._cam._direction;
    auto &cam_up = _pt._cam._up;
    glm::vec3 gcam_pos(cam_pos.x, cam_pos.y, cam_pos.z);
    glm::vec3 gcam_dir(cam_dir.x, cam_dir.y, cam_dir.z);
    glm::vec3 gcam_up(cam_up.x, cam_up.y, cam_up.z);
    view = glm::lookAt(gcam_pos, gcam_dir, gcam_up);
    glm::mat4 proj;
    auto &cproj = _pt._pj;
    auto pparent = get_parent();
    float w, h;
    pparent->get_size(w, h);
    float aspect = w / h;
    proj = glm::perspective(glm::radians(cproj._fovy), aspect, cproj._near,
                            cproj._far);
    _ps_mtl->set_value(_pt._uf_model_name, glm::value_ptr(model), 16);
    _ps_mtl->set_value(_pt._uf_view_name, glm::value_ptr(view), 16);
    _ps_mtl->set_value(_pt._uf_proj_name, glm::value_ptr(proj), 16);
    _ps_mtl->set_value("alpha", &_pt._alpha_nml, 1);
    _ps_mtl->use();
    auto &primid = *_ps_prm;

    glBindVertexArray(primid._vao);
    GLuint &dml = draw_model[_pt._draw_mode];
    if (_pt._with_feedback) {
      if (_gpu_outbuff == 0) {
        glGenBuffers(1, &_gpu_outbuff);
        glBindBuffer(GL_ARRAY_BUFFER, _gpu_outbuff);
        auto buff_sz = _ps_prm->_vertex_buf_len * sizeof(GLfloat);
        glBufferData(GL_ARRAY_BUFFER, buff_sz, nullptr, GL_STATIC_READ);
      }
      glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _gpu_outbuff);
      glBeginTransformFeedback(dml);
    }
    if (primid._ele_buf_len == 0) {
      glDrawArrays(dml, 0, primid._vertex_buf_len);
    } else {
      glDrawElements(dml, primid._ele_buf_len, GL_UNSIGNED_INT, 0);
    }
    if (_pt._with_feedback) {
      glEndTransformFeedback();
      glFlush();
    }
  }
  // ctl_base::draw();
}

} // namespace vg
