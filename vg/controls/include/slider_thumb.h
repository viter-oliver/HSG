#pragma once
#include "image.h"
namespace vg {
class AFG_EXPORT slider_thumb : public control_base {

  DEF_STRUCT_WITH_INIT(intl_pt, _img_pt, (float, _min_point, {0.f}),
                       (float, _max_point, {100.f}),
                       (float, _progress_value_uhd, {0.f}),
                       (float, _pos_minx, {0.f}), (float, _pos_miny, {0.f}),
                       (float, _size_minw, {20.f}), (float, _size_minh, {20.f}),
                       (float, _pos_maxx, {20.f}), (float, _pos_maxy, {20.f}),
                       (float, _size_maxw, {40.f}), (float, _size_maxh, {40.f}),
                       (vi2, _thumb_id_txt))

#if defined(WITHIN_IDE)
  bool _keep_min_scale{false};
  float _w2h_min{1.0};
  bool _keep_max_scale{false};
  float _w2h_max{1.0};
#endif
public:
  slider_thumb();
  image _thumb;
  void set_progress_value(float pg_value) {
    if (pg_value < _img_pt._min_point) {
      pg_value = _img_pt._min_point;
    } else if (pg_value > _img_pt._max_point) {
      pg_value = _img_pt._max_point;
    }
    _img_pt._progress_value_uhd = pg_value;
  }
  void calcu_thumb_pos();
  float get_progress_value() { return _img_pt._progress_value_uhd; }

  void draw();
};
REG_CTL_2_FAC(slider_thumb);
} // namespace vg
