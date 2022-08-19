#include "msgpack_c_op.h"
#include "vg_texture.h"
namespace vg{
  void res_unit_2_msgpack_stream(mpk_stream& mpk,std::string& key,texture::texture_unit& res_unit);
  texture::sd_texture_unit msgpack_stream_2_res_unit(mpk_obj& mpkObj);
}