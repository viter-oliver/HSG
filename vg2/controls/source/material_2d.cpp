#include "material_2d.h"
#include "material_container.h"
namespace vg {
material_2d::material_2d() {
  _pt._vertex_name[0] = '\0';
  _pt._material_name[0] = '\0';
}
void material_2d::link() {
  int imatch = 0;
  auto imt = shader::_mp_sd_materials.find(_pt._material_name);
  if (imt != shader::_mp_sd_materials.end()) {
    _ps_mtl = imt->second;
    imatch++;
  }

  auto iprm = _mp_vertex_base_units.find(_pt._vertex_name);
  if (iprm != _mp_vertex_base_units.end()) {
    _ps_prm = iprm->second;
    imatch++;
  }
  _matched = imatch == 2;
}
void material_2d::draw() {
  if (_matched) {
    if (!_ps_mtl->is_valid()) {
      return;
    }
    static GLuint draw_model[en_gl_count] = {
        GL_POINTS,    GL_LINES,          GL_LINE_LOOP,    GL_LINE_STRIP,
        GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN,
    };
    _ps_mtl->use();
    auto &primid = *_ps_prm;
    glBindVertexArray(primid._vao);
    GLuint &dml = draw_model[_pt._draw_mode];

    if (primid._ele_buf_len == 0) {
      glDrawArrays(dml, 0, primid._vertex_buf_len);
    } else {
      glDrawElements(dml, primid._ele_buf_len, GL_UNSIGNED_INT, 0);
    }
  }
}
