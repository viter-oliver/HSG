#include "plot_lines.h"
#include "easing.h"
namespace vg
{
	plot_lines::plot_lines()
	{ 
#if defined(WITHIN_IDE)
		reg_property_handle(&_pt, [this](void*){
			ImGui::SliderInt("count of value:", &_pt._v_count, 3, MAX_VALUE_COUNT, "%.0f");
			static const char* const func_name[EaseFuncsCount] =
			{
				"EaseLinear",
				"EaseInSine",
				"EaseOutSine",
				"EaseInOutSine",
				"EaseInQuad",
				"EaseOutQuad",
				"EaseInOutQuad",
				"EaseInCubic",
				"EaseOutCubic",
				"EaseInOutCubic",
				"EaseInQuart",
				"EaseOutQuart",
				"EaseInOutQuart",
				"EaseInQuint",
				"EaseOutQuint",
				"EaseInOutQuint",
				"EaseInExpo",
				"EaseOutExpo",
				"EaseInOutExpo",
				"EaseInCirc",
				"EaseOutCirc",
				"EaseInOutCirc",
				"EaseInBack",
				"EaseOutBack",
				"EaseInOutBack",
				"EaseInElastic",
				"EaseOutElastic",
				"EaseInOutElastic",
				"EaseInBounce",
				"EaseOutBounce",
				"EaseInOutBounce",
			};
			if (ImGui::Combo("algebra:", &_algebra, func_name, EaseFuncsCount))
			{
				if (_algebra == 0)
				{
					ImGui::SliderFloatN("values:", _values, _pt._v_count, _pt._min, _pt._max, "%.3f", 1.0f);
				}
				else
				{
					float deltax = _pt._max - _pt._min;
					float unitx = deltax / _pt._v_count;
					float xx = _pt._min;
					for (int ix = 0; ix < _pt._v_count;++ix)
					{
						_values[ix] = easingFun[_algebra](xx);
						xx += unitx;
					}
				}
			}
			
		});
#endif
	}
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
