#include "font_manager.h"
#include <assert.h>
#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include "glad.h"
#endif
#include "user_control_imgui.h"
#include<iostream>
#include<algorithm>
namespace vg {
namespace font_manager{

using namespace std;
FT_Library _ft;
struct  font_assist
{
  font_assist(){
    assert(!FT_Init_FreeType(&_ft) && "fail to init freetype library!");
  }
} _font_assist;

u32 _fmbf_id{0};
font_txt_coordinate::~font_txt_coordinate()
{
     if( _txt_id )
     {
          glDeleteTextures( 1, &_txt_id );
     }
}
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
u32 get_font_txt(u32 txt_w,u32 txt_h,u8* pdata) {
  u32 txt_id;
  glGenTextures(1, &txt_id );
  GLint last_fmid;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last_fmid);
  glBindTexture(GL_TEXTURE_2D,txt_id);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, txt_w,
                txt_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, txt_w, txt_h,
                0, GL_RGBA, GL_UNSIGNED_BYTE, pdata );
  glBindTexture(GL_TEXTURE_2D, last_fmid);
  return txt_id;
}

//检查并加载字体纹理信息
void check_chars(FT_Face &fontFace, dic_glyph_txt&fp,
                std::wstring &wchar_list) {
  dic_glyph_txt &container = fp;

  //glBindTexture(GL_TEXTURE_2D, txtid);
  auto &face = fontFace;
  for (auto &str_it : wchar_list) {
    const auto &it_glyph = container.find(str_it);
    if (it_glyph != container.end()) {
     
      continue;
    }
    sd_font_txt_coordinate sd_font_txt_coordinate_ = make_shared<font_txt_coordinate>();
    
    container[ str_it ] = sd_font_txt_coordinate_;
    if (FT_Load_Char(face, str_it, FT_LOAD_RENDER)) {
      wprintf(L"fail to find %c in font face ", str_it);
      continue;
    }
    auto tw = face->glyph->bitmap.width;
    auto th = face->glyph->bitmap.rows;
    auto lt = face->glyph->bitmap_left;
    auto tp = face->glyph->bitmap_top;
    
    auto ad = face->glyph->advance.x;
     *sd_font_txt_coordinate_ = { {tw, th}, {lt, tp}, (u32)ad, 0 };
    auto &tbuff = face->glyph->bitmap.buffer;
    auto txt_sz = tw * th * 4;
    uint32_t *prgba = new uint32_t[txt_sz];
    memset(prgba, 0xff, txt_sz);
    convert_r_to_rgba(tbuff, prgba, tw * th);
    sd_font_txt_coordinate_->_txt_id = get_font_txt( tw, th,(u8*)prgba );
    // border.x += 5;
    delete[] prgba;

  }
}

bool load_font(font_unit& ft, uint8_t *pfont_buff,
                       FT_Long file_size) {
  FT_Error err =
      FT_New_Memory_Face(_ft, pfont_buff, file_size, 0, &ft._ft_face[0]);
  if (err) {
    printf("fail to load font bufffer!\n");
    return false;
  }
  FT_Select_Charmap(ft._ft_face[0], FT_ENCODING_UNICODE);
  for (int ix = 1; ix < ft._ft_face[0]->num_faces; ix++) {
    err = FT_New_Memory_Face(_ft, pfont_buff, file_size, 0, &ft._ft_face[ix]);
    if (err) {
      printf("fail to load font bufffer!\n");
      return false;
    }
    ft.face_count = ix + 1;
  }
  return true;
}
bool load_font(font_unit& ft,const string& fontPath){
  if (FT_New_Face(_ft, fontPath.c_str(), 0, &ft._ft_face[0])) {
    return false;
  } else {
    for (int ix = 1; ix < ft._ft_face[0]->num_faces; ix++) {
      if (FT_New_Face(_ft, fontPath.c_str(), 0, &ft._ft_face[ix])) {
        return false;
      }
      ft.face_count = ix + 1;
    }
    return true;
  }
}

