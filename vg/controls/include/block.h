#pragma once
#include "control_fac.h"
namespace vg
{
	class AFG_EXPORT block :
		public control_base
	{
		DEF_STRUCT_WITH_INIT(intl_pt,_pt,
			(float, _round_utn, {0.f}),
			(bool, _with_border, {false}),
			(float, _border_width_uhd, {1.f}),
			(vec4, _bkclr),
			(vec4, _bdclr))

	public:

		void set_background_color(float r, float g, float b, float a)
		{
			_pt._bkclr.x = r;
			_pt._bkclr.y = g;
			_pt._bkclr.z = b;
			_pt._bkclr.w = a;
		}
		void set_border_color(float r, float g, float b, float a)
		{
			_pt._bdclr.x = r;
			_pt._bdclr.y = g;
			_pt._bdclr.z = b;
			_pt._bdclr.w = a;
		}
		void draw();
	};
	REG_CTL_2_FAC(block);
}
