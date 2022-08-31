#include "slider.h"
#include "user_control_imgui.h"
namespace vg{
  void slider::ex_init_fun(){
    reg_property_handle(&_slider_pt, _slider_pt._track_path_text, [this]() {
    if (_custom_mid_track.size() > 0) {
      ImGui::Text("track path file:%s", _slider_pt._track_path_text);
      ImGui::SameLine();
      if (ImGui::Button("Delink##custom path")) {
        _custom_track0.clear();
        _custom_track1.clear();
        _custom_mid_track.clear();
        _custom_trace_length = 0;
        _ps_track_file = nullptr;
        _slider_pt._track_path_text[0] = '\0';
      }
    } else {
      ImGui::InputText("custom path file", _slider_pt._track_path_text,
                       sizeof(_slider_pt._track_path_text));
      if (ImGui::Button("Link")) {
        read_point_position_file(_slider_pt._track_path_text);
      }
    }
  });
  }
}