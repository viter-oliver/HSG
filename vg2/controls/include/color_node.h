#pragma once
#include "control_fac.h"
#include "shader.h"
#include "vertex_container.h"
namespace vg
{
     class AFG_EXPORT color_node :
          public control_base
     {
      DEF_STRUCT_WITH_INIT( _pt,
			(char_50, _attached_obj),
			(float_16, _trans_mat4x4),
			(vec3, _ambient_clr),
			(vec3, _diffuse_clr),
			(vec3, _specular_clr))
		  static shader::sd_shader_base _pcolor_node_sd;
		  vertex::sd_vertex_unit _ps_prm = { nullptr };
     public:
		 color_node();
#define DEF_COLOR_SET(item)\
	      color_node&  set_##item(vec3 clr){\
			  _pt._##item##_clr=clr;\
			  return *this;\
		  }
		  DEF_COLOR_SET(ambient)
		  DEF_COLOR_SET(diffuse)
		  DEF_COLOR_SET(specular)
		  void set_trans_mat(float* pfvalue){
			  memcpy(_pt._trans_mat4x4.value, pfvalue, 16 * sizeof(float));
		  }
		  void set_prm_obj(std::string prm_name){
			  strcpy(_pt._attached_obj.value, prm_name.c_str());
		  }
          void transform( glm::mat4& model );
		  void link();
		  void draw();
		  DECLARE_EX_INT
     };
	 REG_CTL_2_FAC( color_node )
}
