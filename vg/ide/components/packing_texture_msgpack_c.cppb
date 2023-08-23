#include "packing_texture_msgpack_c.h"
#include "bin_format.h"

#include "SOIL.h"
namespace vg {
void res_unit_2_msgpack_stream(
    mpk_stream &mpk, packing_texture::packing_texture_unit &res_unit) {
  mpk.pack_array(4);
  mpk.pack_uint32(res_unit._width);
  mpk.pack_uint32(res_unit._height);
  bin_format::texture_buff txtBuf;
  bin_format::get_texture_buff(res_unit.txt_id, res_unit._width,
                               res_unit._height, txtBuf);
  if (res_unit.separated) {
    // TODO:pack the texture data to a separated file
    mpk.pack_uint8(0);
  } else {
    mpk.pack_bin(txtBuf.size());
    mpk.pack_bin_body((const char*)&txtBuf[0], txtBuf.size());
  }
  auto &sb_txt_coor = res_unit.vtexture_coordinate;
  mpk.pack_array(sb_txt_coor.size());
  for (const auto &cd_unit : sb_txt_coor) {
    mpk.pack_array(4);
    mpk.pack_float(cd_unit->_x0);
    mpk.pack_float(cd_unit->_x1);
    mpk.pack_float(cd_unit->_y0);
    mpk.pack_float(cd_unit->_y1);
  }
}
bool msgpack_stream_2_res_unit(packing_texture::packing_texture_unit &res_unit,
                               mpk_obj &mpkObj) {
  res_unit._width = mpkObj.via.array.ptr[0].as<u32>();
  res_unit._height = mpkObj.via.array.ptr[1].as<u32>();
  auto pk_txt_bin = mpkObj.via.array.ptr[3];
  if (pk_txt_bin.type == msgpack::type::POSITIVE_INTEGER) {
    // TODO:initialize the texture data from a separated file by a separated thread

  } else {
    auto bin_sz = pk_txt_bin.via.bin.size;
    auto ptxtBuf = pk_txt_bin.via.bin.ptr;
    res_unit.txt_id = bin_format::get_texture_id(
        (u8*)ptxtBuf, bin_sz, res_unit._width, res_unit._height);
  }
  auto vtxt_coor_bin=mpkObj.via.array.ptr[4];
  auto& vtxt_coor=res_unit.vtexture_coordinate;
  auto vtxt_coor_sz=vtxt_coor_bin.via.array.size;
  vtxt_coor.resize(vtxt_coor_sz);
  for (size_t ix = 0; ix < vtxt_coor_sz; ++ix) {
    vtxt_coor[ix]=make_shared<sub_texture_coordinate>();
    auto& cd_unit=*vtxt_coor[ix];
    auto bin_cd_unit=vtxt_coor_bin.via.array.ptr[ix];
    cd_unit._x0=bin_cd_unit.via.array.ptr[0].as<float>();
    cd_unit._x1=bin_cd_unit.via.array.ptr[1].as<float>();
    cd_unit._y0=bin_cd_unit.via.array.ptr[2].as<float>();
    cd_unit._y1=bin_cd_unit.via.array.ptr[3].as<float>();
  }
  return true;
}
} // namespace vg