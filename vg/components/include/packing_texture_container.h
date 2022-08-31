#pragma once
#ifdef WITHIN_IDE
#include "packing_texture_ide.h"
using sd_packing_texturedef=vg::packing_texture::sd_packing_texture_unit_ide;
#else
#include "packing_texture.h"
using sd_packing_texturedef=vg::packing_texture::sd_packing_texture_unit;
#endif
namespace vg{
  namespace packing_texture{
    using  v_sd_packing_texture = std::vector<sd_packing_texturedef>;
    extern v_sd_packing_texture _v_sd_packing_texture;
  }
}