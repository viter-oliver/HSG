#include "msgpack_c_op.h"
#include "file_chunk.h"
namespace vg{
  void res_unit_2_msgpack_stream(mpk_stream& mpk,std::string& key,file_chunk_unit& res_unit);
  sd_file_chunk_unit msgpack_stream_2_res_unit(mpk_obj& mpkObj);
}