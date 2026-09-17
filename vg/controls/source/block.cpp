#include "user_control_imgui.h"
#include "imgui_internal.h"
#include "block.hpp"

namespace vg
{
	block::block()
	{
		_pt._background_clr = { 0.5, 0.5, 0., 1. };
		_pt._board_clr = { 1., 1., 1., 1. };
	}
	void block::draw()
	{
    auto base_draw_pos = get_draw_pos();
    ImVec2 pos0 = to_imvec2(base_draw_pos);
		ImVec2 pos1(pos0.x + size().x(), pos0.y + size().y());
		ImVec4 bkcl(_pt._background_clr.x(), _pt._background_clr.y(), _pt._background_clr.z(), _pt._background_clr.w());
		ImU32 col = ImGui::ColorConvertFloat4ToU32(bkcl);
		if (_pt._with_border)
		{
			ImVec4 bdcl(_pt._board_clr.x(), _pt._board_clr.y(), _pt._board_clr.z(), _pt._board_clr.w());
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