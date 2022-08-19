#include "font_manager.h"
#include <assert.h>
#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include "glad.h"
#endif
#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
namespace vg {

font_txt_repository::~font_txt_repository() { glDeleteTextures(1, &_txt_id); }
namespace font_manager {
using namespace std;
FT_Library _ft;
struct  font_assist
{
  font_assist(){
    assert(!FT_Init_FreeType(&_ft) && "fail to init freetype library!");
  }
} _font_assist;

u32 _fmbf_id{0};
void clear_texture(u32 &txt_id) {
  GLint last_fmid;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last_fmid);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fmbf_id);
  glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, txt_id,
                       0);            // Only need to do this once.
  glDrawBuffer(GL_COLOR_ATTACHMENT0); // Only need to do this once.
  GLuint clearColor[4] = {0, 0, 0, 0};
  glClearBufferuiv(GL_COLOR, 0, clearColor);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, last_fmid);
}
static void convert_r_to_rgba(u8 *pred, u32 *prgba, u32 data_len) {
  for (size_t i = 0; i < data_len; i++) {
    //*prgba++ = IM_COL32(255, 255, 255, (unsigned int)(*pred++));
    uint8_t *pdst = (uint8_t *)(prgba);
    pdst += 3;
    *pdst = *pred++;
    prgba++;
  }
}
void init_txt_font(u32 fontSize, font_txt_repository &newFrep) {
  newFrep._border = {0, 0};
  newFrep._font_size = fontSize;
  glGenTextures(1, &newFrep._txt_id);
  GLint last_fmid;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last_fmid);
  glBindTexture(GL_TEXTURE_2D, newFrep._txt_id);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newFrep._txt_size.x,
               newFrep._txt_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, last_fmid);
}
void load_chars(FT_Face &fontFace, font_txt_repository &fp,
                std::wstring &wchar_list) {
  dic_glyph_txt &container = fp._dic_txt_cd;
  GLint &max_bearingy = fp._max_bearing;
  GLuint &txtid = fp._txt_id;
  assert(txtid &&
         "you must pass a valid texture id into the function load_chars!");
  vui2 &border = fp._border;
  vi2 &txt_size = fp._txt_size;
  GLuint &fontSize = fp._font_size;
  bool &be_full = fp._full;

  glBindTexture(GL_TEXTURE_2D, txtid);
  auto &face = fontFace;
  FT_Set_Pixel_Sizes(face, 0, fontSize);
  for (auto &str_it : wchar_list) {
    auto &it_glyph = container.find(str_it);
    if (it_glyph != container.end()) {
      auto &ch_glhph = it_glyph->second;
      auto &by = ch_glhph._bearing.y;
      if (by > max_bearingy) {
        max_bearingy = by;
      }
      continue;
    }

    if (FT_Load_Char(face, str_it, FT_LOAD_RENDER)) {
      wprintf(L"fail to find %c in font face ", str_it);
      continue;
    }
    auto tw = face->glyph->bitmap.width;
    auto th = face->glyph->bitmap.rows;
    auto lt = face->glyph->bitmap_left;
    auto tp = face->glyph->bitmap_top;
    if (tp > max_bearingy) {
      max_bearingy = tp;
    }
    auto ad = face->glyph->advance.x;
    float x0 = (float)border.x / (float)txt_size.x;
    float y0 = (float)border.y / (float)txt_size.y;

    auto &tbuff = face->glyph->bitmap.buffer;
    auto txt_sz = tw * th * 4;
    uint32_t *prgba = new uint32_t[txt_sz];
    memset(prgba, 0xff, txt_sz);
    convert_r_to_rgba(tbuff, prgba, tw * th);

    float left_x = border.x + tw;
    float next_x = left_x + 2;
    if (next_x <= txt_size.x) {
      glTexSubImage2D(GL_TEXTURE_2D, 0, border.x, border.y, tw, th, GL_RGBA,
                      GL_UNSIGNED_BYTE, prgba);
      border.x = next_x;
    } else {
      border.y += fontSize;
      border.y += 2;
      if ((border.y + th) > txt_size.y) // is full
      {
        fprintf(stderr, "font glyph repository of control is full!\n");
        be_full = true;
        delete[] prgba;
        return;
      }
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, border.y, tw, th, GL_RGBA,
                      GL_UNSIGNED_BYTE, prgba);
      border.x = tw + 2;
    }
    float x1 = (float)left_x / (float)txt_size.x;
    float y1 = (float)(th + border.y) / (float)txt_size.y;
    // border.x += 5;
    delete[] prgba;
    auto sd_funit = make_shared<font_txt_coordinate>();
    *sd_funit = {{tw, th}, {lt, tp}, ad, x0, y0, x1, y1};
    container[str_it] = sd_funit;
    ;
  }
}
bool load_font(font_unit& ft, uint8_t *pfont_buff,
                       FT_Long file_size) {

  FT_Error err = FT_New_Memory_Face(_ft, pfont_buff, file_size, 0, &ft._ft_face);
  if (err) {
    printf("fail to load font from:%s!\n", fontFaceName.c_str());
    return false;
  }
  FT_Select_Charmap(ft._ft_face, FT_ENCODING_UNICODE);
  return true;
}
bool load_font(font_unit& ft,string& fontPath){
  if (FT_New_Face(_ft,fontPath.c_str(),&ft._ft_face)){
    return false;
  } else {
    return true;
  }
}
  enum omit_type
	{
		en_no_omit,
		en_omit_rest,
		en_omit_rest_with_ellipsis,
		en_omit_type_number
	};
