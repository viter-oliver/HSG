#pragma once
#include "TypeEditor.h"
#include "user_control_imgui.h"

namespace vg {

REGISTER_TYPE_EDITOR(int, [](const std::string& name, void* ptr) {
    ImGui::InputInt(name.c_str(), static_cast<int*>(ptr));
});

REGISTER_TYPE_EDITOR(float, [](const std::string& name, void* ptr) {
    ImGui::InputFloat(name.c_str(), static_cast<float*>(ptr));
});

REGISTER_TYPE_EDITOR(unsigned short, [](const std::string& name, void* ptr) {
    auto val = static_cast<unsigned short*>(ptr);
    int tmp = *val;
    if (ImGui::SliderInt(name.c_str(), &tmp, 0, 65535)) *val = (unsigned short)tmp;
});

// -------- vg_array 类型 --------
template<typename T, int N>
struct TypeEditor<vg_array<T, N>> {
    static void register_editor() {
        VarEditorRegistry::instance().register_type<vg_array<T, N>>(
            [](const std::string& name, void* ptr) {
                auto arr = static_cast<vg_array<T,N>*>(ptr);
                if constexpr (std::is_same_v<T, float>) {
                    if constexpr (N == 2) ImGui::SliderFloat2(name.c_str(), arr->value, -10, 10);
                    else if constexpr (N == 3) ImGui::SliderFloat3(name.c_str(), arr->value, -10, 10);
                    else if constexpr (N == 4) ImGui::SliderFloat4(name.c_str(), arr->value, -10, 10);
                } else {
                    ImGui::Text("%s: unsupported vg_array type", name.c_str());
                }
            });
    }
    static inline bool _auto_reg = ([](){ register_editor(); return true; })();
};
// 针对所有 key_t<T>
template<typename T>
struct TypeEditor<key_t<T>> {
    static void register_editor() {
        VarEditorRegistry::instance().register_type<key_t<T>>(
            [](const std::string& name, void* ptr) {
                auto key = static_cast<key_t<T>*>(ptr);
                ImGui::InputText(name.c_str(), key->value, 50);
            });
    }
    static inline bool _auto_reg = ([](){ register_editor(); return true; })();
};
} // namespace vg
