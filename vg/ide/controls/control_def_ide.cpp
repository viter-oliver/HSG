#include "control_def_ide.h"
#include "common_functions.h"
#include "packing_texture_container.h"
#include "packing_texture_draw.h"
#include "user_control_imgui.h"
#include <regex>
namespace vg {
using namespace std;
extern shared_ptr<control_def_ide> produce_control(string cname);
using edit_field_value_fun = function<void(void *)>;
using dic_special_type_edit = map<string, edit_field_value_fun>;
static dic_special_type_edit _mcustom_type_property_handles_container;
const float screenw = 1920, screenh = 720;
void reg_property_handle(string tpname, edit_field_value_fun ph) {
  _mcustom_type_property_handles_container[tpname] = ph;
}
const int init_common_type_property_handles() {
  reg_property_handle("camera", [](void *membadr) {
    camera *pcamera = reinterpret_cast<camera *>(membadr);
    ImGui::Text("Camera");
    ImGui::SliderFloat("Position x", &pcamera->_position.x, -screenw, screenw);
    ImGui::SliderFloat("Position y", &pcamera->_position.y, -screenh, screenh);
    ImGui::SliderFloat("Position z", &pcamera->_position.z, -screenh, screenh);

    ImGui::SliderFloat("Direction x", &pcamera->_direction.x, -10, 10);
    ImGui::SliderFloat("Direction y", &pcamera->_direction.y, -10, 10);
    ImGui::SliderFloat("Direction z", &pcamera->_direction.z, -10, 10);

    ImGui::SliderFloat("Up x", (float *)&pcamera->_up.x, -10, 10);
    ImGui::SliderFloat("Up y", (float *)&pcamera->_up.y, -10, 10);
    ImGui::SliderFloat("Up z", (float *)&pcamera->_up.z, -10, 10);
  });
  reg_property_handle("directional_light", [](void *membadr) {
    directional_light *pdr_lt = reinterpret_cast<directional_light *>(membadr);
    ImGui::Text("Directional light");
    ImGui::ColorEdit3("Light color", (float *)&pdr_lt->_color,
                      ImGuiColorEditFlags_RGB);
    ImGui::SliderFloat("Light direction x", &pdr_lt->_direction.x, -10, 10);
    ImGui::SliderFloat("Light direction y", &pdr_lt->_direction.y, -10, 10);
    ImGui::SliderFloat("Light direction z", &pdr_lt->_direction.z, -10, 10);
  });
  reg_property_handle("point_light", [](void *membadr) {
    point_light *ppt_lt = reinterpret_cast<point_light *>(membadr);
    ImGui::Text("Point light");
    ImGui::ColorEdit3("Point light color", (float *)&ppt_lt->_color,
                      ImGuiColorEditFlags_RGB);
    ImGui::SliderFloat("Point light position x", &ppt_lt->_position.x, -screenw,
                       screenw);
    ImGui::SliderFloat("Point light position y", &ppt_lt->_position.y, -screenh,
                       screenh);
    ImGui::SliderFloat("Point light position z", &ppt_lt->_position.z, -screenh,
                       screenh);
  });
  reg_property_handle("transformation", [](void *membadr) {
    transformation *ptrans = reinterpret_cast<transformation *>(membadr);
    ImGui::Text("Transformation");
    ImGui::Checkbox("SameScale:", &ptrans->_same_scale);
    if (ptrans->_same_scale) {
      if (ImGui::SliderFloat("Scale", &ptrans->_scale.x, -10.f, 10.f, "%.4f",
                             0.01f)) {
        ptrans->_scale.y = ptrans->_scale.x;
        ptrans->_scale.z = ptrans->_scale.x;
      }
      // ImGui::SliderFloat("Scale", &ptrans->_scale.y, -10.f, 10.f);
      // ImGui::SliderFloat("Scale", &ptrans->_scale.z, -10.f, 10.f);
    } else {
      ImGui::SliderFloat("Scale x", &ptrans->_scale.x, -10.f, 10.f, "%.4f",
                         0.01f);
      ImGui::SliderFloat("Scale y", &ptrans->_scale.y, -10.f, 10.f, "%.4f",
                         0.01f);
      ImGui::SliderFloat("Scale z", &ptrans->_scale.z, -10.f, 10.f, "%.4f",
                         0.01f);
    }
    ImGui::SliderFloat("Rotation x", &ptrans->_rotation.x, -180.f, 180.f);
    ImGui::SliderFloat("Rotation y", &ptrans->_rotation.y, -180.f, 180.f);
    ImGui::SliderFloat("Rotation z", &ptrans->_rotation.z, -180.f, 180.f);
    ImGui::SliderFloat("Translation x", &ptrans->_translation.x, -screenw,
                       screenw, "%.3f", 0.1f);
    ImGui::SliderFloat("Translation y", &ptrans->_translation.y, -screenw,
                       screenw, "%.3f", 0.1f);
    ImGui::SliderFloat("Translation z", &ptrans->_translation.z, -screenw,
                       screenw, "%.3f", 0.1f);
  });
  reg_property_handle("projection", [](void *membadr) {
    projection *pproj = reinterpret_cast<projection *>(membadr);
    ImGui::Text("Projection");
    ImGui::SliderFloat("Fovy", (float *)&pproj->_fovy, 0.f, 180.f);
    // ImGui::SliderFloat("Aspect", (float*)&pproj->_aspect, 0.f, 10);
    ImGui::SliderFloat("Near", (float *)&pproj->_near, 0.f, 180.f);
    ImGui::SliderFloat("Far", (float *)&pproj->_far, 0.f, 180.f);
  });
  return 1;
}
const int dumy_init = init_common_type_property_handles();
static map<string, value_range> s_rg_tips;
const int init_base_value_ranges() {
  s_rg_tips["shd"] = value_range(-100.f, 100.f);
  s_rg_tips["stn"] = value_range(-10.f, 10.f);
  s_rg_tips["srd"] = value_range(-360.f, 360.f);
  s_rg_tips["uhd"] = value_range(0.f, 100.f);
  s_rg_tips["utn"] = value_range(0.f, 10.f);
  s_rg_tips["urd"] = value_range(0.f, 360.f);
  s_rg_tips["nml"] = value_range(0.f, 1.f);
  return 1;
}
static const int s_triger = init_base_value_ranges();
void ShowHelpMarker(const char *desc) {
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}
bool edit_file_ele(field_ele &fele) {
  auto mtype = fele._type;
  auto mname = fele._name;
  auto mtpsz = fele._tpsz;
  char *memb_address = fele._address;
  int array_cnt = fele._count;
  string rg = mname.substr(mname.length() - 3, 3);
  auto &irg = s_rg_tips.find(rg);
  value_range _vrange(-screenw, screenw);
  if (irg != s_rg_tips.end()) {
    _vrange = irg->second;
  }

  string float_format = "%.3f";
  if (regex_search(mname, regex("_hac"))) {
    float_format = "%.6f";
  }

  function<bool(string &, void *)> f_draw_index_prop;

  if (mtype == "char") {
    if (array_cnt > 0) {
      ImGui::InputText(mname.c_str(), (char *)memb_address, array_cnt);
    } else {
      ImGui::SliderInt(mname.c_str(), (int *)memb_address, 0, 255);
    }
  } else if (mtype == "int") {
    f_draw_index_prop = [&](string &str_show, void *maddress) {
      return ImGui::SliderInt(str_show.c_str(), (int *)maddress,
                              _vrange._min._i, _vrange._max._i);
    };
  } else if (mtype == "float" || mtype == "double") {
    f_draw_index_prop = [&](string &str_show, void *maddress) {
      return ImGui::SliderFloat(str_show.c_str(), (float *)maddress,
                                _vrange._min._f, _vrange._max._f,
                                float_format.c_str());
    };
  } else if (mtype == "vui2") {
    if (rg == "txt") // atexture
    {
      auto f_draw_index_prop = [&](string &str_show, void *maddress) {
        using namespace packing_texture;
        constexpr const float imge_edit_view_height_max = 500.f;
        constexpr const float imge_edit_view_width_max =
            imge_edit_view_height_max * 0.618f;
        vui2 *ptxt_idx = (vui2 *)maddress;
        vec2 img_sz;
        int igsize = _v_sd_packing_texture.size();
        if (ptxt_idx->x >= igsize || ptxt_idx->x < 0) {
          ImGui::Text("Invalid texture!");
          img_sz = {88, 81};
        } else {
          auto &ptxt_unit = _v_sd_packing_texture[ptxt_idx->x];
          auto ptxt_unit_ide =
              static_pointer_cast<sd_packing_texture_unit_ide>(ptxt_unit);
          img_sz = {(float)(*ptxt_unit_ide)->_width,
                    (float)(*ptxt_unit_ide)->_height};
          ImGui::Text("Texture:%s", (*ptxt_unit_ide)->name.c_str());
          auto &pvsub_unit = (*ptxt_unit_ide)->vtexture_coordinate;
          if (ptxt_idx->y < pvsub_unit.size()) {
            auto &psub_unit = pvsub_unit[ptxt_idx->y];
            auto psub_unit_ide =
                static_pointer_cast<sd_sub_texture_coordinate_ide>(psub_unit);
            img_sz = {(*psub_unit_ide)->width(), (*psub_unit_ide)->height()};
            ImGui::SameLine();
            ImGui::Text("->%s", (*psub_unit_ide)->filename.c_str());
          }
        }
        vec2 invalid_pos = {-1, 0};
        vec4 tincol = {1, 1, 1, 1}, bordcol = {0, 0, 0, 0};
        bool is_fat = img_sz.x > img_sz.y;
        if (is_fat && img_sz.x > imge_edit_view_width_max) {
          auto scale = img_sz.y / img_sz.x;
          img_sz.x = imge_edit_view_width_max;
          img_sz.y = imge_edit_view_width_max * scale;
        } else if (!is_fat && img_sz.y > imge_edit_view_height_max) {
          auto scale = img_sz.x / img_sz.y;
          img_sz.y = imge_edit_view_height_max;
          img_sz.x = imge_edit_view_height_max * scale;
        }

        packing_texture::draw_rect(*ptxt_idx, invalid_pos, img_sz, tincol,
                                   bordcol);
      };
    } else {
      f_draw_index_prop = [&](string &str_show, void *maddress) {
        return ImGui::SliderInt2(str_show.c_str(), (int *)maddress,
                                 _vrange._min._u, _vrange._max._u);
      };
    }
  } else if (mtype == "vi3") {
    f_draw_index_prop = [&](string &str_show, void *maddress) {
      return ImGui::SliderInt3(str_show.c_str(), (int *)maddress,
                               _vrange._min._i, _vrange._max._i);
    };
  } else if (mtype == "vi4") {
    f_draw_index_prop = [&](string &str_show, void *maddress) {
      return ImGui::SliderInt4(str_show.c_str(), (int *)maddress,
                               _vrange._min._i, _vrange._max._i);
    };
  } else if (mtype == "vec2") {
    f_draw_index_prop = [&](string &str_show, void *maddress) {
      return ImGui::SliderFloat2(str_show.c_str(), (float *)maddress,
                                 _vrange._min._f, _vrange._max._f,
                                 float_format.c_str());
    };
  } else if (mtype == "vec3") {
    f_draw_index_prop = [&](string &str_show, void *maddress) {
      if (rg == "clr") {
        return ImGui::ColorEdit3(str_show.c_str(), (float *)maddress,
                                 ImGuiColorEditFlags_RGB);
      } else {
        return ImGui::SliderFloat3(str_show.c_str(), (float *)maddress,
                                   _vrange._min._f, _vrange._max._f,
                                   float_format.c_str());
      }
    };
  } else if (mtype == "vec4") {
    f_draw_index_prop = [&](string &str_show, void *maddress) {
      if (rg == "clr") {
        return ImGui::ColorEdit4(str_show.c_str(), (float *)maddress,
                                 ImGuiColorEditFlags_RGB);
      } else {
        return ImGui::SliderFloat4(str_show.c_str(), (float *)maddress,
                                   _vrange._min._f, _vrange._max._f,
                                   float_format.c_str());
      }
    };
  } else if (mtype == "bool") {
    f_draw_index_prop = [&](string &str_show, void *maddress) {
      return ImGui::Checkbox(str_show.c_str(), (bool *)maddress);
    };
  } else {
    printf("unknown member type!:%s\n", mtype.c_str());
    return false;
  }
  if (f_draw_index_prop) {
    if (array_cnt > 0) {
      for (int ix = 0; ix < array_cnt; ++ix) {
        char str_index[50] = {0};
        sprintf(str_index, "[%d]", ix);
        string mname_width_index = mname + str_index;
        void *memb_index_address = (char *)memb_address + ix * mtpsz;
        f_draw_index_prop(mname_width_index, memb_index_address);
      }
    } else {
      f_draw_index_prop(mname, memb_address);
    }
  }
  return true;
}
void control_def_ide::ex_init_fun() {
  reg_property_handle(&_in_p, &_in_p._size, [this]() {
    ImGui::SliderFloat("width:", &_in_p._size.x, 1920.f, 1920.f);
    ImGui::SliderFloat("height:", &_in_p._size.y, 720.f, 720.f);
  });
  _after_draw_handle = [&]() {
    if (_selected) {
      draw_sel_anchor();
    }
  };
}
void control_def_ide::provide_value(
    dragging_value_to_tar::ptr_dragging_value pdragging_value) {}
void control_def_ide::draw_propertys() {
  int pgidx = 0;
  string type_name = typeid(*this).name();
  string type_show = type_name.substr(sizeof("class vg::"));
  ImGui::Text("Type name:%s", type_show.c_str());
  for (auto &prop_ele : _vprop_eles) {
    auto &prop_page = prop_ele->_pro_page;
    int idx = 0;
    for (auto &memb : prop_page) {

      auto &imemb_tp_handl =
          _mcustom_type_property_handles_container.find(memb->_type);
      if (imemb_tp_handl != _mcustom_type_property_handles_container.end()) {
        imemb_tp_handl->second(memb->_address);
      } else {
        auto &imemb_var_handle = _unique_property_handles.find(
            st_member_key(prop_ele->_pro_address, memb->_address));
        if (imemb_var_handle != _unique_property_handles.end()) {
          imemb_var_handle->second();
        } else {
          if (!edit_file_ele(*memb)) {
            continue;
          }
        }
      }
      ImGui::SameLine();
      ImGui::PushID(idx);
      if (IconButton("ailias") && ImGui::IsMouseDoubleClicked(0)) {
        // TODO: open create alias window
      }
      if (IconButton("drag")) {
        // TODO:dragging_value_to_tar::reg_a_value();
      }
      dragging_value_to_tar::try_dragging();
      ImGui::PopID();
      idx++;
    }
    ImGui::Separator();
    ImGui::Spacing();
    pgidx++;
  }
}

void control_def_ide::draw_outline() {
  auto winpos = ImGui::GetWindowPos();
  auto abpos = absolute_coordinate_of_base_pos();
  ImVec2 pos[4];
  string cur_cname = typeid(*this).name();
  cur_cname = cur_cname.substr(sizeof("class autofuture::"));
  ImVec2 asz = to_imvec2(size());

  pos[0] = winpos + ImVec2(abpos.x, abpos.y);
  pos[1] = pos[0] + ImVec2(asz.x, 0.f);
  pos[2] = pos[0] + asz;
  pos[3] = pos[0] + ImVec2(0.f, asz.y);
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  float alpha = 1.f;
  if (!be_seen()) {
    alpha = 0.4f;
  }

  ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(0.7, 0.7, 0.7, alpha));
  float thickness = 0.8f;
  if (_selected) {
    col = ImGui::ColorConvertFloat4ToU32(ImVec4(1, 0, 0, alpha));
  }

