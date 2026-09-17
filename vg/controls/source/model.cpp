#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "model.hpp"

namespace vg
{
    using namespace std;
    void ctrl_model::calculate_properties(m_sd_bind_var &child_properties)
    {
      FIND_ITEM(model)
      if (imodel != child_properties.end()) {
        auto &s_band_v = imodel->second;
        auto s_band_v16 = dynamic_pointer_cast<bind_var_t<f32_16>>(s_band_v);
        if (s_band_v16 == nullptr) {
          return;
        }
        auto &bind_v_16 = *s_band_v16;
        const auto copy_size = sizeof(float) * 16;
        glm::mat4 parent_model=glm::make_mat4(bind_v_16._value.value);
        bind_v_16._calculated = true;
        glm::vec3 gtranslate(_pt._trans_translation_x, _pt._trans_translation_y,
                             _pt._trans_translation_z);
        glm::vec3 gscale(_pt._trans_scale_x, _pt._trans_scale_y,
                         _pt._trans_scale_z);
        function<void()> f_rotate[en_rotate_order_cnt] = {
            [&]() {
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_x,
                                         glm::vec3(1.0f, 0.0f, 0.0f));
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_y,
                                         glm::vec3(0.0f, 1.0f, 0.0f));
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_z,
                                         glm::vec3(0.0f, 0.0f, 1.0f));
            },
            [&]() {
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_x,
                                         glm::vec3(1.0f, 0.0f, 0.0f));
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_z,
                                         glm::vec3(0.0f, 0.0f, 1.0f));
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_y,
                                         glm::vec3(0.0f, 1.0f, 0.0f));
            },
            [&]() {
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_y,
                                         glm::vec3(0.0f, 1.0f, 0.0f));
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_x,
                                         glm::vec3(1.0f, 0.0f, 0.0f));
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_z,
                                         glm::vec3(0.0f, 0.0f, 1.0f));
            },
            [&]() {
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_y,
                                         glm::vec3(0.0f, 1.0f, 0.0f));
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_z,
                                         glm::vec3(0.0f, 0.0f, 1.0f));
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_x,
                                         glm::vec3(1.0f, 0.0f, 0.0f));
            },
            [&]() {
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_z,
                                         glm::vec3(0.0f, 0.0f, 1.0f));
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_x,
                                         glm::vec3(1.0f, 0.0f, 0.0f));
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_y,
                                         glm::vec3(0.0f, 1.0f, 0.0f));
            },
            [&]() {
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_z,
                                         glm::vec3(0.0f, 0.0f, 1.0f));
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_y,
                                         glm::vec3(0.0f, 1.0f, 0.0f));
              parent_model = glm::rotate(parent_model, _pt._trans_rotation_x,
                                         glm::vec3(1.0f, 0.0f, 0.0f));
            },
        };
        function<void()> f_trans[en_trans_order_cnt] = {
            [&]() {
              f_rotate[_pt._rotate_order]();
              parent_model = glm::scale(parent_model, gscale);
              parent_model = glm::translate(parent_model, gtranslate);
            },
            [&]() {
              parent_model = glm::scale(parent_model, gscale);
              f_rotate[_pt._rotate_order]();
              parent_model = glm::translate(parent_model, gtranslate);
            },
            [&]() {
              f_rotate[_pt._rotate_order]();
              parent_model = glm::translate(parent_model, gtranslate);
              parent_model = glm::scale(parent_model, gscale);
            },
            [&]() {
              parent_model = glm::translate(parent_model, gtranslate);
              f_rotate[_pt._rotate_order]();
              parent_model = glm::scale(parent_model, gscale);
            },
            [&]() {
              parent_model = glm::scale(parent_model, gscale);
              parent_model = glm::translate(parent_model, gtranslate);
              f_rotate[_pt._rotate_order]();
            },
            [&]() {
              parent_model = glm::translate(parent_model, gtranslate);
              parent_model = glm::scale(parent_model, gscale);
              f_rotate[_pt._rotate_order]();
            },
        };
        f_trans[_pt._trans_order]();
        //glm::mat4 child_model = glm::make_mat4(bind_v_16._value.value);
        //child_model = parent_model * child_model;
        auto pmodel = glm::value_ptr(parent_model);
        memcpy_s(bind_v_16._value.value, copy_size, pmodel, copy_size);
      } 
      shared_properties::calculate_properties(child_properties);
    }
}