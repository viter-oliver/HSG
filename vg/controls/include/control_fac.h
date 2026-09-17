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
    auto new_sd_ctl=std::move(control_factory::instance().Create(cname));
    cname += "*";
	new_sd_ctl->set_name(cname);
#ifdef WITHIN_IDE
    if (new_sd_ctl)
        new_sd_ctl->ex_init_fun();
#endif
    return new_sd_ctl;
}

#define REG_CTL_2_FAC(T) static control_factory::control_fac_assist<T> reg_##T(#T)
#define REG_CTL_2_FAC_ARGS(T,...) static control_factory::control_fac_assist<T,__VA_ARGS__> reg_##T(#T)

#ifdef WITHIN_IDE
REG_CTL_2_FAC(control_def_ide);
#else
REG_CTL_2_FAC(control_def);
#endif
inline sd_control_base get_copy_of_object(sd_control_base byobject) {
    std::string cname = typeid(*byobject).name();
    cname = cname.substr(sizeof("class vg:"));

    auto pnew = produce_control(cname);
    auto &mem_page_by = byobject->get_mem_page();
    auto &mem_page_new = pnew->get_mem_page();
    for (int ix = 0; ix < mem_page_by.size(); ix++) {
        memcpy_s(mem_page_new[ix]._pro_address, mem_page_new[ix]._pro_sz,
                 mem_page_by[ix]._pro_address, mem_page_by[ix]._pro_sz);
    }
    pnew->link();
    auto icnt = byobject->child_count();
    for (int ii = 0; ii < icnt; ii++) {
        auto pchild =
            std::static_pointer_cast<control_base>( byobject->get_child(ii));
        auto pchd_cpy = get_copy_of_object(pchild);
        pnew->add_child(pchd_cpy);
    }
    return pnew;
}
inline sd_control_base get_hit_obj_by(vec2 &tar_pos, sd_control_base sd_obj) {
    for (int ix = sd_obj->child_count()-1; ix >= 0; ix--) {
        auto sd_child = std::static_pointer_cast<control_base> (sd_obj->get_child(ix));
        if (sd_child->visibility()) {
            auto sd_hit_obj = get_hit_obj_by(tar_pos, sd_child);
            if (sd_hit_obj)
              return sd_hit_obj;
        }
    }
    if (sd_obj->contain(tar_pos)) {
        return sd_obj;
    } else {
        return nullptr;
    }
}
}