int draw_wstring(ps_font_unit &pf_u, FT_Long fontSize, vec2 &start_pos,
                 vec2 &end_pos, float scale, std::wstring &str_content,
                 const vec4 &txt_col, float width, int omit_rest,
                 bool be_new) {
  font_txt_repository *pfrp = nullptr;
  GLint max_beary = 0;
  // auto& ifont = _dic_fonts.find(fontFace);
  auto &f_u = *pf_u;
  const auto &irep = f_u._ft_rep.find(fontSize);
  if (irep != f_u._ft_rep.end()) {
    pfrp = &irep->second;
  } else {
    f_u._ft_rep[fontSize]._txt_size = {f_u._char_count_r * fontSize,
                                       f_u._char_count_c * fontSize};
    init_txt_font(fontSize, f_u._ft_rep[fontSize]);
    pfrp = &f_u._ft_rep[fontSize];
  }
  if (!pfrp->_full) {
    load_chars(f_u._ft_face, *pfrp, str_content);
  }
  max_beary = pfrp->_max_bearing;
  bool be_break = str_content[0] == L'O' && str_content[1] == L'S';

  end_pos = start_pos;
  float base_line = start_pos.y + (float)max_beary;
  float str_most_right_edge = start_pos.x + width;
  float str_real_right_edg = start_pos.x;
  float maxy = 0.f;
  GLuint &txt_id = pfrp->_txt_id;
  dic_glyph_txt &txt_cd_container = pfrp->_dic_txt_cd;
  bool will_omit_test = false;
  int cnt_char = 0;
  int cnt_char_w = str_content.size();
  for (auto &wstr_item : str_content) {
    const auto &glyph_txt_it = txt_cd_container.find(wstr_item);
    if (glyph_txt_it != txt_cd_container.end()) {
      auto &glyph_txt_cd = glyph_txt_it->second;
      auto bearing = glyph_txt_cd._bearing;
      auto tsize = glyph_txt_cd._size;
      float x0 = glyph_txt_cd._x0;
      float x1 = glyph_txt_cd._x1;
      float y0 = glyph_txt_cd._y0;
      float y1 = glyph_txt_cd._y1;
      auto advance = glyph_txt_cd._advance;
      auto bearing_x = bearing.x * scale;
      float char_left_edge = end_pos.x + bearing_x;
      float char_right_edge = char_left_edge + tsize.x * scale; // +bearing_x_n;
      cnt_char++;
      if (char_right_edge > str_most_right_edge) {
        if (omit_rest != en_no_omit) {
          // if (cnt_char < cnt_char)
          {
            wstring omit_sign = omit_rest == en_omit_rest ? L" " : L"…";
            load_chars(pf_u->_ft_face, *pfrp, omit_sign);
            auto &glyph_omit = txt_cd_container.find(omit_sign[0]);
            auto &glyph_omit_txt_cd = glyph_omit->second;
            bearing = glyph_omit_txt_cd._bearing;
            tsize = glyph_omit_txt_cd._size;
            x0 = glyph_omit_txt_cd._x0;
            x1 = glyph_omit_txt_cd._x1;
            y0 = glyph_omit_txt_cd._y0;
            y1 = glyph_omit_txt_cd._y1;
            advance = glyph_omit_txt_cd._advance;
            char_left_edge = end_pos.x + bearing.x * scale;
            char_right_edge = char_left_edge + tsize.x * scale;
          }

          will_omit_test = true;
        } else {
          end_pos.x = start_pos.x;
          base_line = maxy + max_beary;
          char_left_edge = end_pos.x + bearing.x * scale;
          char_right_edge = char_left_edge + tsize.x * scale;
        }
      }
      ImVec2 pos0{char_left_edge, base_line - bearing.y * scale};
      ImVec2 pos1{pos0.x, pos0.y + tsize.y * scale};
      ImVec2 pos2{char_right_edge, pos1.y};
      ImVec2 pos3{pos2.x, pos0.y};
      ImVec2 uv0{x0, y0};
      ImVec2 uv1{x0, y1};
      ImVec2 uv2{x1, y1};
      ImVec2 uv3{x1, y0};
      ImVec4 dcol{txt_col.x, txt_col.y, txt_col.z, txt_col.w};
      if (!be_new)
        ImGui::ImageQuad((ImTextureID)txt_id, pos0, pos1, pos2, pos3, uv0, uv1, uv2,
                  uv3, dcol);
      float shift_dis =
          (advance >> 6) *
          scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
      // end_pos.x += bearing_x_n;
      end_pos.x += shift_dis;
      if (end_pos.x > str_real_right_edg) {
        str_real_right_edg = end_pos.x;
      }
      if (maxy < pos1.y) {
        maxy = pos1.y;
      }
      if (will_omit_test) {
        break;
      }
    }
  }
  end_pos.x = str_real_right_edg;
  end_pos.y = maxy;
  return cnt_char;
}
int draw_wstring(ps_font_unit &pf_u, FT_Long fontSize, vec2 &start_pos,
                 vec2 &end_pos, float scale, std::wstring &str_content,
                 const vec4 &txt_col, vec2 &l_top_edge,
                 vec2 &r_bottom_edge, float &line_spacing, bool be_new) {
  font_txt_repository *pfrp = nullptr;
  GLint max_beary = 0;
  // auto& ifont = _dic_fonts.find(fontFace);
  auto &f_u = *pf_u;
  const auto &irep = f_u._ft_rep.find(fontSize);
  if (irep != f_u._ft_rep.end()) {
    pfrp = &irep->second;
  } else {
    f_u._ft_rep[fontSize]._txt_size = {f_u._char_count_r * fontSize,
                                       f_u._char_count_c * fontSize};
    init_txt_font(fontSize, f_u._ft_rep[fontSize]);
    pfrp = &f_u._ft_rep[fontSize];
  }
  if (!pfrp->_be_full) {
    load_chars(f_u._ft_face, *pfrp, str_content);
  }
  max_beary = pfrp->_max_bearingy;
  end_pos = start_pos;
  float base_line = start_pos.y + (float)max_beary;
  float maxy = base_line;
  GLuint &txt_id = pfrp->_txt_id;
  dic_glyph_txt &txt_cd_container = pfrp->_dic_txt_cd;
  int cnt_char = 0;
  int cnt_char_w = str_content.size();
  bool be_first_return = true;
  for (auto &wstr_item : str_content) {
    const auto &glyph_txt_it = txt_cd_container.find(wstr_item);
    if (glyph_txt_it != txt_cd_container.end()) {
      auto &glyph_txt_cd = glyph_txt_it->second;
      auto bearing = glyph_txt_cd._bearing;
      auto tsize = glyph_txt_cd._size;
      float x0 = glyph_txt_cd._x0;
      float x1 = glyph_txt_cd._x1;
      float y0 = glyph_txt_cd._y0;
      float y1 = glyph_txt_cd._y1;
      auto advance = glyph_txt_cd._advance;
      auto bearing_x = bearing.x * scale;
      float char_left_edge = end_pos.x + bearing_x;
      float char_right_edge = char_left_edge + tsize.x * scale; // +bearing_x_n;
      cnt_char++;
      if (char_right_edge > r_bottom_edge.x) {
        end_pos.x = l_top_edge.x;

        base_line = base_line + max_beary + line_spacing;
        char_left_edge = end_pos.x + bearing.x * scale;
        char_right_edge = char_left_edge + tsize.x * scale;
        auto new_top = base_line - max_beary;
        if (be_first_return) {
          be_first_return = false;
          l_top_edge.y = new_top;
        }
        r_bottom_edge.y = new_top;
      }
      ImVec2 pos0{char_left_edge, base_line - bearing.y * scale};
      ImVec2 pos1{pos0.x, pos0.y + tsize.y * scale};
      ImVec2 pos2{char_right_edge, pos1.y};
      ImVec2 pos3{pos2.x, pos0.y};
      ImVec2 uv0{x0, y0};
      ImVec2 uv1{x0, y1};
      ImVec2 uv2{x1, y1};
      ImVec2 uv3{x1, y0};
      ImVec4 dcol{txt_col.x, txt_col.y, txt_col.z, txt_col.w};
      if (!be_new)
        ImGui::ImageQuad((ImTextureID)txt_id, pos0, pos1, pos2, pos3, uv0, uv1, uv2,
                  uv3, dcol);
      float shift_dis =
          (advance >> 6) *
          scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
      // end_pos.x += bearing_x_n;
      end_pos.x += shift_dis;
      if (maxy < pos1.y) {
        maxy = pos1.y;
      }
    }
  }
  end_pos.y = base_line + max_beary;
  return cnt_char;
}
} // namespace font_manager
} // namespace vg