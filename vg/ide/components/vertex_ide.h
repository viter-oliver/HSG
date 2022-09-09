#include "vertex.h"
#include "edit_interface.h"
namespace vg{
  namespace vertex{
    struct vertex_ide:public vertex_unit,edit_interface{
      bool _sel=false;
      void edit_item();
    };
    using sd_vertex_ide=std::shared_ptr<vertex_ide>;

  }
  using mp_sd_vertex_ide=std::map<std::string,vertex::sd_vertex_ide>;
  void create_item(mp_sd_vertex_ide& vertex_list);
}