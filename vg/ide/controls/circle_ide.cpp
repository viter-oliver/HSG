#include "user_control_imgui.h"
#include "circle.h"
namespace vg {
	void circle::ex_init_fun() {
		reg_property_handle(&_pt, &_pt._numb_segs, [this]()
		{
			ImGui::SliderInt("Number of segments", &_pt._numb_segs, 3, 60);
		});
	}
}