int draw_wstring(font_unit &f_u, FT_Long fontSize, vec2 &start_pos,
                 vec2 &end_pos, float scale, std::wstring &str_content,
                 const vec4 &txt_col, float width, int omit_rest, 
                 float& first_line_offset,bool be_new,
                 u8 face_id) {
  GLint max_beary = 0;
  sd_dic_glyph_txt pfrp = nullptr;
  const auto &irep = f_u._ft_rep.find(fontSize);
  if (irep != f_u._ft_rep.end()) {
    pfrp = irep->second;
  } else {
       pfrp = make_shared<dic_glyph_txt>();
       f_u._ft_rep[fontSize]= pfrp;
  }
  auto& font_container = *pfrp;
  FT_Set_Pixel_Sizes(f_u._ft_face[face_id], 0, fontSize);
  check_chars(f_u._ft_face[face_id], font_container, str_content);

  bool be_break = str_content[0] == L'O' && str_content[1] == L'S';

  end_pos = start_pos;
  float base_line = start_pos.y() + (float)max_beary;
  float str_most_right_edge = start_pos.x() + width;
  float str_real_right_edg = start_pos.x();
  
  float maxy = 0.f;
  
  bool will_omit_test = false;
  int cnt_char = 0;
  int cnt_char_w = str_content.size();
  //gray@2024.09.26
  float line_height = fontSize * scale;
  //start_pos.y -= line_height;
  bool first_line_flag = 1;
  float cur_first_line_offset = 0;
  for (auto &wstr_item : str_content) 
  {
    const auto &glyph_txt_it = font_container.find(wstr_item);
    if (glyph_txt_it != font_container.end()) 
    {
      auto &glyph_txt_cd = *glyph_txt_it->second;
      auto bearing = glyph_txt_cd._bearing;
      auto tsize = glyph_txt_cd._size;
      auto advance = glyph_txt_cd._advance;
      auto bearing_x = bearing.x()* scale;
      float char_left_edge = end_pos.x() + bearing.x();
      float char_right_edge = char_left_edge + tsize.x() * scale; // +bearing_x_n;
      cnt_char++;
      if (char_right_edge > str_most_right_edge) {
        if (omit_rest != en_no_omit) 
        {
          // if (cnt_char < cnt_char)
          {
            wstring omit_sign = omit_rest == en_omit_rest ? L" " : L"...";
            check_chars(f_u._ft_face[face_id], *pfrp, omit_sign);
            const auto &glyph_omit = font_container.find(omit_sign[0]);
            auto &glyph_omit_txt_cd = *glyph_omit->second;
            bearing = glyph_omit_txt_cd._bearing;
            tsize = glyph_omit_txt_cd._size;
            advance = glyph_omit_txt_cd._advance;
            char_left_edge = end_pos.x() + bearing.x() * scale;
            char_right_edge = char_left_edge + tsize.x() * scale;
          }

          will_omit_test = true;
        } 
        else
        {
          /*
          end_pos.x = start_pos.x;
          base_line = maxy + max_beary;
          char_left_edge = end_pos.x + bearing.x * scale;
          char_right_edge = char_left_edge + tsize.x * scale;
          */

          // 换行逻辑
          end_pos.x() = start_pos.x(); // 新行开始时重置 x 位置
          end_pos.y() += line_height; // 向下移动 y 位置
          base_line = end_pos.y(); // 更新基线
          char_left_edge = end_pos.x() + bearing.x() * scale;
          char_right_edge = char_left_edge + tsize.x() * scale;
          first_line_flag = 0;
        }
      }
      ImVec2 pos0{char_left_edge, base_line - bearing.y() * scale};   //left-up
      ImVec2 pos1{pos0.x, pos0.y + tsize.y() * scale};                //left-bottle
      ImVec2 pos2{char_right_edge, pos1.y};                         //right-bottle
      ImVec2 pos3{pos2.x, pos0.y};                                  //right-up

      start_pos.y() = start_pos.y()< pos0.y ? start_pos.y() : pos0.y;
      end_pos.y() = end_pos.y() > pos1.y ? end_pos.y() : pos1.y;
      if(first_line_flag)
      cur_first_line_offset = cur_first_line_offset > (pos1.y - pos0.y) ? cur_first_line_offset : (pos1.y - pos0.y);


      ImVec2 uv0{0, 0};             //left-bottle
      ImVec2 uv1{0, 1};             //left-up
      ImVec2 uv2{1, 1};             //right-up
      ImVec2 uv3{1, 0};             //right-bottle
      ImVec4 dcol{txt_col.x(), txt_col.y(), txt_col.z(), txt_col.w()};
      if (!be_new)
           imgui_ImageQuad((ImTextureID)glyph_txt_cd._txt_id, pos0, pos1, pos2, pos3, uv0, uv1, uv2,
                  uv3, dcol);
      float shift_dis =
          (advance >> 6) *
          scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
      // end_pos.x += bearing_x_n;
      end_pos.x() += shift_dis;
      
      //考虑在这里做换行操作
      if (end_pos.x() > str_real_right_edg)
      {
        str_real_right_edg = end_pos.x();
      }
      if (maxy < pos1.y) {
      }
      if (will_omit_test) {
        break;
      }
    }
  }
  end_pos.x() = str_real_right_edg;
  //end_pos.y = maxy;
  first_line_offset = cur_first_line_offset;
  return cnt_char;
}
} // namespace font_manager
} // namespace vg