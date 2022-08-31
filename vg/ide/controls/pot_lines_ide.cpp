#include "plot_lines.h"
#include "easing.h"
#include "user_control_imgui.h"
namespace vg{
  void plot_lines::ex_init_fun(){
reg_property_handle(&_pt, &_pt._v_count,[this](){
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
  }
}