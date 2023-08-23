#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vg_type.h"
namespace vg {
using attribute_format = std::vector<u8>;
struct bounding_box{
  float _xmin{ 0.f }, _xmax{ 0.f };
	float _ymin{ 0.f }, _ymax{ 0.f };
	float _zmin{ 0.f }, _zmax{ 0.f };
};

struct vertex{
  u32 _vao{0}, _vbo{0}, _ebo{0};
  // data below could be got by glGetBufferParameteriv(GL_ARRAY_BUFFER,
  // GL_BUFFER_SIZE, &bufferSize); 
  u32 _vertex_buf_len, _ele_buf_len, _mem_usage;
  attribute_format _format;
  bounding_box _bounding_box; 
  ~vertex();
  u8 stride() {
    u8 sum = 0;
    for (auto &ele : _format) {
      sum += ele;
    }
    return sum;
  }
  void calcu_boundingbox(float* vertex_data,u32 vertex_cnt);
  void load_vertex_data(u32 mem_usage, float *pvertex_data, u32 vertex_bytes_nm,
                        attribute_format fmt, u32 *pele_buff = 0,
                        u32 ele_cnt = 0);
  void enableVertex();
};

} // namespace vg