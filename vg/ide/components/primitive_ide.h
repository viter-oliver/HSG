#include "primitive.h"
namespace vg{
  namespace primitive{
    struct primitive_ide:primitive_unit{
      bool _sel=false;
    };
    using sd_primitive_ide=std::shared_ptr<primitive_ide>;
    
  }
}