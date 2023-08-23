#include "image_file.h"
#include "SOIL.h"
#include "common_functions.h"
#include "user_control_imgui.h"
/*
x'=(x-a)cos��+(y-b)sin��+a
y'=-(x-a)sin��+(y-b)cos��+b
*/

namespace vg {

bool image_file::load_image_file(char *image_path) {
  if (_texture_id != 0) {
    glDeleteTextures(1, &_texture_id);
    _texture_id = 0;
  }
  GLubyte *imgdata = NULL;
  imgdata = SOIL_load_image(image_path, &_width, &_height, &_channels,
                            SOIL_LOAD_RGBA);
  if (imgdata == NULL) {
    printf("fail to load image from:%s\n", image_path);
    return false;
  }
  glGenTextures(1, &_texture_id);
  glBindTexture(GL_TEXTURE_2D, _texture_id);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  // Step3 �趨filter����
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, imgdata);
  SOIL_free_image_data(imgdata);
  return true;
}
void image_file::draw() {
  if (_texture_id == 0) {
    return;
  }
  float sizew = size().x;
  float sizeh = size().y;
  ImVec2 abpos = to_imvec2(absolute_coordinate_of_base_pos());
  ImVec2 winpos = ImGui::GetWindowPos();
  ImVec2 pos0 = {abpos.x + winpos.x, abpos.y + winpos.y};
  ImVec2 pos1 = {pos0.x, pos0.y + sizeh};
  ImVec2 pos2 = {pos0.x + sizew, pos0.y + sizeh};
  ImVec2 pos3 = {pos0.x + sizew, pos0.y};

  ImVec2 uv0(1.f, 0.f);
  ImVec2 uv1(1.f, 1.f);
  ImVec2 uv2(0.f, 1.f);
  ImVec2 uv3(0.f, 0.f);
  ImGui::ImageQuad((ImTextureID)_texture_id, pos0, pos1, pos2, pos3, uv0, uv1,
                   uv2, uv3);
  // ctl_base::draw();

}
} // namespace vg