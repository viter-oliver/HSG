#include "polygon_image.h"
#include "user_control_imgui.h"
namespace vg{
  void polygon_image::ex_init_fun(){
    reg_property_handle(&_img_pt, _img_pt._track_file, [this](){
			if (_track0.size()<2)
			{
				ImGui::InputText("Track path file", _img_pt._track_file,name_len);
				if (ImGui::Button("Link"))
				{
					read_track_file(_img_pt._track_file);
				}
			}
			else
			{
				ImGui::Text("Track path file:%s", _img_pt._track_file);
				ImGui::SameLine();
				if (ImGui::Button("Delink##track"))
				{
					_track0.clear();
					_track1.clear();
					_ps_track_file = nullptr;
					_img_pt._track_file[0] = '\0';
				}
			}
		});
  }
  void polygon_image::draw_sel_anchor(){
	  auto pt_cnt = _track0.size();
	  if (!_ps_track_file|| pt_cnt < 2) {
		  return;
	  }
	  auto abpos = absolute_coordinate_of_base_pos();
	  auto winpos = ImGui::GetWindowPos();
	  ImU32 col = ImGui::GetColorU32(ImGuiCol_HeaderActive);
	  ImVec2 editunit(edit_unit_len, edit_unit_len);
	  for (int ix = 0; ix < pt_cnt; ++ix) {
		  auto& pt_anchor0 = to_imvec2(_track0[ix]);
		  auto pt_l_t0 = pt_anchor0 - editunit;
		  auto pt_r_b0 = pt_anchor0 + editunit;
		  ImGui::RenderFrame(pt_l_t0, pt_r_b0, col);
		  auto& pt_anchor1 = to_imvec2(_track1[ix]);
		  auto pt_l_t1 = pt_anchor1 - editunit;
		  auto pt_r_b1 = pt_anchor1 + editunit;
		  ImGui::RenderFrame(pt_l_t1, pt_r_b1, col);
	  }
  }
}