#include "msgpack_c_op.h"
#include "packing_texture.h"

namespace vg{
  void res_unit_2_msgpack_stream(mpk_stream& mpk,packing_texture::packing_texture_unit& res_unit);
  bool msgpack_stream_2_res_unit(packing_texture::packing_texture_unit& res_unit,mpk_obj& mpkObj);
}