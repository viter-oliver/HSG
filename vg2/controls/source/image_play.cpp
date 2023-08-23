#include "image_play.h"
#include "packing_texture_draw.h"
#include "packing_texture_container.h"
#include "common_functions.h"
#include "user_control_imgui.h"
/*
x'=(x-a)cos��+(y-b)sin��+a
y'=-(x-a)sin��+(y-b)cos��+b
*/
namespace vg
{
	using namespace std;
	using namespace packing_texture;
	image_play::image_play(){}
	int image_play::get_frames_count() {
		if (_img_pt._attached_txt >= _v_sd_packing_texture.size()) return 0;
		return _v_sd_packing_texture[_img_pt._attached_txt]->vtexture_coordinate.size();
	}
	void image_play::draw()
	{
		if (_img_pt._attached_txt >= _v_sd_packing_texture.size()) return;
		//base::draw();
		vui2 pid = { _img_pt._attached_txt,_img_pt._frame_index };
		auto ab_base_pos = absolute_coordinate_of_base_pos();
		draw_rect(pid, ab_base_pos, size());

	}

	
}
