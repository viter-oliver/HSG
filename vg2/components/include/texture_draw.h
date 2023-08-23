#include "texture_ex.h"
namespace vg {
void draw_quad(texture_key &id, vec2 &pos0, vec2 &pos1, vec2 &pos2,
               vec2 &pos3, vec4 &&tint_clr = {1, 1, 1, 1},
               vec4 &&bord_clr = {0, 0, 0, 0});
void draw_rect(texture_key &id, vec2 &pos, vec2 &rect_size,
               vec4 &&tint_clr = {1, 1, 1, 1}, vec4 &&bord_clr = {0, 0, 0, 0});

} // namespace vg