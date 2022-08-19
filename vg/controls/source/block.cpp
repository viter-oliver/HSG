#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "block.h"

namespace vg
{
	block::block()
	{
		_pt._bkclr = { 1., 1., 1., 1. };
		_pt._bdclr = { 1., 1., 1., 1. };
	}
	void block::draw()
	{
		auto abpos = absolute_coordinate_of_base_pos();
		ImVec2 winpos = ImGui::GetWindowPos();
		ImVec2 pos0 = { abpos.x + winpos.x, abpos.y + winpos.y };
		ImVec2 pos1(pos0.x + size().x, pos0.y + size().y);
		ImVec4 bkcl(_pt._bkclr.x, _pt._bkclr.y, _pt._bkclr.z, _pt._bkclr.w);
		ImU32 col = ImGui::ColorConvertFloat4ToU32(bkcl);
		if (_pt._with_border)
		{
			ImVec4 bdcl(_pt._bdclr.x, _pt._bdclr.y, _pt._bdclr.z, _pt._bdclr.w);
			ImU32 bd_clr = ImGui::ColorConvertFloat4ToU32(bdcl);
			ImGui::RenderFrame(pos0, pos1, bd_clr,false,_pt._round_utn);
			pos0.x += _pt._border_width_uhd; pos0.y += _pt._border_width_uhd;
			pos1.x -= _pt._border_width_uhd; pos1.y -= _pt._border_width_uhd;
			ImGui::RenderFrame(pos0, pos1, col, false, _pt._round_utn);
		}
		else
		{
			ImGui::RenderFrame(pos0, pos1, col, false, _pt._round_utn);
		}
	}
}