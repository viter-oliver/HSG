
#include "macro_tool.h"
#include "font.h"
#include <vector>
namespace vg {
namespace font_manager {
enum omit_type{
en_no_omit,
en_omit_rest,
en_omit_rest_with_ellipsis,
en_omit_type_number
};


bool load_font(font_unit& ft, uint8_t *pfont_buff,
                       FT_Long file_size);

bool load_font(font_unit& ft,const std::string& fontPath);
int draw_wstring(font_unit &f_u, FT_Long fontSize, vec2 &start_pos,
                 vec2 &end_pos, float scale, std::wstring &str_content,
                 const vec4 &txt_col, float width, int omit_rest,float& first_line_offset,
                 bool be_new,u8 face_id=0);
int draw_wstring(font_unit f_u, FT_Long fontSize, vec2 &start_pos,
                 vec2 &end_pos, float scale, std::wstring &str_content,
                 const vec4 &txt_col, vec2 &l_top_edge, vec2 &r_bottom_edge,
                 float &line_spacing, bool be_new, u8 face_id = 0);
} // namespace font_manager
} // namespace vg