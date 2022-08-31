#include "plot_lines.h"
#include "user_control_imgui.h"
namespace vg
{
	void plot_lines::draw()
	{
		//ctl_base::draw();
		auto abpos = absolute_coordinate_of_base_pos();
		//ImVec2 winpos = ImGui::GetWindowPos();
		ImGui::SetCursorPosX(abpos.x);
		ImGui::SetCursorPosY(abpos.y);
		ImGui::PlotLines("Lines", _values, _pt._v_count, 0, "avg 0.0", -1.0f, 1.0f, ImVec2(_in_p._sizew, _in_p._sizeh));

	}
}
