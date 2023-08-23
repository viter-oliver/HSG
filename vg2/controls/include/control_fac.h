#pragma once
#ifdef WITHIN_IDE
#include "control_def_ide.h"
using control_base=vg::control_def_ide;
#else
#include "control_def.h"
using control_base=vg::control_def;
#endif
#include <fab/fab.h>
namespace vg{
using sd_control_base=std::shared_ptr<control_base>;
using control_fac=fab::Factory<control_base>;

struct control_factory {
    template<typename T, typename ... Args >
    struct control_fac_assist {
        static T* create_new_obj(Args ...args) {
            return new T(args...);
        }
        control_fac_assist(std::string key) {
            instance().Register(key, control_fac_assist<T, Args...>::create_new_obj);
        }
    };
    static control_fac& instance() {
        static  control_fac instance_;
        return instance_;
    }
};
inline sd_control_base produce_control(std::string cname){
    return std::move(control_factory::instance().Create(cname));
}
#define REG_CTL_2_FAC(T) static control_factory::control_fac_assist<T> reg_##T(#T)
#define REG_CTL_2_FAC_ARGS(T,...) static control_factory::control_fac_assist<T,__VA_ARGS__> reg_##T(#T)
}