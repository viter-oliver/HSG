#pragma once
#include <string>
#include <map>
#include <ft2build.h>
#include <memory>
#include "vg_type.h"
#include FT_FREETYPE_H
namespace vg{
  namespace font_manager{
	  struct font_txt_coordinate{
	    vui2 _size;
	    vi2 _bearing;
	    u32 _advance;
	    u32 _txt_id { 0 };
	    ~font_txt_coordinate();
	  };
	  using sd_font_txt_coordinate=std::shared_ptr<font_txt_coordinate>;
	  using dic_glyph_txt=std::map<wchar_t,sd_font_txt_coordinate>;
	  using sd_dic_glyph_txt = std::shared_ptr<dic_glyph_txt>;
	  using ifont_size = FT_Long;
	  using dic_font_rep=std::map<ifont_size, sd_dic_glyph_txt>;
	}
  const u8 max_face_count = 10;
	struct font_unit{
	std::string _buffer;
    u8 face_count = 1;
    FT_Face _ft_face[max_face_count]{nullptr, nullptr, nullptr, nullptr, nullptr,
                                   nullptr, nullptr, nullptr, nullptr, nullptr};
	font_manager::dic_font_rep _ft_rep;
	~font_unit()
	{
    for (int ix = 0; ix < max_face_count; ix++) {
      if (_ft_face[ix]){
        FT_Done_Face(_ft_face[ix]);
			}
    }
	}
	};
	DEF_TYPE_DIC( font_unit )
  
}