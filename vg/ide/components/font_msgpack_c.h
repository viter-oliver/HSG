#include "msgpack_c_op.h"
#include "font.h"
namespace vg{
  void res_unit_2_msgpack_stream(mpk_stream& mpk,font_unit& res_unit);
  bool msgpack_stream_2_res_unit(font_unit& res_unit,mpk_obj& mpkObj);  
}