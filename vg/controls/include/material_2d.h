#pragma once
#include "control_fac.h"
#include "material.h"
#include "primitive.h"
namespace vg
{
	class AFG_EXPORT material_2d :
		public control_base
	{

		DEF_STRUCT_WITH_INIT(intl_pt, _pt,
			(int, _draw_mode, {en_gl_triangles}),
			(char, _primitive_name[FILE_NAME_LEN]),
			(char, _material_name[FILE_NAME_LEN]))
		bool _matched{ false };
		primitive::sd_primitive_unit _ps_prm;
		shader::sd_material _ps_mtl;
	public:
		material_2d();
		~material_2d(){}
		DECLARE_EX_INT
		void link();
		void draw();
	};
	REG_CTL_2_FAC(material_2d);
}
