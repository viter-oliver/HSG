#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include "glad/glad.h"
#endif
#include "vertex.h"
namespace vg {
GLuint draw_model[en_gl_count] = {
    GL_POINTS,    GL_LINES,          GL_LINE_LOOP,    GL_LINE_STRIP,
    GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN,
};
const char *str_draw_model[en_gl_count] = {
    "GL_POINTS",    "GL_LINES",          "GL_LINE_LOOP",   "GL_LINE_STRIP",
    "GL_TRIANGLES", "GL_TRIANGLE_STRIP", "GL_TRIANGLE_FAN"
};
  /**
void calcu_boundingbox(float *vertex_data, u32 vertex_cnt) {
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
}*/

#if 0
vertex::vertex(vertex_file_head* pvertex_head,u32 buffer_len) {
  _format.clear();
  _format.resize(pvertex_head->demension);
  get_vtx_format(pvertex_head->format_index,_format);
  _progressive=pvertex_head->type;
  _mem_usage=mem_usage[pvertex_head->usage];
  _ele_type=ele_type[pvertex_head->element_type];
  _vertex_count=pvertex_head->vertex_cnt;
  if (!_vao) {
    glGenVertexArrays(1, &_vao);
  }  
  pvertex_head++;
  float * pvertex_data=(float*)pvertex_head;  
  glBindVertexArray(_vao);
  GLubyte stride_ = stride();
  auto vertex_len=_vertex_count*stride_;
  
  if(!_progressive){//interlaced
    if (!_vbo) {
      glGenBuffers(1, &_vbo);
    }
    auto temp_buffer_len = vertex_len * sizeof( GLfloat );
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, temp_buffer_len, pvertex_data, _mem_usage);
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
  } else{
    for(int ix=0;ix<7;ix++){
      if (_vbos[ix]) {
        glDeleteBuffers(1, &_vbos[ix]);
      }
    }
    glGenBuffers(_format.size(),_vbos);
    GLuint idx = 0;
    for (auto &el : _format) {
      glBindBuffer(GL_ARRAY_BUFFER,_vbos[idx]);
      auto base_ele_count = el * _vertex_count;
      auto temp_buffer_len =base_ele_count*sizeof(float);
      glBufferData(GL_ARRAY_BUFFER, temp_buffer_len,pvertex_data,_mem_usage);
      glVertexAttribPointer(idx, el, GL_FLOAT, GL_FALSE,
                        0,
                        0); //
      glEnableVertexAttribArray(idx);                  
      idx++;
      pvertex_data+=base_ele_count;
    }
  }


  if (_ele_type) {
    pvertex_data+=vertex_len;
    
    if (!_ebo) {
      glGenBuffers(1, &_ebo);
    }

    auto ele_sz=ele_size[pvertex_head->element_type];
    auto ele_len=buffer_len-_vertex_count * sizeof(float);
    _ele_count=ele_len/ele_len;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ele_len, pvertex_data,
                 _mem_usage);
  }
  glBindVertexArray(0);
}
#endif
vertex::vertex(vtx_format& fmt,float *pvertex_data, u32 vertex_count, 
  u32 mem_usage, bool interlaced,u32 *pele_buff, u32 ele_cnt){
  _format=fmt;
  _vertex_count = vertex_count;
  _interlaced = interlaced;
  _ele_count = ele_cnt;
  _mem_usage = mem_usage;
  u8 std_len=stride();
  auto vertex_buf_len=std_len * _vertex_count* sizeof(float);
  glGenVertexArrays( 1, &_vao );
  glBindVertexArray(_vao);
  if( interlaced ){
    glGenBuffers(1, _vbos);
    auto temp_buffer_len = std_len*vertex_count * sizeof(GLfloat);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, temp_buffer_len, pvertex_data, _mem_usage);
    GLuint idx = 0;
    int pointer = 0;
    for (auto &el : _format) {
      glEnableVertexAttribArray(idx);
      glVertexAttribPointer(idx, el, GL_FLOAT, GL_FALSE,
                            std_len * sizeof(GLfloat),
                            (void *)(pointer * sizeof(GLfloat))); //
      pointer += el;
      idx++;
    }
  }else{
    glGenBuffers(_format.size(), _vbos);
    GLuint idx = 0;
    for (auto &vtx_el : _format) {
      glBindBuffer(GL_ARRAY_BUFFER, _vbos[idx]);
      auto base_ele_count = vtx_el * _vertex_count;
      auto temp_buffer_len = base_ele_count * sizeof(float);
      glBufferData(GL_ARRAY_BUFFER, temp_buffer_len, pvertex_data, _mem_usage);
      glVertexAttribPointer(idx, vtx_el, GL_FLOAT, GL_FALSE, 0,
                            0); //
      glEnableVertexAttribArray(idx);
      idx++;
      pvertex_data += base_ele_count;
    }
  }
  
  if (pele_buff) {
    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _ele_count,
                pele_buff, mem_usage);
  }
  glBindVertexArray(0);

}
vertex::vertex(std::vector<vertex_ele>& vtx_eles, u32 vertex_count,
    u32 mem_usage, u32* pele_buff, u32 ele_cnt) {
    _interlaced = false;
    _vertex_count = vertex_count;
    _ele_count = ele_cnt;
    _mem_usage = mem_usage;
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glGenBuffers(vtx_eles.size(), _vbos);
    for (int ix = 0; ix < vtx_eles.size();ix++) {
        auto& vtx_u = vtx_eles[ix];
        _format.push_back(vtx_u.ele_size);
		glBindBuffer(GL_ARRAY_BUFFER, _vbos[ix]);
        auto base_ele_count = vtx_u.ele_size * _vertex_count;
        auto temp_buffer_len = base_ele_count * sizeof(float);
        glBufferData(GL_ARRAY_BUFFER, temp_buffer_len, vtx_u.pdata, _mem_usage);
        glVertexAttribPointer(ix, vtx_u.ele_size, GL_FLOAT, GL_FALSE, 0,
            0); //
        glEnableVertexAttribArray(ix);
	}
    if (pele_buff) {
        glGenBuffers(1, &_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _ele_count,
            pele_buff, mem_usage);
    }
    glBindVertexArray(0);
}
void vertex::enableVertex() {
  glBindVertexArray(_vao);
  #if 0
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
  #endif
}
void vertex::draw(u32 model) {
  glBindVertexArray(_vao);
  if (0 == _ele_count) {
    glDrawArrays(model, 0, _vertex_count);
  } else {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glDrawElements(model, _ele_count, GL_UNSIGNED_INT, 0);
  }
}
vertex::~vertex() {
  if (_vao) {
    glDeleteVertexArrays(1, &_vao);
  }
  if (_ebo) {
    glDeleteBuffers(1, &_ebo);
  }

  for(int ix=0;ix<7;ix++){
      if (_vbos[ix]) {
        glDeleteBuffers(1, &_vbos[ix]);
      }
    }
}
} // namespace vg