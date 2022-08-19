#pragma once
#include <string>
#include <map>
#include <ft2build.h>
#include <memory>
#include "vg_type.h"
#include FT_FREETYPE_H
namespace vg{
  struct font_txt_coordinate{
    vui2 _size;
    vi2 _brearing;
    u32 _advance;
    float _x0,_y0,_x1,_y1;
  };
  using sd_font_txt_coordinate=std::shared_ptr<font_txt_coordinate>;
  using dic_glyph_txt=std::map<wchar_t,sd_font_txt_coordinate>;
  struct font_txt_repository{
    u32 _txt_id{0};
    vui2 _txt_size;
    vui2 _border;
    s32 _max_bearing;
    u32 _font_size{0};
    bool _full{false};
    dic_glyph_txt _dic_txt_cd;
    ~font_txt_repository();
  };
  using dic_font_rep=std::map<int,font_txt_repository>;
  struct font_unit{
    
    std::string _name;
    FT_Face _ft_face{nullptr};
    u32 _char_count_r={64},_char_count_c={64};
    dic_font_rep _ft_rep;
    ~font_unit()
		{
			if (_ft_face)
			{
				FT_Done_Face(_ft_face);
			}
		}
  };
}