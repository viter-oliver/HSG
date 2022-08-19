#pragma once
#include "control_fac.h"
#include "material.h"
#include "primitive.h"
namespace vg
{
	class AFG_EXPORT cube_3d :
		public control_base
	{
		DEF_STRUCT_WITH_INIT(intl_pt, _pt, 
			(vec3, _translation_shd),
			(vec3, _scale_stn),
			(vec3, _rotation_srd))
		/** referencing the color material */
		shader::sd_material _pmaterial;
		/** referencing the cube primitive object */
		primitive::sd_primitive_unit _pcube_prim;
	public:
		cube_3d();
		~cube_3d();
		void draw();

	};
	REG_CTL_2_FAC(cube_3d);
}