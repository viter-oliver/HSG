#include "vg_type.h"
#include <string>
#include <memory>
#include <map>
#include <vector>
namespace vg {
namespace primitive {
using attribute_format = std::vector<u8>;
struct primitive_unit {
  u32 _vao{0}, _vbo{0}, _ebo{0};
  // data below could be got by glGetBufferParameteriv(GL_ARRAY_BUFFER,
  // GL_BUFFER_SIZE, &bufferSize); u32 _vertex_buf_len, _ele_buf_len;
  attribute_format _format;
  ~primitive_unit();
  u8 stride() {
    u8 sum = 0;
    for (auto &ele : _format) {
      sum += ele;
    }
    return sum;
  }
  void load_vertex_data(u32 mem_usage, float *pvertex_data, u32 vertex_bytes_nm,
                        attribute_format fmt, u32 *pele_buff = 0,
                        u32 ele_cnt = 0);
  void enableVertex();
};
using sd_primitive_unit=std::shared_ptr<primitive_unit>;

} // namespace primitive
} // namespace vg