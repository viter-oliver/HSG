#pragma once
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <string>

namespace vg {

// 注册表核心类
class VarEditorRegistry {
public:
    using EditFunc = std::function<void(const std::string&, void*)>;

    static VarEditorRegistry& instance() {
        static VarEditorRegistry inst;
        return inst;
    }

    template<typename T>
    void register_type(EditFunc func) {
        auto typeinfo = std::type_index(typeid(T));
        editors_[typeinfo] = std::move(func);
    }

    template<typename T>
    bool edit(const std::string& name, T& value) const {
		auto typeinfo = std::type_index(typeid(T));
        auto it = editors_.find(typeinfo);
        if (it != editors_.end()) {
            it->second(name, &value);
            return true;
        }
        return false;
    }

private:
    std::unordered_map<std::type_index, EditFunc> editors_;
};

} // namespace vg
