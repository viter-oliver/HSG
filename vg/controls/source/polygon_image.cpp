#include <sstream>
#include "polygon_image.h"
#include "common_functions.h"
#include "file_chunk_container.h"
#include "packing_texture_container.h"
#include "user_control_imgui.h"
namespace vg {
void polygon_image::read_track_file(const char *track_file) {
  auto &ifile = _mp_file_chunk.find(track_file);
  if (ifile != _mp_file_chunk.end()) {
    auto &p_trk_file = ifile->second;
    char *pData = (char *)p_trk_file->_pbin;
    auto buff_sz = p_trk_file->_fsize;
    auto alen = conver_track_buff_to_pair(pData, buff_sz, _track0, _track1);
    _track_segs0.clear();
    _track_segs1.clear();
    _seg_len0 = _seg_len1 = 0.;
    _ps_track_file = ifile->second;
    if (alen < 2) {
      _ps_track_file = nullptr;
      return;
    }
    for (int ix = 0; ix < (int)alen - 1; ix++) {
      auto vpt = _track0[ix + 1] - _track0[ix];
      auto back0 = vpt.norm();
      _seg_len0 += back0;
      _track_segs0.emplace_back(back0);
      auto vpt1 = _track1[ix + 1] - _track1[ix];
      auto back1 = vpt1.norm();
      _seg_len1 += back1;
      _track_segs1.emplace_back(back1);
    }
  }
}
void polygon_image::link() { read_track_file(_img_pt._track_file); }
polygon_image::polygon_image() { _img_pt._img_txt = {0, 0}; }
void polygon_image::draw() {
  if (!_ps_track_file) {
    return;
  }
  auto &img_id = _img_pt._img_txt;
  auto &cur_res_list = *packing_texture::_v_sd_packing_texture[img_id.x];
  int texture_id = cur_res_list.txt_id;
  packing_texture::sub_texture_coordinate ptext_cd = { 0,0,1,1 };
  if(img_id.y< cur_res_list.vtexture_coordinate.size())
    ptext_cd = *cur_res_list.vtexture_coordinate[img_id.y];

  int tw = cur_res_list._width;
  int th = cur_res_list._height;
  auto abpos = absolute_coordinate_of_base_pos();
  ImVec2 winpos = ImGui::GetWindowPos();
  ImVec2 basePpos = winpos+abpos;
  float x0 = ptext_cd._x0 / tw;
  float y0 = ptext_cd._y0 / th;
  float x1 = ptext_cd._x1 / tw;
  float y1 = ptext_cd._y1 / th;
  float wdelta = (y1 - y0) / 80.0;
  y0 += wdelta;
  y1 -= wdelta;
  float udelta = x1 - x0;
  float vdelta = y1 - y0;
  auto pt_cnt = _track0.size();
  if (pt_cnt < 2) {
    return;
  }
  float prs_value0 = _img_pt._progrss_nml * _seg_len0;
  float prs_value1 = _img_pt._progrss_nml * _seg_len1;
  float prs_value = (prs_value0 + prs_value1) / 2.0;
  int i_cross = 0;
  float tmp_len = 0.;
  for (; i_cross < pt_cnt - 1; i_cross++) // find the cross segments
  {
    auto av_seg = (_track_segs0[i_cross] + _track_segs0[i_cross]) / 2.0;
    tmp_len += av_seg;
    if (tmp_len > prs_value) {
      break;
    }
  }

  float v_seg0 = 0., v_seg1 = 0.;
  float ybase = y0 + (1 - _img_pt._progrss_nml) * vdelta;
  float v_p0 = 0, v_p1 = 0, v_n0 = 0, v_n1 = 0;
  for (int ix = 0; ix < i_cross; ix++) {
    ImVec2 pos0 = basePpos + _track0[ix];
    ImVec2 pos1 = basePpos + _track0[ix + 1];
    ImVec2 pos2 = basePpos + _track1[ix + 1];
    ImVec2 pos3 = basePpos + _track1[ix];
    v_p0 = v_seg0 / _seg_len0;
    v_p1 = v_seg1 / _seg_len1;
    v_seg0 += _track_segs0[ix];
    v_seg1 += _track_segs1[ix];

    v_n0 = v_seg0 / _seg_len0;
    v_n1 = v_seg1 / _seg_len1;
    ImVec2 uv0(x0, ybase + v_p0 * vdelta);
    ImVec2 uv1(x0, ybase + v_n0 * vdelta);
    ImVec2 uv2(x1, ybase + v_n1 * vdelta);
    ImVec2 uv3(x1, ybase + v_p1 * vdelta);
    ImGui::ImageQuad((ImTextureID)texture_id, pos0, pos1, pos2, pos3, uv0, uv1,
                     uv2, uv3);
  }
  float delta_len0 = prs_value0 - v_seg0;
  float delta_len1 = prs_value1 - v_seg1;

  ImVec2 cs_pos0 = basePpos + _track0[i_cross];
  ImVec2 cs_uv0(x0, ybase + v_n0 * vdelta);
  auto dir0 = _track0[i_cross + 1] - _track0[i_cross];
  float dir0_len = dir0.norm();
  ImVec2 dir0_nml = to_imvec2( dir0) / dir0_len;
  ImVec2 cs_pos_c0 = cs_pos0 + dir0_nml * delta_len0;
  float vc0 = prs_value0 / _seg_len0;
  ImVec2 cs_uv_c0(x0, ybase + vc0 * vdelta);

  ImVec2 cs_pos1 = basePpos + _track1[i_cross];
  ImVec2 cs_uv1(x1, ybase + v_n1 * vdelta);
  auto dir1 = _track1[i_cross + 1] - _track1[i_cross];
  float dir1_len = dir1.norm();
  ImVec2 dir1_nml = to_imvec2( dir1) / dir1_len;
  ImVec2 cs_pos_c1 = cs_pos1 + dir1_nml * delta_len1;
  float vc1 = prs_value1 / _seg_len1;
  ImVec2 cs_uv_c1(x1, ybase + vc1 * vdelta);
  if (delta_len0 + delta_len1 > 0.0) {
    ImGui::ImageQuad((ImTextureID)texture_id, cs_pos0, cs_pos_c0, cs_pos_c1,
                     cs_pos1, cs_uv0, cs_uv_c0, cs_uv_c1, cs_uv1);
  }
  cs_pos_c0 = cs_pos0 + dir0_nml * (delta_len0 - delta_len0 / 10);
  cs_pos_c1 = cs_pos1 + dir1_nml * (delta_len1 - delta_len1 / 10);
  ybase = y0;
  cs_uv_c0 = {x0, ybase};
  cs_uv_c1 = {x0, ybase};
  ImVec2 cs_pos_s0 = basePpos + _track0[i_cross + 1];
  v_seg0 = dir0_len - delta_len0;
  float cs_v_p0 = v_seg0 / _seg_len0;
  ImVec2 cs_uv_s0(x0, ybase + cs_v_p0 * vdelta);

  ImVec2 cs_pos_s1 = basePpos + _track1[i_cross + 1];
  v_seg1 = dir1_len - delta_len1;
  float cs_v_p1 = v_seg1 / _seg_len1;
  ImVec2 cs_uv_s1(x1, ybase + cs_v_p1 * vdelta);
  ImGui::ImageQuad((ImTextureID)texture_id, cs_pos_c0, cs_pos_s0, cs_pos_s1,
                   cs_pos_c1, cs_uv_c0, cs_uv_s0, cs_uv_s1, cs_uv_c1);

  for (int ix = i_cross + 1; ix < pt_cnt - 1; ix++) {
    ImVec2 pos0 = basePpos + _track0[ix];
    ImVec2 pos1 = basePpos + _track0[ix + 1];
    ImVec2 pos2 = basePpos + _track1[ix + 1];
    ImVec2 pos3 = basePpos + _track1[ix];
    v_p0 = v_seg0 / _seg_len0;
    v_p1 = v_seg1 / _seg_len1;
    v_seg0 += _track_segs0[ix];
    v_seg1 += _track_segs1[ix];

    v_n0 = v_seg0 / _seg_len0;
    v_n1 = v_seg1 / _seg_len1;

    ImVec2 uv0(x0, ybase + v_p0 * vdelta);
    ImVec2 uv1(x0, ybase + v_n0 * vdelta);
    ImVec2 uv2(x1, ybase + v_n1 * vdelta);
    ImVec2 uv3(x1, ybase + v_p1 * vdelta);
    ImGui::ImageQuad((ImTextureID)texture_id, pos0, pos1, pos2, pos3, uv0, uv1,
                     uv2, uv3);
  }
  // base::draw();
}
bool polygon_image::contain(vec2& tar_pos) {
    v_vec2 polygon(_track0);
    polygon.insert(polygon.end(),_track1.begin(),_track1.end());
    return RayCastingAlgorithmPointWithin(polygon, tar_pos);
}
} // namespace vg