#pragma once
#include <string>
#include "vg_type.h"
#include "bind_var_def.h"
namespace vg{
 template <class T> 
 struct var_base_t:public var_base {
     T _value;
 };
 template<class T>
  struct bind_var_t:public bind_var{
    std::string _var_name;
    T& _value;
    bind_var_t(std::string name,T& value):_var_name(name),_value(value){}
    sd_var_base spawn() {
        auto sd_v=std::make_shared<var_base_t<T>>();
        sd_v->_value = _value;
        return sd_v;
    }
    bind_var_t<T>& opratpr = (var_base_t<T>&var){
        _value = var._value;
        return *this;
    }
    bool assign(sd_var_base& sbase) {
        if (typeid(*sbase) == typeid(var_base_t<T>)) {
            auto sd_var_base_t = std::static_pointer_cast<var_base_t<T>>(sbase);
            _value = sd_var_base_t->_value;
            return true;
        }
        return false;
    }
    bool scale(sd_var_base from, sd_var_base to, double s) override {
        if (typeid(*from) != typeid(var_base_t<T>)
            || typeid(*to) != typeid(var_base_t<T>)
            ) {
            return false
        }
        auto sd_from = std::static_pointer_cast<var_base_t<T>>(from);
        auto sd_to = std::static_pointer_cast<var_base_t<T>>(to);
        T valueSp = sd_to->_value - sd_from->_value;
        _value = sd_from->_value + valueSp * s;
        return true;
    }
    void edit();
  };
}