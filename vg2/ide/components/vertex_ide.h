#include "vertex.h"

namespace vg{

 struct vertex_ide:public vertex{
     using sd_vertex_ide = std::shared_ptr<vertex_ide>;
     static sd_vertex_ide new_instance();
     static void edit(vertex_ide& vu);
 };

}