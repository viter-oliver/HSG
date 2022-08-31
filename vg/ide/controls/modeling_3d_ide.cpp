#include "modeling_3d.h"
#include "user_control_imgui.h"
namespace vg {
  using namespace model;
void modeling_3d::ex_init_fun() {
  reg_property_handle(&_pty_page, _pty_page._model_name, [this]() {
    if (_pmodel) {
      ImGui::Text("Model name:%s", _pty_page._model_name);
      ImGui::SameLine();
      if (ImGui::Button("Delink##model_name")) {
        _pmodel = nullptr;
      }
    } else {
      ImGui::InputText("Model name:", _pty_page._model_name, FILE_NAME_LEN);
      if (ImGui::Button("import")) {
        auto imodel = model::g_mmodel_list.find(_pty_page._model_name);
        if (imodel != model::g_mmodel_list.end()) {
          _pmodel = imodel->second;
          bounding_box mbox;
          auto &mdlist = *_pmodel;
          for (auto &amesh : mdlist) {
            auto &box = amesh._box;
            if (box._xmin < mbox._xmin) {
              mbox._xmin = box._xmin;
            }
            if (box._xmax > mbox._xmax) {
              mbox._xmax = box._xmax;
            }
            if (box._ymin < mbox._ymin) {
              mbox._ymin = box._ymin;
            }
            if (box._ymax > mbox._ymax) {
              mbox._ymax = box._ymax;
            }
            if (box._zmin < mbox._zmin) {
              mbox._zmin = box._zmin;
            }
            if (box._zmax > mbox._zmax) {
              mbox._zmax = box._zmax;
            }
          }
          auto zthickness = mbox._zmax - mbox._zmin;
          auto yheight = mbox._ymax - mbox._zmin;

          _pty_page._cam._position = {(mbox._xmax + mbox._xmin) * 0.5f,
                                      (mbox._ymax + mbox._ymin) * 0.5,
                                      mbox._zmin - zthickness};
          _pty_page._cam._direction = _pty_page._cam._position;
          _pty_page._cam._direction.z = mbox._zmin;
          _pty_page._cam._up = {0.f, 1.f, 0.f};
          _pty_page._pj._near = mbox._zmin;
          _pty_page._pj._far = mbox._zmax;
#define PI 3.1415926545
          _pty_page._pj._fovy =
              2 * atanf(yheight * 0.5 / zthickness) * 180 / PI;
          _pty_page._light_posx = _pty_page._cam._position.x;
          _pty_page._light_posy = _pty_page._cam._position.y;
          _pty_page._light_posz = _pty_page._cam._position.z;
        }
      }
    }
  });
  reg_property_handle(&_pty_page, &_pty_page._draw_mode, [this]() {
    ImGui::Combo("Draw mode:", &_pty_page._draw_mode, draw_mode, en_gl_count);
  });
}
} // namespace vg