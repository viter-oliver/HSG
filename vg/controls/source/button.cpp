#include "button.hpp"
namespace vg
{
	button::button()
	{
		_pt._image_normal_txt = _pt._image_press_txt = _pt._image_select_txt = {0, 0};
	}
	void button::draw()
	{
		auto& tintClr=_pt._tint_clr;
        auto& bdClr=_pt._border_clr;
		vec2 draw_pos=absolute_coordinate_of_base_pos();
	
	}


}