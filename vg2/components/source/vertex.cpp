#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include "glad/glad.h"
#endif
#include "vertex.h"
namespace vg {
void vertex::calcu_boundingbox(float *vertex_data, u32 vertex_cnt) {
  auto stride_ = stride();
  for (u32 ix = 0; ix < vertex_cnt; ++ix) {
    if (vertex_data[0] < _bounding_box._xmin) {
      _bounding_box._xmin = vertex_data[0];
    }
    if (vertex_data[0] > _bounding_box._xmax) {
      _bounding_box._xmax = vertex_data[0];
    }
    if (_format[0] > 1) {
      if (vertex_data[1] < _bounding_box._ymin) {
        _bounding_box._ymin = vertex_data[1];
      }
      if (vertex_data[1] > _bounding_box._ymax) {
        _bounding_box._ymax = vertex_data[1];
      }
    }
    if (_format[0] > 2) {
      if (vertex_data[2] < _bounding_box._zmin) {
        _bounding_box._zmin = vertex_data[2];
      }
      if (vertex_data[2] > _bounding_box._zmax) {
        _bounding_box._zmax = vertex_data[2];
      }
    }
    vertex_data += stride_;
  }
}
void vertex::load_vertex_data(u32 mem_usage, float *pvertex_data,
                              u32 vertex_bytes_nm, attribute_format fmt,
                              u32 *pele_buff, u32 ele_cnt) {
  if (!_vbo) {
    glGenBuffers(1, &_vbo);
  }
  if (!_vao) {
    glGenVertexArrays(1, &_vao);
  }
  _format = fmt;
  _vertex_buf_len = vertex_bytes_nm;
  _mem_usage = mem_usage;
  _ele_buf_len = ele_cnt;
  GLubyte stride_ = stride();
  auto vertex_cnt = vertex_bytes_nm / sizeof(float) * stride_;
  calcu_boundingbox(pvertex_data, vertex_cnt);

  glBindVertexArray(_vao);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);

  glBufferData(GL_ARRAY_BUFFER, vertex_bytes_nm * sizeof(GLfloat), pvertex_data,
               mem_usage);

  if (pele_buff) {
    if (!_ebo) {
      glGenBuffers(1, &_ebo);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * ele_cnt, pele_buff,
                 mem_usage);
  }
  GLuint idx = 0;

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
void vertex::enableVertex() {
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
vertex::~vertex() {
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
m_sd_vertex g_vertexies；
} // namespace vg