#include "packing_texture.h"
namespace vg{
  namespace packing_texture{
   void draw_quad(packing_text_id &id,vec2& pos0,vec2& pos1,vec2& pos2,vec2& pos3,vec4& tint_clr,vec4 &bord_clr);
   void draw_rect(packing_text_id &id,vec2& pos,vec2& rect_size,vec4 &tint_clr,vec4& bord_clr);
  }
}