#include "file_chunk_msgpack_c.h"
namespace vg {
void res_unit_2_msgpack_stream(mpk_stream &mpk, std::string &key,
                               file_chunk_unit &res_unit) {
  mpk.pack_array(2);
  mpk.pack_str(key.size());
  mpk.pack_str_body(key.c_str(), key.size());
  mpk.pack_bin(res_unit._fsize);
  mpk.pack_bin_body((const char*)res_unit._pbin, res_unit._fsize);
}
sd_file_chunk_unit msgpack_stream_2_res_unit(mpk_obj &mpkObj) {
  auto file_sz = mpkObj.via.bin.size;
  auto s_file_obj = std::make_shared<file_chunk_unit>(file_sz);
  memcpy_s(s_file_obj->_pbin, file_sz, mpkObj.via.bin.ptr, file_sz);
  return s_file_obj;
}
} // namespace vg