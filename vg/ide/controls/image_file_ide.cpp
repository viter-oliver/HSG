#include "image_file.h"
#include "SOIL.h"
#include <commdlg.h>
#include "common_functions.h"
#include "user_control_imgui.h"
namespace vg {
void image_file::ex_init_fun() {
  reg_property_handle(&_img_pt, &_img_pt._angle_srd, [this]() {
    ImGui::SliderFloat("Rotate angle:",&_img_pt._angle_srd,-360.f,360.f);
    if (_texture_id != 0) {
      ImGui::Text("image size:%d,%d", _width, _height);
      if (ImGui::Button("Unloading image")) {
        glDeleteTextures(1, &_texture_id);
        _texture_id = 0;
      }
    } else {
      if (ImGui::Button("Loading image:")) {
        OPENFILENAME ofn = {sizeof(OPENFILENAME)};
        // ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = GetForegroundWindow();
        ofn.lpstrFilter =
            "image "
            "file:\0*.png;*.bmp;*.jpg;*.jpeg;*.gif;*.dds;*.tga;*.psd;*.hdr\0\0";
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST |
                    OFN_HIDEREADONLY | OFN_ENABLETEMPLATE | OFN_ENABLEHOOK;
        ofn.hInstance = GetModuleHandle(NULL);
        char strFileName[MAX_PATH] = {0};
        ofn.nFilterIndex = 1;
        ofn.lpstrFile = strFileName;
        ofn.nMaxFile = sizeof(strFileName);
        ofn.lpstrTitle = "Loading image...";
        if (GetOpenFileName(&ofn)) {
          load_image_file(strFileName);
        }
      }
    }
  });
}
} // namespace vg