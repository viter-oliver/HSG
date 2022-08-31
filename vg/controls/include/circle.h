#pragma once
#include "control_fac.h"
namespace vg
{
	class AFG_EXPORT circle :
		public control_base
	{


		DEF_STRUCT_WITH_INIT(intl_pt,_pt,
			(float, _radius, {1.f}),
			(float, _thickness_utn, { 1.f }),
			(int, _numb_segs, { 20 }),
			(bool, _solid, {false}),
			(vec4, _bkclr))

	public:
		circle();
		void draw();
		bool contain(vec2 &tar_pos){
			vec2 vnm=tar_pos-absolute_coordinate_of_base_pos();
			auto nml=vnm.norm();
			return nml<_pt._radius;
		}
		DECLARE_EX_INT
	};
	REG_CTL_2_FAC(circle);
}
