#pragma once
#include "font.h"
namespace vg{
  struct font_ide:public font_unit{
    using sd_font_ide=std::shared_ptr<font_ide>;
    static sd_font_ide new_instance();
    static void edit(font_ide& fu);
  };
}