  draw_list->AddPolyline(pos, 4, col, true, thickness);
  for (auto &bc : _vchilds) {
    bc->draw_outline();
  }
}
void control_def_ide::draw_sel_anchor() {}
void control_def_ide::draw_corners() {
  ImU32 col = ImGui::GetColorU32(ImGuiCol_HeaderActive);
  auto abpos = absolute_coordinate_of_base_pos();
  ImVec2 winpos = ImGui::GetWindowPos();
  ImVec2 pos1 = winpos + ImVec2(abpos.x, abpos.y);
  ImVec2 pos2 = {pos1.x, pos1.y + size().y};
  ImVec2 pos3 = {pos1.x + size().x, pos1.y + size().y};
  ImVec2 pos4 = {pos1.x + size().x, pos1.y};

  ImVec2 editunit(edit_unit_len, edit_unit_len);
  ImVec2 pos1a = pos1 - editunit;
  ImVec2 pos1b = pos1 + editunit;
  ImGui::RenderFrame(pos1a, pos1b, col);
  ImVec2 pos2a = pos2 - editunit;
  ImVec2 pos2b = pos2 + editunit;
  ImGui::RenderFrame(pos2a, pos2b, col);
  ImVec2 pos3a = pos3 - editunit;
  ImVec2 pos3b = pos3 + editunit;
  ImGui::RenderFrame(pos3a, pos3b, col);
  ImVec2 pos4a = pos4 - editunit;
  ImVec2 pos4b = pos4 + editunit;
  ImGui::RenderFrame(pos4a, pos4b, col);
}
using json = nlohmann::json;

