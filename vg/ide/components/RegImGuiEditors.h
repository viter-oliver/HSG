#pragma once
#include "VarEditorRegistry.h"
#include "font.h"
#include "user_control_imgui.h"
#include "vertex.h"
#include "vg_texture.h"
#include "shader.h"
namespace vg {

  template <typename T> struct TypeEditor {
  static void register_editor() {
    using vg::VarEditorRegistry;
    VarEditorRegistry &reg = VarEditorRegistry::instance();
    using traits = is_instance_of_vg_array<T>;
    static class {
        const u8 u8_v[2] = { 0, 255 };     // u8
        const u16 u16_v[2] = {0, 2000};      // u16
        const u32 u32_v[2] = { 0, 4000 };      // u32
        const s8 s8_v[2] = {-127, 127};    // s8
        const s16 s16_v[2] = {-200, 200};    // s16
        const s32 s32_v[2]{ -2000, 2000 };  // s32
        const f32 f32_v[2] = { -2000.f, 2000.f };  // f32
        const f64 fg64_v[2] = { -20000.f, 20000.f }; // f64
		const f32 f32_v_urd[2] = { 0.f,360.f }; 
        const f32 f32_v_nml[2] = { 0,1.f };
        const f32 f32_v_utn[2] = { 0,10.f };
	public:
        void get(int range_id,const void*& min, const void*& max,std::string& tailstr) {
            switch (range_id)
            {
                case u8_value:
					min = &u8_v[0];
                    max= &u8_v[1];
				    break;
				case u16_value:
                    min = &u16_v[0];
					max = &u16_v[1];
					break;
                case u32_value:
					min = &u32_v[0];
					max = &u32_v[1];
                    break;
				case s8_value:
					min = &s8_v[0];
					max = &s8_v[1];
					break;
				case s16_value:
					min = &s16_v[0];
					max = &s16_v[1];
					break;
				case s32_value:
					min = &s32_v[0];
					max = &s32_v[1];
					break;
				case f32_value:
                    if (tailstr == "_urd") {
                        min = &f32_v_urd[0];
                        max= &f32_v_urd[1];
                    }
                    else if (tailstr == "_nml") {
                        min = &f32_v_nml[0];
                        max= &f32_v_nml[1];

                    }
                    else if (tailstr == "_utn") {
                        min = &f32_v_utn[0];
                        max= &f32_v_utn[1];
                    }
                    else {
					    min = &f32_v[0];
					    max = &f32_v[1];
                    }
					break;
				case f64_value:
					min = &fg64_v[0];
					max = &fg64_v[1];
					break;
            default:
                min = &u8_v[0];
                max = &u8_v[1];
                break;
            }
		}

    }range;
    ImGuiDataType dtype = ImGuiDataType_Float;
    int range_id = f32_value;
    if constexpr (traits::value) {
      using InnerT = typename traits::value_type;
      constexpr int N = traits::size;
      if constexpr (N == 50 && std::is_same_v<InnerT, s8>) {
        reg.register_type<T>([](const std::string &name, void *ptr) {
          ImGui::PushID(ptr);
          ImGui::InputText(name.c_str(), (char*)ptr, FILE_NAME_LEN);
          ImGui::PopID();
        });
      } else {
        if constexpr (std::is_same_v<InnerT, u8>) {
          dtype = ImGuiDataType_U8;
          range_id = u8_value;
        } else if constexpr (std::is_same_v<InnerT, u16>) {
          dtype = ImGuiDataType_U16;
          range_id = u16_value;
        } else if constexpr (std::is_same_v<InnerT, u32>) {
          dtype = ImGuiDataType_U32;
          range_id = u32_value;
        } else if constexpr (std::is_same_v<InnerT, s8>) {
          dtype = ImGuiDataType_S8;
          range_id = s8_value;
        } else if constexpr (std::is_same_v<InnerT, s16>) {
          dtype = ImGuiDataType_S16;
          range_id = s16_value;
        } else if constexpr (std::is_same_v<InnerT, s32>) {
          dtype = ImGuiDataType_S32;
          range_id = s32_value;
        } else if constexpr (std::is_same_v<InnerT, f32>) {
          dtype = ImGuiDataType_Float;
          range_id = f32_value;
        } else if constexpr (std::is_same_v<InnerT, f64>) {
          dtype = ImGuiDataType_Double;
          range_id = f64_value;
        }
        const ImGuiDataType dataType = dtype;
        const int count = N;
        const int rid = range_id;
        reg.register_type<T>([dataType,count,rid](const std::string &name, void *ptr) {
           const void *min, * max;
            std::string tailstr;
            get_tailstr(name.c_str(), tailstr);
            range.get(rid, min, max, tailstr);
            /*std::string vname;
            if (tailstr.size() > 0) {
                get_substr_from_tail(name.c_str(), vname);
            }
            else {
                vname = name;
            }*/
            SliderScalarMatrixN(name.c_str(), dataType, ptr, count, min,
                               max);
        });
      }
    } else {
        if constexpr (std::is_same_v<T, bool>) {
            reg.register_type<T>([](const std::string& name, void* ptr) {
                ImGui::Checkbox(name.c_str(), (bool*)ptr);
                });
        }
        else {
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
          const ImGuiDataType dataType = dtype;
          const int rid = range_id;
          reg.register_type<T>([dataType,rid](const std::string &name, void *ptr) {
		      const void *min, *max;
              std::string tailstr;
              std::string vname;
              get_tailstr(name.c_str(), tailstr);
		      range.get(rid, min, max, tailstr);
              if (tailstr.size() > 0&& tailstr[0]=='_') {
                  get_substr_from_tail(name.c_str(), vname);
              }
              else {
                  vname = name;
              }
              SliderScalarMatrixN(vname.c_str(), dataType, ptr, 1, min,
                                 max);
          });
        }
      
    }
  }
  static inline bool _auto_reg = ([]() {
    register_editor();
    return true;
  })();
};

template <typename... Elems, int... Ns>
void ensure_common_types(std::integer_sequence<int, Ns...>) {
  // 对每个 Elem 注册它自己
  ((void)vg::TypeEditor<Elems>::_auto_reg, ...);

  // 对每个 Elem 的数组类型分别注册
  (
      []<typename T, int... Dims>() {
        ((void)vg::TypeEditor<vg_array<T, Dims>>::_auto_reg, ...);
      }.template operator()<Elems, Ns...>(),
      ...);
}
// ensure_common_types<int, float, double>(std::integer_sequence<int, 2,3,4>{});

template <typename T> struct TypeEditor<key_t<T>> {
  static void register_editor() {
    VarEditorRegistry::instance().register_type<key_t<T>>(
        [](const std::string &name, void *ptr) {
          std::string type_name = typeid(T).name();
          type_name = type_name.substr(type_name.find_last_of(':') + 1);
          internal_image(type_name.c_str());
          auto key = static_cast<key_t<T> *>(ptr);
          ImGui::PushID(ptr);
          ImGui::InputText(name.c_str(), key->value, 50);
		  ImGui::PopID();
          if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload *payload =
                    ImGui::AcceptDragDropPayload(type_name.c_str())) {
              if (ImGui::IsMouseReleased(0)) {
                memcpy_s(key->value, payload->DataSize, payload->Data,
                         payload->DataSize);
              }
            }
            ImGui::EndDragDropTarget();
          }
          if constexpr (std::is_same_v<T, texture>) {
#if 1
            auto itxt = texture_map_.find(*key);
            if (itxt != texture_map_.end()) {
              auto &tx = *itxt->second;
              ImGui::Image((ImTextureID)tx._txt_id,
                           ImVec2(tx._width, tx._height), ImVec2(0, 0),
                           ImVec2(1, 1), ImColor(255, 255, 255, 255),
                           ImColor(255, 255, 255, 128));
			  ImGui::SameLine();
			  ImGui::Text("original size:%dx%d", tx._width, tx._height);
            }
#endif
          } else if constexpr (std::is_same_v<T, font_unit>) {

          } else if constexpr (std::is_same_v<T, vertex>) {
          }
        });
  }

  static inline bool _auto_reg = ([]() {
    register_editor();
    return true;
  })();
};

template <typename... key_type> void ensure_key_types() {
  // 对每个 Elem 注册它自己和对应的数组版本
  ((void)vg::TypeEditor<key_t<key_type>>::_auto_reg, ...);
}
void force_link_ImGuiEditors();
} // namespace vg