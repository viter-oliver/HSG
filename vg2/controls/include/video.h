#pragma once
#include "control_fac.h"

namespace vg {
class AFG_EXPORT video : public control_base {

  DEF_STRUCT_WITH_INIT(_img_pt, 
  (float, _aposx, {0.f}),
  (float, _aposy, {0.f}), 
  (float, _angle_nml, {0.f}),
  (float, _alpha_nml, {1.f}), 
  (vec3, _tin_clr))
  u32 _txt_id = {0};
  u32 _pboIds[2];
  int _txt_width = {0}, _txt_height = {0};
  char_50 _video_dev_name;
  bool _linked = false;
  void init_txt_obj();
  void init_pbo();
  void release_res();

public:
  video();
  ~video();
  void delink();
  void draw();
  void update_pixels(u32 *dst, int sz);
  void update_text_from_image_buff(u32 *pimage_buff, int buff_len);
  // GLuint get_txt_id(){ return _txt_id; }
};
REG_CTL_2_FAC(video);
} // namespace vg
