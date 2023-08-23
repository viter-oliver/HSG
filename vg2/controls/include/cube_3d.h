#pragma once
#include "control_fac.h"
#include "material.h"
#include "vertex.h"
namespace vg
{
	class AFG_EXPORT cube_3d :
		public control_base
	{
		DEF_STRUCT_WITH_INIT(_pt, 
			(vec3, _translation_shd),
			(vec3, _scale_stn),
			(vec3, _rotation_srd))
		/** referencing the color material */
		shader::sd_material _pmaterial;
		/** referencing the cube vertex object */
		vertex::sd_vertex_unit _pcube_prim;
	public:
		cube_3d();
		~cube_3d();
		void draw();

	};
	REG_CTL_2_FAC(cube_3d);
}