#pragma once
#include <string>
#include "vg_type.h"
#include "texture_ide.h"
namespace vg{
void open_file_to_folder(std::string& file_name,const char* folder,const char* dlg_title,const char* str_filter="file:\0*.*\0\0");
void load_vtex_from_json(v_sd_sub_txt& vcoor,char* json_buff,int buff_len);
}
