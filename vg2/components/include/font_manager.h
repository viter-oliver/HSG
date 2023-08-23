
#include "macro_tool.h"
#ifdef WITHIN_IDE
#include "font_ide.h"
using font_base=vg::font_ide;
#else
#include "font.h"
using font_base=vg::font_unit;
#endif
#include <vector>
namespace vg {
DEFINE_SMART(font_base)
enum omit_type{
en_no_omit,
en_omit_rest,
en_omit_rest_with_ellipsis,
en_omit_type_number
};
namespace font_manager {

v_sd_font_base& get_dic_fonts();
bool load_font(font_base& ft, uint8_t *pfont_buff,
                       FT_Long file_size);

bool load_font(font_base& ft,std::string& fontPath);
int draw_wstring(font_base &f_u, FT_Long fontSize, vec2 &start_pos,
                 vec2 &end_pos, float scale, std::wstring &str_content,
                 const vec4 &txt_col, float width, int omit_rest,
                 bool be_new);
int draw_wstring(font_base f_u, FT_Long fontSize, vec2 &start_pos,
                 vec2 &end_pos, float scale, std::wstring &str_content,
                 const vec4 &txt_col, vec2 &l_top_edge,
                 vec2 &r_bottom_edge, float &line_spacing, bool be_new);
} // namespace font_manager
} // namespace vg