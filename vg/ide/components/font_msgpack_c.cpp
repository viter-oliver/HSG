#include "font_manager.h"
#include "font_msgpack_c.h"
#include "dir_output.h"
#include "common_functions.h"

namespace vg{
  using namespace std;
  extern string g_cureent_directory;
  void res_unit_2_msgpack_stream(mpk_stream& mpk,font_unit& res_unit){
    mpk.pack_array(3);
    mpk.pack_fix_uint32(res_unit._char_count_c);
    mpk.pack_fix_uint32(res_unit._char_count_r);
    string font_path=g_cureent_directory+font_fold+res_unit._name;
    get_file_data(font_path,[&](char* file_buff,u32 dataLen){
      mpk.pack_bin(dataLen);
      mpk.pack_bin_body(file_buff,dataLen);
    });
  }
  bool msgpack_stream_2_res_unit(font_unit& res_unit,mpk_obj& mpkObj){
    res_unit._char_count_c=mpkObj.via.array.ptr[0].as<u32>();
    res_unit._char_count_r=mpkObj.via.array.ptr[1].as<u32>();
    auto font_data=mpkObj.via.array.ptr[3];
    auto font_data_size=font_data.via.bin.size;
    font_manager::load_font(res_unit,(u8*)font_data.via.bin.ptr,font_data_size);
  }
}