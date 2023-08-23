#include "font.h"
#include <vector>
namespace vg {
using ps_font_unit = std::shared_ptr<font_unit>;
using dic_fonts = std::vector<ps_font_unit>;
enum omit_type{
en_no_omit,
en_omit_rest,
en_omit_rest_with_ellipsis,
en_omit_type_number
};
namespace font_manager {

dic_fonts& get_dic_fonts();
bool load_font(font_unit& ft, uint8_t *pfont_buff,
                       FT_Long file_size);

bool load_font(font_unit& ft,std::string& fontPath);
int draw_wstring(ps_font_unit &pf_u, FT_Long fontSize, vec2 &start_pos,
                 vec2 &end_pos, float scale, std::wstring &str_content,
                 const vec4 &txt_col, float width, int omit_rest,
                 bool be_new);
int draw_wstring(ps_font_unit &pf_u, FT_Long fontSize, vec2 &start_pos,
                 vec2 &end_pos, float scale, std::wstring &str_content,
                 const vec4 &txt_col, vec2 &l_top_edge,
                 vec2 &r_bottom_edge, float &line_spacing, bool be_new);
} // namespace font_manager
} // namespace vg