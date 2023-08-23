#include "image_play.h"
#include "user_control_imgui.h"
namespace vg{
  void image_play::ex_init_fun(){
    reg_property_handle(&_img_pt, &_img_pt._frame_index, [this](){
			int frame_cnt = get_frames_count();
			ImGui::SliderInt("frame index:", &_img_pt._frame_index, 0, frame_cnt-1);
		});
  }
}