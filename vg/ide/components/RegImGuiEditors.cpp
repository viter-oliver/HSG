#include "RegImGuiEditors.h"
namespace vg {
    using namespace shader;
    void force_link_ImGuiEditors() {}
    struct AutoEditorRegister {
        AutoEditorRegister() {
            ensure_common_types<f32, s32, f64,u32,bool>(
                std::integer_sequence<int, 2, 3, 4,6,8,9,12,16>{}
            );
            ensure_common_types<s8>(
                std::integer_sequence<int, 50>{}
            );
            ensure_key_types<texture, font_unit, vertex,shader_base>();
        }
    };
    static AutoEditorRegister _auto_editor_register;

#if 0 
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

#define EDIT_COMPONENT(item, expression)                                       \
  void bind_var_t<item##_key>::edit() {                                        \
    internal_image(#item);                                                     \
    ImGui::SameLine(0, 20);                                                    \
    ImGui::InputText(_var_name.c_str(), _value.value, FILE_NAME_LEN);          \
    if (ImGui::BeginDragDropTarget()) {                                        \
      if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(#item)) { \
        if (ImGui::IsMouseReleased(0)) {                                       \
          memcpy_s(_value.value, payload->DataSize, payload->Data,             \
                   payload->DataSize);                                         \
        }                                                                      \
      }                                                                        \
      ImGui::EndDragDropTarget();                                              \
    }                                                                          \
    auto i##item = item##_map_.find(_value);                                   \
    if (i##item != item##_map_.end()) {                                        \
      auto &unit = *i##item->second;                                           \
      expression                                                               \
    }                                                                          \
  }
EDIT_COMPONENT(texture, ImGui::Image((ImTextureID)unit._txt_id,
                                     ImVec2(unit._width, unit._height));)
EDIT_COMPONENT(font_unit, ;)
EDIT_COMPONENT(vertex, ;)

void bind_var_t<texture_key>::edit() {
  internal_image("texture");
  ImGui::SameLine(0, 20);
  ImGui::InputText(_var_name.c_str(), _value.value, FILE_NAME_LEN);
  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload *payload =
            ImGui::AcceptDragDropPayload("texture")) {
      if (ImGui::IsMouseReleased(0)) {
        memcpy_s(_value.value, payload->DataSize, payload->Data,
                 payload->DataSize);
      }
    }
    ImGui::EndDragDropTarget();
  }
  auto itxt = texture_map_.find(_value);
  if (itxt != texture_map_.end()) {
    auto &tx = *itxt->second;
    ImGui::Image((ImTextureID)tx._txt_id, ImVec2(tx._width, tx._height),
                 ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255),
                 ImColor(255, 255, 255, 128));
  }
}
#endif
} // namespace vg
