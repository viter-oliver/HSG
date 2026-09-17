#pragma once
#include "control_fac.h"
namespace vg
{
	class AFG_EXPORT block :
		public control_base
	{
  DEF_STRUCT_WITH_INIT(_pt,(float, _round_utn, {0.f}),
			(bool, _with_border, {false}),
			(float, _border_width_uhd, {1.f}),
			(vec4, _background_clr),
			(vec4, _board_clr))

	public:
		block();
		void set_background_color(float r, float g, float b, float a)
		{
			_pt._background_clr.x() = r;
			_pt._background_clr.y() = g;
			_pt._background_clr.z() = b;
			_pt._background_clr.w() = a;
		}
		void set_border_color(float r, float g, float b, float a)
		{
			_pt._board_clr.x() = r;
			_pt._board_clr.y() = g;
			_pt._board_clr.z() = b;
			_pt._board_clr.w() = a;
		}
		void draw();
	};
	REG_CTL_2_FAC(block);
}
