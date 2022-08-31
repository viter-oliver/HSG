#include "packing_texture_json.h"
#include "SOIL.h"
#include "dir_output.h"
#include <GL/gl3w.h>
#include <fstream>
#include <iostream>
namespace vg {
using namespace std;
using json = nlohmann::json;
extern string g_current_directory;
void res_unit_2_json(json &jTar,
                     packing_texture::packing_texture_unit_ide &res_unit) {
  jTar["name"]=res_unit.name;
  jTar["texture_pack_file"] = res_unit.texture_pack_file;
  jTar["texture_data_file"] = res_unit.texture_data_file;
  jTar["separated"] = res_unit.separated;
}
bool json_2_res_unit(packing_texture::packing_texture_unit_ide &res_unit,
                     json &jsrc) {
  try {
    res_unit.separated = jsrc["separated"];
    res_unit.name=jsrc["name"];
    res_unit.texture_pack_file = jsrc["texture_pack_file"];
    res_unit.texture_data_file = jsrc["texture_data_file"];
    string path_texture_pack_file = g_current_directory + packing_texture_fold;
    string path_texture_data_file = path_texture_pack_file;
    path_texture_pack_file += res_unit.texture_pack_file;
    path_texture_data_file += res_unit.texture_data_file;
    
    { // initialize packing texture
      int width, height, channels;
      GLubyte *imgdata = SOIL_load_image(path_texture_pack_file.c_str(), &width,
                                         &height, &channels, SOIL_LOAD_RGBA);
      if (imgdata == NULL) {
        return false;
      }
      GLuint textureId = 0;
      glGenTextures(1, &textureId);
      glBindTexture(GL_TEXTURE_2D, textureId);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      // Step2 设定wrap参数
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      // Step3 设定filter参数
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // Step4 加载纹理
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, imgdata);
      // Step5 释放纹理图片资源
      SOIL_free_image_data(imgdata);
      res_unit.txt_id = textureId;
      res_unit._width = width;
      res_unit._height = height;
    }
    { // initialize vtexture coordinate
      auto &vtxt_cd = res_unit.vtexture_coordinate;
      ifstream ifs(path_texture_data_file);
      json jcd;
      ifs >> jcd;
      auto &jframes = jcd["frames"];
      auto isz = jframes.size();
      for (size_t id = 0; id < isz; ++id) {
        auto ntxt_cd = make_shared<packing_texture::sub_texture_coordinate_ide>();
        auto &jfm_u = jframes[id];
        auto &frame = jfm_u["frame"];
        ntxt_cd->filename = jfm_u["filename"];
        bool roatated = frame["rotated"];
        ntxt_cd->_x0 = frame["x"];
        ntxt_cd->_y0 = frame["y"];

        if (roatated) {
          ntxt_cd->_x1 = ntxt_cd->_x0 + frame["h"];
          ntxt_cd->_y1 = ntxt_cd->_y0 + frame["w"];
        } else {
          ntxt_cd->_x1 = ntxt_cd->_x0 + frame["w"];
          ntxt_cd->_y1 = ntxt_cd->_y0 + frame["h"];
        }
        vtxt_cd.emplace_back(ntxt_cd);
      }
    }
  } catch (json::exception &e) {
    std::cerr << e.what() << '\n';
    return false;
  }
  return true;
}

} // namespace vg