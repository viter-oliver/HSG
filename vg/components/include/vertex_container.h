#ifdef WITHIN_IDE
#include "vertex_ide.h"
using sd_vertex_base=vg::vertex::sd_vertex_ide;
#else
#include "vertex.h"
using sd_vertex_base=vg::vertex::sd_vertex_unit;
#endif
namespace vg{
  namespace vertex{
    using mp_vertex_base=std::map<std::string,sd_vertex_base>;
    extern mp_vertex_base _mp_vertex_base_units;
  }
}