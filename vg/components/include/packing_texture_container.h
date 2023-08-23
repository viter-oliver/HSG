#pragma once
#ifdef WITHIN_IDE
#include "packing_texture_ide.h"
using packing_texture_def=vg::packing_texture::packing_texture_unit_ide;
#else
#include "packing_texture.h"
using packing_texture_def=vg::packing_texture::packing_texture_unit;
#endif
namespace vg{
  namespace packing_texture{
    using sd_packing_texturedef=std::shared_ptr<packing_texture_def>;
    using  v_sd_packing_texture = std::vector<sd_packing_texturedef>;
    extern v_sd_packing_texture _v_sd_packing_texture;
  }
}