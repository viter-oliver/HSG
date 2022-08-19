#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include "glad/glad.h"
#endif
#include "primitive.h"
namespace vg {
namespace primitive {
mp_primitive_unit _mp_primitive_units;
primitive_unit::~primitive_unit() {
  if (_vao) {
    glDeleteVertexArrays(1, &_vao);
  }
  if (_ebo) {
    glDeleteBuffers(1, &_ebo);
  }
  if (_vbo) {
    glDeleteBuffers(1, &_vbo);
  }
}
void primitive_unit::load_vertex_data(u32 mem_usage, float *pvertex_data,
                                      u32 vertex_bytes_nm, attribute_format fmt,
                                      u32 *pele_buff, u32 ele_cnt) {
  if (!_vbo) {
    glGenBuffers(1, &_vbo);
  }
  if (!_vao) {
    glGenVertexArrays(1, &_vao);
  }
  _format = fmt;
  glBindVertexArray(_vao);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, vertex_bytes_nm, pvertex_data, mem_usage);

  if (pele_buff) {
    if (!_ebo) {
      glGenBuffers(1, &_ebo);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * ele_cnt, pele_buff,
                 mem_usage);
  }
  GLuint idx = 0;
  GLubyte stride_ = stride();
  int pointer = 0;

  for (auto &el : _format) {
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, el, GL_FLOAT, GL_FALSE,
                          stride_ * sizeof(GLfloat),
                          (void *)(pointer * sizeof(GLfloat))); //
    pointer += el;
    idx++;
  }

  glBindVertexArray(0);
}
// FIXME:most of enableVertex appeared in load_vertex_data
void primitive_unit::enableVertex() {
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  if (_ebo > 0) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
  }

  GLuint idx = 0;
  GLubyte stride_ = stride();
  int pointer = 0;

  for (auto &el : _format) {
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, el, GL_FLOAT, GL_FALSE,
                          stride_ * sizeof(GLfloat),
                          (void *)(pointer * sizeof(GLfloat))); //
    pointer += el;
    idx++;
  }
}
} // namespace primitive
} // namespace vg