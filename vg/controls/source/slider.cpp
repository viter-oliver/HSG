#include "slider.h"
#include "common_functions.h"
#include <sstream>
#include "file_chunk_container.h"
#include "packing_texture_container.h"
#include "user_control_imgui.h"
//#define  IMGUI_DEFINE_MATH_OPERATORS
namespace vg {
enum en_slide_direticn {
  en_horizontal,
  en_vertical,
  en_custom,
  en_direction_cnt
};

const char *const direction_iitem[en_direction_cnt] = {"horizontal", "vertical",
                                                       "custom"};
bool slider::read_point_position_file(const char *str) {
  auto ij = _mp_file_chunk.find(str);
  if (ij == _mp_file_chunk.end()) {
    return false;
  }
  _ps_track_file = ij->second;
  char *pData = (char *)_ps_track_file->_pbin;
  auto buff_sz = _ps_track_file->_fsize;
  auto alen =
      conver_track_buff_to_pair(pData, buff_sz, _custom_track0, _custom_track1);
  if (alen > 0) {
    _custom_mid_track.resize(alen);
    _custom_track_segment.clear();
    vec2 prev_midpoint = {-1, 0};
    _custom_trace_length = 0;
    for (int ix = 0; ix < alen; ++ix) {
      auto &track0 = _custom_track0[ix];
      auto &track1 = _custom_track0[ix];
      auto &mid_track = _custom_mid_track[ix];
      mid_track = track0 + track1;
      mid_track = mid_track * 0.5;
      if (prev_midpoint.x >= 0) {
        _custom_track_segment.emplace_back();
        auto &cur_seg = _custom_track_segment.back();
        auto vt_2_pt = mid_track - prev_midpoint;
        cur_seg = vt_2_pt.norm();
        _custom_trace_length += cur_seg;
      }
      prev_midpoint = mid_track;
    }
  }
  return true;
}

slider::slider(){
  _slider_pt._track_path_text[0] = '\0';
  _slider_pt._texture_bg_index_txt = _slider_pt._texture_head_index_txt =
  _slider_pt._texture_thumb_index_txt = {0, 0};
  // reg_property_handle(&_slider_pt, 0, [this](void*){});
}
void slider::link() { read_point_position_file(_slider_pt._track_path_text); }


void slider::draw() {
  // base::draw();
  auto &bg_gp_id = _slider_pt._texture_bg_index_txt.x;
  auto &bg_txt_index = _slider_pt._texture_bg_index_txt.y;
  auto &cur_res_list = *packing_texture::_v_sd_packing_texture[bg_gp_id];
  int texture_id = cur_res_list.txt_id;
  auto &pvtext_cd = cur_res_list.vtexture_coordinate;
  if (pvtext_cd.size() == 0)
    return;
  if (bg_txt_index >= pvtext_cd.size()) {
    printf("invalid texture index:%d\n", bg_txt_index);
    return;
  }
  auto& sub_unit = *pvtext_cd[bg_txt_index];
  int bg_gp_texture_width = cur_res_list._width;
  int bg_gp_texture_height = cur_res_list._height;
  float sizew = _slider_pt._bg_txtw;
  float sizeh = _slider_pt._bg_txth;
  auto abpos = absolute_coordinate_of_base_pos();
  auto winpos = ImGui::GetWindowPos();
  ImVec2 screen_base_pos = winpos+abpos;
  ImVec2 pos1 = screen_base_pos;
  ImVec2 pos2 = {pos1.x, pos1.y + sizeh};
  ImVec2 pos3 = {pos1.x + sizew, pos1.y + sizeh};
  ImVec2 pos4 = {pos1.x + sizew, pos1.y};
  ImVec2 uv0 = ImVec2(sub_unit._x0 / bg_gp_texture_width,
      sub_unit._y0 / bg_gp_texture_height);
  ImVec2 uv1 = ImVec2(sub_unit._x0 / bg_gp_texture_width,
                      sub_unit._y1 / bg_gp_texture_height);
  ImVec2 uv2 = ImVec2((sub_unit._x1) / bg_gp_texture_width,
      sub_unit._y1 / bg_gp_texture_height);
  ImVec2 uv3 = ImVec2((sub_unit._x1) / bg_gp_texture_width,
      sub_unit._y0 / bg_gp_texture_height);

  float offsetx = abpos.x - base_pos().x;
  float offsety = abpos.y - base_pos().y;

  ImGui::ImageQuad((ImTextureID)texture_id, pos1, pos2, pos3, pos4, uv0, uv1,
                   uv2, uv3);
  auto &hd_gp_id = _slider_pt._texture_head_index_txt.x;
  auto &txt_hd_id = _slider_pt._texture_head_index_txt.y;
  auto &hd_res_gp = *packing_texture::_v_sd_packing_texture[hd_gp_id];
  int hd_texture_id = hd_res_gp.txt_id;
  int hd_gp_texture_width = hd_res_gp._width;
  int hd_gp_texture_height = hd_res_gp._height;
  auto &hd_text_cd = hd_res_gp.vtexture_coordinate;

  /***********************************************************progress*********************************************************/
  if (txt_hd_id >= hd_text_cd.size()) {
    printf("invalid texture index:%d\n", txt_hd_id);
    return; // txt_hd_id = 0;
  }
  if (0 == _custom_trace_length)
    return; //第一次进入random时判断
  ImVec2 txt_size(hd_gp_texture_width, hd_gp_texture_height);
  ImVec2 uv_org = ImVec2((hd_text_cd[txt_hd_id]->_x0) / hd_gp_texture_width,
                         (hd_text_cd[txt_hd_id]->_y0) / hd_gp_texture_height);
  float real_length = _custom_trace_length * _slider_pt._progress_nml;
  vec2 value_point0, value_point1, dir_thumb0, dir_thumb1;
  float tmp_length = 0.f;
  int idx = 0;
  for (auto seg_unit : _custom_track_segment) {
    auto test_len = tmp_length + seg_unit;
    if (test_len <= real_length) {
      tmp_length = test_len;
      pos1 = screen_base_pos + _custom_track0[idx];     // pp_prev._point0;
      pos2 = screen_base_pos + _custom_track1[idx];     // pp_prev._point1;
      pos3 = screen_base_pos + _custom_track1[idx + 1]; // pp_cur._point1;
      pos4 = screen_base_pos + _custom_track0[idx + 1]; // pp_cur._point0;
      uv0 = uv_org + to_imvec2(_custom_track0[idx]) / txt_size;
      uv1 = uv_org + to_imvec2(_custom_track1[idx]) / txt_size;
      uv2 = uv_org + to_imvec2(_custom_track1[idx + 1]) / txt_size;
      uv3 = uv_org + to_imvec2(_custom_track0[idx + 1]) / txt_size;
      ImGui::ImageQuad((ImTextureID)hd_texture_id, pos1, pos2, pos3, pos4, uv0,
                       uv1, uv2, uv3);
      if (test_len == real_length) {
        value_point0 = _custom_track0[idx];
        value_point1 = _custom_track1[idx];
        dir_thumb0 = _custom_track0[idx + 1] - _custom_track0[idx];
        dir_thumb0 = _custom_track1[idx + 1] - _custom_track1[idx];
        break;
      }
      idx++;
    } else {
      float delta = test_len - real_length;
      auto cur_seg = _custom_track_segment[idx];
      delta = cur_seg - delta;
      auto dir0 = _custom_track0[idx + 1] - _custom_track0[idx];
      auto dir0_nm = dir0.norm();
      auto dir1 = _custom_track1[idx + 1] - _custom_track1[idx];
      auto dir1_nm = dir1.norm();
      value_point0 = _custom_track0[idx] + dir0 * (delta / dir0_nm);
      value_point1 = _custom_track1[idx] + dir1 * (delta / dir1_nm);
      dir_thumb0 = dir0;
      dir_thumb1 = dir1;
      pos1 = screen_base_pos + _custom_track0[idx];
      pos2 = screen_base_pos + _custom_track1[idx];
      pos3 = screen_base_pos + value_point1;
      pos4 = screen_base_pos + value_point0;
      uv0 = uv_org + to_imvec2(_custom_track0[idx]) / txt_size;
      uv1 = uv_org + to_imvec2(_custom_track1[idx]) / txt_size;
      uv2 = uv_org + to_imvec2(value_point1) / txt_size;
      uv3 = uv_org + to_imvec2(value_point0) / txt_size;
      ImGui::ImageQuad((ImTextureID)hd_texture_id, pos1, pos2, pos3, pos4, uv0,
                       uv1, uv2, uv3);
      break;
    }
  }

  /************************************************thumb**************************************************/
  if (!_slider_pt._thumb_visible)
    return;
  auto tb_height = _slider_pt._tb_height;
  auto tb_delta = tb_height / 2 - _slider_pt._tb_offset;
  auto tb_dir0_nm =dir_thumb0.norm();
  auto tb_point0 = value_point0 - dir_thumb0 * (tb_delta / tb_dir0_nm);
  auto tb_dir1_nm = dir_thumb1.norm();
  auto tb_point1 = value_point1 - dir_thumb1 * (tb_delta / tb_dir1_nm);

  auto tb_point2 = tb_point1 + dir_thumb1 * (tb_height / tb_dir1_nm);
  auto tb_point3 = tb_point0 + dir_thumb0 * (tb_height / tb_dir0_nm);
  pos1 = screen_base_pos + tb_point0;
  pos2 = screen_base_pos + tb_point1;
  pos3 = screen_base_pos + tb_point2;
  pos4 = screen_base_pos + tb_point3;
  auto &tb_gp_id = _slider_pt._texture_thumb_index_txt.x;
  auto &tb_id = _slider_pt._texture_thumb_index_txt.y;
  auto &tb_re_gp = *packing_texture::_v_sd_packing_texture[tb_gp_id];
  int tb_texture_id = tb_re_gp.txt_id;
  int tb_gp_texture_width = tb_re_gp._width;
  int tb_gp_texture_height = tb_re_gp._height;
  auto &vtb_text_cd = tb_re_gp.vtexture_coordinate;
  auto& tb_text_cd = *vtb_text_cd[tb_id];
  uv0 = ImVec2(tb_text_cd._x0 / tb_gp_texture_width,
               tb_text_cd._y0 / tb_gp_texture_height);
  uv1 = ImVec2(tb_text_cd._x0 / tb_gp_texture_width,
               (tb_text_cd._y1) / tb_gp_texture_height);
  uv2 = ImVec2((tb_text_cd._x1) / tb_gp_texture_width,
               (tb_text_cd._y1) / tb_gp_texture_height);
  uv3 = ImVec2((tb_text_cd._x1) / tb_gp_texture_width,
               (tb_text_cd._y0) / tb_gp_texture_height);
  ImGui::ImageQuad((ImTextureID)tb_texture_id, pos1, pos2, pos3, pos4, uv1, uv2,
                   uv3, uv0);
}
} // namespace vg
