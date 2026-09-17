#pragma once
#include "VarEditorRegistry.h"

namespace vg {

// 默认模板：不注册任何东西
template<typename T>
struct TypeEditor {
    static void register_editor() {}
};

// 注册宏，用于声明一个类型的编辑器
#define REGISTER_TYPE_EDITOR(TYPE, BODY) \
    namespace vg { \
        template<> struct TypeEditor<TYPE> { \
            static void register_editor() { \
                VarEditorRegistry::instance().register_type<TYPE>(BODY); \
            } \
            static inline bool _auto_reg = ([](){ register_editor(); return true; })(); \
        }; \
    }

} // namespace vg
