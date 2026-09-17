#pragma once
#include "bind_var.h"
#include "vg_texture.h"
#include "font.h"
#include "vertex.h"
#include "shader.h"

namespace vg{
  template<typename T>
  void bind_var_t<T>::edit() {
    using traits = is_instance_of_vg_array<T>;

    int range[type_value_number][2] =
    { 
      {0,255},//u8
      {0,2000},//u16
      {0,4000},//u32
      {-127,128},//s8
      {-200,200},//s16
      { -2000, 2000},//s32
      { -2000, 2000},//f32
      { -20000, 20000},//f64
      { 0, 1}//f64
    };
    ImGuiDataType dtype;
    int range_id = 0;
    if constexpr (traits::value) {
        using InnerT = typename traits::value_type;
        constexpr int N = traits::size;
        if constexpr (N == 50 && std::is_same_v<InnerT, s8>) {
            ImGui::InputText(_var_name.c_str(), _value.value, FILE_NAME_LEN);
        }
        else {
            if constexpr (std::is_same_v<InnerT, u8>) {
                dtype = ImGuiDataType_U8;
                range_id = u8_value;
            }
            else if constexpr (std::is_same_v<InnerT, u16>) {
                dtype = ImGuiDataType_U16;
                range_id = u16_value;
            }
            else if constexpr (std::is_same_v<InnerT, u32>) {
                dtype = ImGuiDataType_U32;
                range_id = u32_value;
            }
            else if constexpr (std::is_same_v<InnerT, s8>) {
                dtype = ImGuiDataType_S8;
                range_id = s8_value;
            }
            else if constexpr (std::is_same_v<InnerT, s16>) {
                dtype = ImGuiDataType_S16;
                range_id = s16_value;
            }
            else if constexpr (std::is_same_v<InnerT, s32>) {
                dtype = ImGuiDataType_S32;
                range_id = s32_value;
            }
            else if constexpr (std::is_same_v<InnerT, f32>) {
                dtype = ImGuiDataType_Float;
                range_id = f32_value;
            }
            else if constexpr (std::is_same_v<InnerT, f64>) {
                dtype = ImGuiDataType_Double;
                range_id = f64_value;
            }
            ImGui::SliderScalarN(_var_name.c_str(), dtype, _value.value,
                N,
                &range[range_id][0],
                &range[range_id][1]);
        }
        
    } else {
        if constexpr (std::is_same_v<T, u8>) {
            dtype = ImGuiDataType_U8;
            range_id = u8_value;
        } else if constexpr (std::is_same_v<T, u16>) {
            dtype = ImGuiDataType_U16;
            range_id = u16_value;
        } else if constexpr (std::is_same_v<T, u32>) {
            dtype = ImGuiDataType_U32;
            range_id = u32_value;
        } else if constexpr (std::is_same_v<T, s8>) {
            dtype = ImGuiDataType_S8;
            range_id = s8_value;
        } else if constexpr (std::is_same_v<T, s16>) {
            dtype = ImGuiDataType_S16;
            range_id = s16_value;
        } else if constexpr (std::is_same_v<T, s32>) {
            dtype = ImGuiDataType_S32;
            range_id = s32_value;
        } else if constexpr (std::is_same_v<T, f32>) {
            dtype = ImGuiDataType_Float;
            range_id = f32_value;
        } else if constexpr (std::is_same_v<T, f64>) {
            dtype = ImGuiDataType_Double;
            range_id = f64_value;
        }
        ImGui::SliderScalarN(_var_name.c_str(), dtype, &_value,
            1,
            &range[range_id][0],
            &range[range_id][1]
        );
        //ImGui::InputFloat(_var_name.c_str(), _value.value);
    }

}
template<typename T>
struct bind_var_t<key_t<T>> {
    std::string _var_name;
    key_t<T>& _value;
    bind_var_t(std::string name, key_t<T>& value)
        : _var_name(name), _value(value) {
    }

    void edit() {
        //ImGui::Text("key_t<%s> editing disabled", typeid(T).name());
        std::string ty_namne = typeid(T).name();
        internal_image(ty_namne.c_str());
        ImGui::SameLine(0, 20); 
        ImGui::InputText(_var_name.c_str(), _value.value, FILE_NAME_LEN); 
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ty_namne.c_str())) {
                    if (ImGui::IsMouseReleased(0)) {
                            memcpy_s(_value.value, payload->DataSize, payload->Data, payload->DataSize); 
                    }
            }
            ImGui::EndDragDropTarget(); 
        }
       
    }
};
}
