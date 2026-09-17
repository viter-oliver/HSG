#pragma once
#include <windows.h>
#include <locale.h>  
#include <ShlObj.h>
#include <Commdlg.h>
#include "font_manager.h"
#include "font.h"
#include "user_control_imgui.h"
#include "common_functions.h"
#include "ifile_open.hpp"
namespace vg{
  extern std::string g_cureent_directory;
  bool load_font(font_unit& ft_u, const std::string& ttf_file) {
      get_file_data(ttf_file, [&](char* pbuf, u32 len) {
          ft_u._buffer.resize(len);
          char* pdata = &ft_u._buffer[0];
          memcpy_s(pdata, len, pbuf, len);
          });
      //fkey = ttf_file_name;
      if (!font_manager::load_font(ft_u, ttf_file))
      {
          std::string err_msg = "fail to load file:";
          err_msg += ttf_file;
          err_msg += "!";
          MessageBox(GetForegroundWindow(), err_msg.c_str(), "Error info", MB_OK);
          return false;
      }
      return true;
  }
  template<>
  bool create_new_pair<font_unit>(font_unit_key& fkey,sd_font_unit& sd_fu){
    if(ImGui::Button("New Font...")){
      OPENFILENAME ofn = { sizeof(OPENFILENAME) };
      ofn.hwndOwner = GetForegroundWindow();
      //ofn.lpstrFilter = "ttf file:\0*.ttf\0\0";
      char strFileName[MAX_PATH] = { 0 };
      ofn.nFilterIndex = 1;
      ofn.lpstrFile = strFileName;
      ofn.nMaxFile = sizeof(strFileName);
      ofn.lpstrTitle = "Loading font file...";
      ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
      if (GetOpenFileName(&ofn))
      {
        std::string ttf_file = strFileName;
        std::string ttf_file_name = ttf_file.substr(ttf_file.find_last_of('\\') + 1);
        sd_fu = std::make_shared<font_unit>();
        font_unit& ft_u=*sd_fu;
        if (!load_font(ft_u, ttf_file)) {
		   return false;
        }
        fkey.get_str_value( ttf_file_name );
        return true;
      }
    }

    return false;
  }
  template<>
  void res_edit<font_unit>(font_unit& fu){
    ImGui::Text( "Number of faces:%d", fu.face_count );
    struct st_face_index
    {
         u32 face_index : 16;
         u32 instance_index : 15;
         u32 reserved : 1;
    };
    for(int ix=0;ix<fu.face_count;ix++){
      
      st_face_index* pface_index = (st_face_index*) & fu._ft_face[ix]->face_index;
      ImGui::Text( "Face index:%d", pface_index->face_index );
      ImGui::Text( "Instance index:%d", pface_index->instance_index );
      ImGui::Text("Number of glyphs:%d", fu._ft_face[ix]->num_glyphs);
      ImGui::Text( "Family name:%s", fu._ft_face[ix]->family_name );
      ImGui::Text( "Style name:%s", fu._ft_face[ix]->style_name );
    }
    
  }
  REG_MAP_COPONENT_EDIT_CONTAINER(font_unit)
  struct open_font :public IFileOpener {
      bool tryOpen(const std::string& path) {
          size_t pos = path.find_last_of('.');
          if (pos != std::string::npos) {
              std::string ext = path.substr(pos + 1);
              const char* freetype_supported_extensions[] = {
                        "ttf","TTF", "ttc", "otf","otc","pfb", "pfa","cff","cid","pcf","bdf", "pfr", "fon","fnt", nullptr
              };
			  //check supported extensions
              for (int ix = 0; freetype_supported_extensions[ix] != nullptr; ix++) {
                  if (ext == freetype_supported_extensions[ix]) {
                      std::string ttf_file_name = path.substr(path.find_last_of('\\') + 1);
					  sd_font_unit sd_fu = std::make_shared<font_unit>();
					  font_unit& ft_u = *sd_fu;
                      if (!load_font(ft_u, path)) {
                          return false;
					  }
					  font_unit_key fkey;
					  fkey.get_str_value(ttf_file_name);
                      font_unit_assist_._sd_o->add_new_ele(fkey, sd_fu);
                      return  true;
                  }
              }
          }
          return false;
      }
  };
}