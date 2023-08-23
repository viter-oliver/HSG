#pragma once
#include "control_fac.h"
#include "material.h"
#include "vertex.h"
namespace vg
{
	class AFG_EXPORT material_2d :
		public control_base
	{

		DEF_STRUCT_WITH_INIT(_pt,
			(int, _draw_mode, {en_gl_triangles}),
			(char_50, _vertex_name),
			(char_50, _material_name))
		bool _matched{ false };
		vertex::sd_vertex_unit _ps_prm;
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
