#pragma once
#include "shared_properties.h"
#include "vertex.h"
//#include "shader.h"

namespace vg{
class shader_control : public shared_properties {
protected:
  DEF_STRUCT_WITH_INIT(_pt, 
  (int, draw_mode, {en_gl_triangles}),
  (vertex_key, input_data))
public:
  DECLARE_EX_INT
  void draw();
  void set_vertex_key(vertex_key &vtx_key) { 
    _pt.input_data = vtx_key;
  }
};
}