void control_def_ide::init_property_from_json(nlohmann::json &jvalue) {
  for (auto &prop_ele : _vprop_eles) {
    auto &prop_page = prop_ele->_pro_page;
    for (auto &memb : prop_page) {
      auto mtype = memb->_type;
      auto mname = memb->_name;
      auto mtpsz = memb->_tpsz;
      char *memb_address = memb->_address;
      int array_cnt = memb->_count;
      function<void(void *, json &)> f_assingn_json_to_memb;
      if (mtype == "int") {
        f_assingn_json_to_memb = [&](void *membaddr, json &vele) {
          *(int *)membaddr = vele;
        };
      } else if (mtype == "float" || mtype == "double") {
        f_assingn_json_to_memb = [&](void *membaddr, json &vele) {
          *(float *)membaddr = vele;
        };
      } else if (mtype == "vi2") {
        f_assingn_json_to_memb = [&](void *membaddr, json &vele) {
          *(int *)membaddr = vele["x"];
          *((int *)membaddr + 1) = vele["y"];
        };
      } else if (mtype == "vi3") {
        f_assingn_json_to_memb = [&](void *membaddr, json &vele) {
          *(int *)membaddr = vele["x"];
          *((int *)membaddr + 1) = vele["y"];
          *((int *)membaddr + 2) = vele["z"];
        };
      } else if (mtype == "vi4") {
        f_assingn_json_to_memb = [&](void *membaddr, json &vele) {
          *(int *)membaddr = vele["x"];
          *((int *)membaddr + 1) = vele["y"];
          *((int *)membaddr + 2) = vele["z"];
          *((int *)membaddr + 3) = vele["w"];
        };
      } else if (mtype == "vec2") {
        f_assingn_json_to_memb = [&](void *membaddr, json &vele) {
          *(float *)membaddr = vele["x"];
          *((float *)membaddr + 1) = vele["y"];
        };
      } else if (mtype == "vec3") {
        f_assingn_json_to_memb = [&](void *membaddr, json &vele) {
          *(float *)membaddr = vele["x"];
          *((float *)membaddr + 1) = vele["y"];
          *((float *)membaddr + 2) = vele["z"];
        };
      } else if (mtype == "vec4") {
        f_assingn_json_to_memb = [&](void *membaddr, json &vele) {
          *(float *)membaddr = vele["x"];
          *((float *)membaddr + 1) = vele["y"];
          *((float *)membaddr + 2) = vele["z"];
          *((float *)membaddr + 3) = vele["w"];
        };
      } else if (mtype == "bool") {
        f_assingn_json_to_memb = [&](void *membaddr, json &vele) {
          *(bool *)membaddr = vele;
        };
      } else {
        f_assingn_json_to_memb = [&](void *membaddr, json &vele) {
          string out_bin;
          convert_string_to_binary(vele.get<string>(), out_bin);
          memcpy(membaddr, &out_bin[0], out_bin.size());
        };
      }
      if (array_cnt > 1) {
        json &jtemp = jvalue[mname];
        if (jtemp.is_null()) {
          continue;
        }
        if (mtype == "char") {
          auto &vbytes = jtemp.get<string>(); // must be string
          strcpy((char *)memb_address, vbytes.c_str());
        } else {
          json &marray = jtemp;

          for (int ix = 0; ix < array_cnt; ++ix) {
            void *memb_index_address = (char *)memb_address + ix * mtpsz;
            f_assingn_json_to_memb(memb_index_address, marray[ix]);
          }
        }
      } else {
        json &jtemp = jvalue[mname];
        if (jtemp.is_null()) {
          continue;
        }
        f_assingn_json_to_memb(memb_address, jtemp);
      }
    }
  }
  json childs = jvalue["childs"];
  size_t chcnt = childs.size();
  for (size_t ix = 0; ix < chcnt; ix++) {
    json &child = childs[ix];
    auto &cname = child["type"].get<string>();
    shared_ptr<control_def_ide> pcontrol_instance = produce_control(cname);
    add_child(pcontrol_instance);
    pcontrol_instance->init_property_from_json(child);
  }
}
void control_def_ide::save_property_to_json(nlohmann::json &junit) {
  string cname = typeid(*this).name();
  cname = cname.substr(sizeof("class vg::"));
  junit["type"] = cname;
  for (auto &prop_ele : _vprop_eles) {
    auto &prop_page = prop_ele->_pro_page;
    for (auto &memb : prop_page) {
      auto &mtype = memb->_type;
      auto &mname = memb->_name;
      auto &mtpsz = memb->_tpsz;
      void *memb_address = memb->_address;
      int array_cnt = memb->_count;
      auto vg_f_2_Json = [](json &jobj, string &ftype, void *membaddr) {
        if (ftype == "vi2") {
          jobj["x"] = *(int *)membaddr;
          jobj["y"] = *((int *)membaddr + 1);
          return true;
        } else if (ftype == "vi3") {
          jobj["x"] = *(int *)membaddr;
          jobj["y"] = *((int *)membaddr + 1);
          jobj["z"] = *((int *)membaddr + 2);
          return true;
        } else if (ftype == "vi4") {
          jobj["x"] = *(int *)membaddr;
          jobj["y"] = *((int *)membaddr + 1);
          jobj["z"] = *((int *)membaddr + 2);
          jobj["w"] = *((int *)membaddr + 2);
          return true;
        } else if (ftype == "vec2") {
          jobj["x"] = *(float *)membaddr;
          jobj["y"] = *((float *)membaddr + 1);
          return true;
        } else if (ftype == "vec3") {
          jobj["x"] = *(float *)membaddr;
          jobj["y"] = *((float *)membaddr + 1);
          jobj["z"] = *((float *)membaddr + 2);
          return true;
        } else if (ftype == "vec4") {
          jobj["x"] = *(float *)membaddr;
          jobj["y"] = *((float *)membaddr + 1);
          jobj["z"] = *((float *)membaddr + 2);
          jobj["w"] = *((float *)membaddr + 2);
          return true;
        }
        return false;
      };
      if (array_cnt > 0) {
        if (mtype == "char") {
          char *str_memb = new char[array_cnt];
          memset(str_memb, 0, array_cnt);
          memcpy(str_memb, memb_address, array_cnt);
          junit[mname] = str_memb;
          delete str_memb;
        } else {
          json marray;
          auto save_to_json = [&](json &jobj, string &ftype, void *membaddr) {
            json jv;
            if (ftype == "int") {
              int imemb = *(int *)membaddr;
              jobj.emplace_back(imemb);
            } else if (ftype == "float" || ftype == "double") {
              float fmemb = *(float *)membaddr;
              jobj.emplace_back(fmemb);
            } else if (mtype == "bool") {
              bool bmemb = *(bool *)membaddr;
              jobj.emplace_back(bmemb);
            } else if (vg_f_2_Json(jv, ftype, membaddr)) {
              jobj.emplace_back(jv);
            } else {
              string vsbytes;
              convert_binary_to_string((char *)membaddr, mtpsz, vsbytes);
              vsbytes[mtpsz] = '\0';
              json vbytes = vsbytes;
              jobj.emplace_back(vbytes);
            }
          };

          for (int ix = 0; ix < array_cnt; ++ix) {
            void *memb_index_address = (char *)memb_address + ix * mtpsz;
            save_to_json(marray, mtype, memb_index_address);
          }
          junit[mname] = marray;
        }
      } else {
        json jobj;
        if (mtype == "int") {
          junit[mname] = *(int *)memb_address;
        } else if (mtype == "float" || mtype == "double") {
          junit[mname] = *(float *)memb_address;
        } else if (mtype == "bool") {
          junit[mname] = *(bool *)memb_address;
        } else if (vg_f_2_Json(jobj, mtype, memb_address)) {
          junit[mname] = jobj;
        } else {
          string vsbytes;
          convert_binary_to_string((char *)memb_address, mtpsz, vsbytes);
          vsbytes[mtpsz * 2] = '\0';
          junit[mname] = vsbytes;
        }
      }
    }
  }

  json jchilds;
  size_t chcnt = child_count();
  for (size_t ix = 0; ix < chcnt; ix++) {
    auto pch_uc =
        static_pointer_cast<shared_ptr<control_def_ide>>(get_child(ix));
    json jchuc;
    (*pch_uc)->save_property_to_json(jchuc);
    jchilds.emplace_back(jchuc);
  }
  if (chcnt > 0) {
    junit["childs"] = jchilds;
  }
}
} // namespace vg