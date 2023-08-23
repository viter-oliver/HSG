#ifdef WITHIN_IDE
#include "vertex_ide.h"
using vertex_def=vg::vertex::vertex_ide;
#else
#include "vertex.h"
using vertex_def=vg::vertex::vertex_unit;
#endif
namespace vg{
  namespace vertex{
    using sd_vertex_def=std::shared_ptr<vertex_def>;
    using mp_vertex_base=std::map<std::string,sd_vertex_def>;
    extern mp_vertex_base _mp_vertex_base_units;
  }
}