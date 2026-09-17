#pragma once
#include <vector>
#include "vg_type.h"
namespace vg {
using vtx_format=std::vector<u8>;
struct bounding_box{
  float _xmin{ 0.f }, _xmax{ 0.f };
	float _ymin{ 0.f }, _ymax{ 0.f };
	float _zmin{ 0.f }, _zmax{ 0.f };
};
enum {
  en_gl_points,
  en_gl_lines,
  eg_gl_line_loop,
  en_gl_line_strip,
  en_gl_triangles,
  en_gl_triangle_strip,
  en_gl_triangle_fan,
  en_gl_count
};
extern u32 draw_model[en_gl_count];
extern const char *str_draw_model[en_gl_count];
struct vertex_ele {
    u8 ele_size = 0;
    float* pdata = nullptr;
};
struct vertex {
  u32 _vao{0}, _ebo{0};
  u32 _vbos[7]={0};
  // data below could be got by glGetBufferParameteriv(GL_ARRAY_BUFFER,
  // GL_BUFFER_SIZE, &bufferSize); 
  u32 _vertex_count{0}, _ele_count{0}, _mem_usage{0};
  bool _interlaced;
  //u32 _ele_type{0};
  vtx_format _format;
  bounding_box _bounding_box; 
  //bool _progressive{false};
  //vertex(vertex_file_head* pvertex_head,u32 buffer_len);
  vertex(vtx_format& fmt,float *pvertex_data, u32 vertex_count,
                        u32 mem_usage,bool interlaced=true,
                        u32 *pele_buff = 0, u32 ele_cnt = 0);
  vertex(std::vector<vertex_ele>& vtx_eles, u32 vertex_count,
         u32 mem_usage, u32* pele_buff = 0, u32 ele_cnt = 0);
  ~vertex();
  u8 stride() {
    u8 sum = 0;
    for (auto &ele : _format) {
      sum += ele;
    }
    return sum;
  }
  void enableVertex();
  void draw(u32 model);
};
DEF_TYPE_DIC(vertex)
} // namespace vg