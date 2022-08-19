#include "circle.h"
#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
//#include "basic3d.h"


namespace vg
{
	circle::circle()
	{
		_pt._bkclr = { 1., 1., 0., 1. };
		#if defined(WITHIN_IDE)
		reg_property_handle(&_pt, 2, [this](void* memb_adress)
		{
			ImGui::SliderInt("Number of segments",&_pt._numb_segs,3,60);
		});
		#endif
	}
	void circle::draw()
	{
		auto ab_pos=absolute_coordinate_of_base_pos();
		ImVec2 abpos(ab_pos.x,ab_pos.y);
		ImVec2 winpos = ImGui::GetWindowPos();
		ImVec2 pos0 = { abpos.x + winpos.x, abpos.y + winpos.y };
		ImVec4 bkcl(_pt._bkclr.x, _pt._bkclr.y, _pt._bkclr.z, _pt._bkclr.w);
		ImU32 col = ImGui::ColorConvertFloat4ToU32(bkcl);
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		int numb_segs = _pt._numb_segs;
		if (numb_segs<20)
		{
			numb_segs = 20;
		}
		if (_pt._solid)
		{
			draw_list->AddCircleFilled(pos0, _pt._radius, col, numb_segs);
		}
		else
		{
			draw_list->AddCircle(pos0, _pt._radius, col, numb_segs, _pt._thickness_utn);
		}

	}

}