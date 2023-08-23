#pragma once
#include "control_fac.h"
namespace vg {

class AFG_EXPORT image_file : public control_base {
  u32 _texture_id = {0};
  int _width, _height, _channels;

public:

  bool load_image_file(char *image_path);
  u32 get_texture_id() { return _texture_id; }
  void draw();
  DECLARE_EX_INT
  DECLARE_DRAW_SEL_ANCHOR
};

REG_CTL_2_FAC(image_file);
} // namespace vg