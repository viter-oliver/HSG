#pragma once
#include "control_fac.h"
#include "shader.h"
#include "vertex_container.h"

namespace vg
{
     class AFG_EXPORT a_texture_node :
          public control_base
     {
          DEF_STRUCT_WITH_INIT( intl_pt, _pt,
			  (char, _attached_obj[FILE_NAME_LEN]),
			   (int, _attached_txt),
               (float, _trans_mat4x4[16]))
		  static shader::sd_shader_base _ptxt_node_sd;
		  vertex::sd_vertex_unit _ps_prm = { nullptr };
     public:
		 a_texture_node();

		  void set_trans_mat(float* pfvalue){
			  memcpy(_pt._trans_mat4x4, pfvalue, 16 * sizeof(float));
		  }
		  void set_prm_obj(std::string prm_name){
			  strcpy(_pt._attached_obj, prm_name.c_str());
		  }
		  void set_txt_obj(int txt_index){
			  _pt._attached_txt=txt_index;
		  }
          void transform( glm::mat4& model );

		  void link();
		  void draw();
		  DECLARE_EX_INT
     };
	 REG_CTL_2_FAC( a_texture_node )
}
