#include "vg_texture_json.h"
#include "SOIL.h"
#include "dir_output.h"
#include <GL/gl3w.h>
#include <fstream>
#include <iostream>
namespace vg {
using json = nlohmann::json;
using namespace std;
using namespace texture;
extern string g_cureent_directory;
void res_unit_2_json(nlohmann::json &jTar, texture::sd_texture_unit pres_unit,
                     std::string &res_unit_name) {
  jTar["name"] = res_unit_name;
  jTar["separated"] = pres_unit->_separated;
  jTar["mipmap"] = pres_unit->_mipmap;
}
bool json_2_res_unit(texture::sd_texture_unit pres_unit,
                     std::string &res_unit_name, nlohmann::json &jsrc) {
  try {
    res_unit_name = jsrc["name"];
    string img_file_path = g_cureent_directory + texture_fold + res_unit_name;
    pres_unit = make_shared<texture_unit>();
    pres_unit->_separated = jsrc["separated"];
    pres_unit->_mipmap = jsrc["mipmap"];
    GLubyte *imgdata = NULL;
    int width, height, channels;
    imgdata = SOIL_load_image(img_file_path.c_str(), &width, &height, &channels,
                              SOIL_LOAD_RGBA);

    if (imgdata == NULL) {
      printf("Fail to load texture file:%s\n", img_file_path.c_str());
      return false;
    }
    pres_unit->_width=width;
    pres_unit->_height=height;

    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    pres_unit->txt_id = textureId;
    glBindTexture(GL_TEXTURE_2D, textureId);
    if (channels == SOIL_LOAD_RGBA) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    // Step2 设定wrap参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Step3 设定filter参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (pres_unit->_mipmap) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR); // 为MipMap设定filter方法
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, imgdata);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, imgdata);
    }

    SOIL_free_image_data(imgdata);

  } catch (json::exception &e) {
    std::cerr << e.what() << '\n';
    return false;
  }
  return true;
}
} // namespace vg
