#pragma once
#include <vector>
#include <string>
#include <memory>
#include "vg_type.h"
namespace vg{
  namespace packing_texture{
    struct sub_texture_coordinate
    {
      float _x0,_y0,_x1,_y1;
      float width(){return _x1-_x0;}
      float height(){return _y1-_y0;}
    };
    using sd_sub_texture_coordinate=std::shared_ptr<sub_texture_coordinate>;
    using vtxt_coor_unit=std::vector<sd_sub_texture_coordinate>;
    struct packing_texture_unit{
        u32 txt_id{-1};
        u32 _width,_height;
        bool separated={false};
        bool mipmap={false};
        vtxt_coor_unit vtexture_coordinate;
        virtual ~packing_texture_unit();
    };
  
    using  sd_packing_texture_unit = std::shared_ptr<packing_texture_unit>;
    using  v_sd_packing_texture = std::vector<sd_packing_texture_unit>;
    extern v_sd_packing_texture _v_sd_packing_texture;
    using  packing_text_id=vi2;
  }
}