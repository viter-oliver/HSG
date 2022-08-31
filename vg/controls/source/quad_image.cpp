#include "quad_image.h"
#include "user_control_imgui.h"
#include "packing_texture_draw.h"

namespace vg {

quad_image::quad_image() {
  _img_pt._texture_index_txt = {0xffff, 0};
  _img_pt._imge_pos[1] = {0., 100.};
  _img_pt._imge_pos[2] = {100., 100.};
  _img_pt._imge_pos[3] = {100., 0.};

}
void quad_image::draw() {
  auto &img_txt_id = _img_pt._texture_index_txt;
  packing_texture::draw_quad(img_txt_id, base_pos(), _img_pt._imge_pos[0], _img_pt._imge_pos[1], _img_pt._imge_pos[2]);
}
} // namespace vg