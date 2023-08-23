#pragma once
#include "control_fac.h"
#include "material.h"
#include "vg_texture.h"
namespace vg
{
	class AFG_EXPORT model_3d :
		public control_base
	{
		DEF_STRUCT_WITH_INIT(_pt,
			(vec3,_scale_stn),
			(vec3, _rotation_srd),
			(vec3, _translation_shd),
			(char_50,_mesh_data_file))
		shader::sd_material _pmaterial;
		texture::sd_texture_unit _texture;
		u32 _vao;
		u32 _vbo;
		u32 _tri_cnt;
	public:
		model_3d();
		~model_3d();

		void draw();
#if defined(WITHIN_IDE)
		void load_mesh_data_2_vertices();
		void load_vertics_2_vbo();
#endif
	};

	REG_CTL_2_FAC(model_3d);
}