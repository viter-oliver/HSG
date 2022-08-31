#include "material_3d.h"
#include "user_control_imgui.h"
#include "material_container.h"
namespace vg {
void material_3d::ex_init_fun() {
  reg_property_handle(&_pt, &_pt._draw_mode, [this]() {
    ImGui::Combo("Draw mode:", &_pt._draw_mode, draw_mode, en_gl_count);
  });
  reg_property_handle(&_pt, _pt._primitive_name, [this]() {
    if (_ps_prm) {
      ImGui::Text("Primitive object:%s", _pt._primitive_name);
      ImGui::SameLine();
      if (ImGui::Button("Delink##prm_obj")) {
        _ps_prm = nullptr;
        _matched = false;
        if (_gpu_outbuff != 0) {
          glDeleteBuffers(1, &_gpu_outbuff);
          _gpu_outbuff = 0;
        }
      }
    } else {
      ImGui::InputText("Primitive object:", _pt._primitive_name, FILE_NAME_LEN);
      if (ImGui::Button("Link##prm")) {
        auto &iprm = g_primitive_list.find(_pt._primitive_name);
        if (iprm != g_primitive_list.end()) {
          // sscanf()

          _ps_prm = iprm->second;
          const auto &imdl = g_mmodel_list.find(_ps_prm->_model_name);
          if (imdl != g_mmodel_list.end()) {
            auto &mdl = *imdl->second;
            auto mesh_size = mdl.size();
            if (_ps_prm->_mesh_id < mesh_size) {
              auto &mesh_tar = mdl[_ps_prm->_mesh_id];
              auto &tbox = mesh_tar._box;
              vec3 pt_core = {(tbox._xmax - tbox._xmin) * 0.5,
                              (tbox._ymax - tbox._ymin) * 0.5,
                              (tbox._zmax - tbox._zmin) * 0.5};
              _pt._cam._position = pt_core;
              _pt._cam._position.z = 5 * pt_core.z;
              _pt._cam._direction = _pt._cam._position - pt_core;
              _pt._cam._up = {0.f, 1.f, 0.f};
            }
          }
          if (_pt._with_feedback) {
            glGenBuffers(1, &_gpu_outbuff);
            glBindBuffer(GL_ARRAY_BUFFER, _gpu_outbuff);
            auto buff_sz = _ps_prm->_vertex_buf_len * sizeof(GLfloat);
            glBufferData(GL_ARRAY_BUFFER, buff_sz, nullptr, GL_STATIC_READ);
          }
        } else {
          string str_prm_nm(_pt._primitive_name);
          if (ref_a_intenal_primitive(str_prm_nm)) {
            _ps_prm = g_primitive_list[str_prm_nm];
          }
        }
      }
    }
    if (_pt._with_feedback) {
      if (ImGui::Button("Show feedback")) {
        get_output_vertex(_output_buff);
        ImGui::OpenPopup("ShowFeedback");
      }
      if (ImGui::BeginPopupModal("ShowFeedback")) {
        auto stride = 3; // _ps_prm->get_stride();
        auto osz = _output_buff.size();
        for (int ix = 0; ix < osz; ix++) {
          ImGui::Text("%f", _output_buff[ix]);
          auto imd = ix % stride;
          if (imd != (stride - 1)) {
            ImGui::SameLine();
          }
        }
        if (ImGui::Button("Cancel")) {
          ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
      }
    }
  });
  reg_property_handle(&_pt, _pt._material_name, [this]() {
    if (_ps_mtl) {
      ImGui::Text("Material name:%s", _pt._material_name);
      ImGui::SameLine();
      if (ImGui::Button("Delink##material")) {
        _ps_mtl = nullptr;
        _matched = false;
      }
    } else {
      ImGui::InputText("Material name:", _pt._material_name, FILE_NAME_LEN);
      if (ImGui::Button("Link##mtl")) {
        auto &imtl = shader::_mp_sd_materials.find(_pt._material_name);
        if (imtl != shader::_mp_sd_materials.end()) {
          _ps_mtl = imtl->second;
        }
      }
    }
    if (!_matched) {
      ImGui::Text("material is unmatched to primitive object");
    }
  });

}
} // namespace vg