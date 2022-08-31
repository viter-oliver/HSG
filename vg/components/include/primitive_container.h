#ifdef WITHIN_IDE
#include "primitive_ide.h"
using sd_primitivbe_base=vg::primitive::sd_primitive_ide;
#else
#include "primitive.h"
using sd_primitivbe_base=vg::primitive::sd_primitive_unit;
#endif
namespace vg{
  namespace primitive{
    using mp_primitive_base=std::map<std::string,sd_primitivbe_base>;
    extern mp_primitive_base _mp_primitive_units;
  }
}