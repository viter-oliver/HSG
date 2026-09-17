#include "shared_properties.h"
#include "user_control_imgui.h"
#include "imgui_internal.h"
namespace vg{
    using namespace std;
    void shared_properties::draw_properties(){
        string type_name = typeid(*this).name();
        string type_show = type_name.substr(sizeof("class vg:"));
        ImGui::Text("Type name:%s", type_show.c_str());
        auto parent = get_parent();
        auto test_ptr = dynamic_cast<shared_properties*>(parent);
        if (test_ptr) {
          #if 0
          m_sd_bind_var tmp_var_dic;
          for (auto &s_bv : _v_bind_var) {
            tmp_var_dic[s_bv->var_name()]=s_bv->clone();
          }
          test_ptr->calculate_properties(tmp_var_dic);
          for (auto &var_pair : tmp_var_dic) {
            var_pair.second->edit();
          }
          #else
          for (auto &s_bv : _v_bind_var) {
            s_bv->_calculated = false;
          }
          test_ptr->calculate_properties(_m_bind_var);
          int id = 0;
          for (auto &s_bv : _v_bind_var) {
            if (!s_bv->_calculated) {
              auto iuhd = _user_properties_handlor.find(id);
              if (iuhd != _user_properties_handlor.end()) {
                iuhd->second();
              } else {
                s_bv->edit();
              }
            }
            id++;
          }
          #endif
        } else {
          control_base::draw_properties();
          /*
          for (auto &s_bv : _v_bind_var) {
            s_bv->edit();
          }*/
        }
    }
}