#include "user_control_imgui.h"
#include "imgui_internal.h"
#include "circle.hpp"
//#include "basic3d.h"


namespace vg
{
  circle::circle()
	{
		_pt._bkclr = { 1., 1., 0., 1. };
	}

	void circle::draw()
	{
    auto base_draw_pos = get_draw_pos();
    ImVec2 pos0 = to_imvec2(base_draw_pos);
		ImVec4 bkcl(_pt._bkclr.x(), _pt._bkclr.y(), _pt._bkclr.z(), _pt._bkclr.w());
		ImU32 col = ImGui::ColorConvertFloat4ToU32(bkcl);
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		int numb_segs = _pt._numb_segs;
		if (numb_segs<20)
		{
			numb_segs = 20;
		}
		if (_pt._solid)
		{
      draw_list->AddCircleFilled(pos0, _pt._radius_f1s, col,
                                                   numb_segs);
		}
		else
		{
      draw_list->AddCircle(pos0, _pt._radius_f1s, col, numb_segs,
                           _pt._thickness_utn);
		}

	}

}