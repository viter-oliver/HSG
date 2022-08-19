#pragma once
#include <string>
#include "packing_texture_ide.h"
namespace vg{
void open_file_to_folder(std::string& file_name,const char* folder,const char* dlg_title,const char* str_filter="file:\0*.*\0\0");
void load_vector_texture_coordinate_ide_from_json(packing_texture::vtxt_coor_unit& vcoor,char* json_buff,int buff_len);
}
