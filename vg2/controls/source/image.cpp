#include "image.h"
#include "packing_texture_draw.h"

/*
x'=(x-a)cos��+(y-b)sin��+a
y'=-(x-a)sin��+(y-b)cos��+b
*/

namespace vg {
image::image() {
  _img_pt._tint_clr = {1.f, 1.f, 1.f, 1.f};
  _img_pt._texture_index_txt = {0, 0};
}
void image::draw() {
  auto rotated = abs(_img_pt._angle_srd) >= 0.1;
  auto &pid = _img_pt._texture_index_txt;
  auto &tintClr = _img_pt._tint_clr;
  auto &bdClr = _img_pt._border_clr;
  if (!rotated) {
    packing_texture::draw_rect(pid, start_point(), size(), tintClr, bdClr);

  } else {
    vec2 p1 = start_point();
    vec2 p2 = {p1.x + size().x, p1.y};
    vec2 p3 = p1 + size();
    vec2 p4 = {p1.x, p1.y + size().y};
    auto &center = _img_pt._rotate_center;
    auto angle = _img_pt._angle_srd;
    auto pr1 = p1.rotate_by(center, angle);
    auto pr2 = p2.rotate_by(center, angle);
    auto pr3 = p3.rotate_by(center, angle);
    auto pr4 = p4.rotate_by(center, angle);

    packing_texture::draw_quad(pid, pr1, pr2, pr3, pr4, tintClr, bdClr);
  }
}
} // namespace vg