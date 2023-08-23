#pragma once
#include <chrono>
#include "bind_base.h"
namespace vg {

extern m_sd_bind_base g_aliase_dic;
template<class T> 
bool set_property_aliase_value_T(std::string aliase_name, T tvalue )
{
    auto& ialias=g_aliase_dic.find(aliase_name);
    if(ialias==g_aliase_dic.end()){
     vg_print("%s is invalid alias name!\n",aliase_name.c_str());
     return false;
    }
    auto& sbind=ialias->second;
    if(typeid(*sbind)!=typeid(bind_var_t<T>)){
     vg_print("The type of %s is invalid!\n",aliase_name.c_str());
     return false;
    }
    auto s_t_bind=std::static_pointer_cast<bind_var_t<T>>(sbind);
   
    (*s_t_bind)._value=tvalue;
    return true;
}

template<> inline bool set_property_aliase_value_T(std::string aliase_name, char_50 svalue )
{
    const auto& ialias=g_aliase_dic.find(aliase_name);
    if(ialias==g_aliase_dic.end()){
     vg_print("%s is invalid alias name!\n",aliase_name.c_str());
     return false;
    }
    auto& sbind=ialias->second;
    if(typeid(*sbind)!=typeid(bind_var_t<char_50>)){
     vg_print("The type of %s is invalid!\n",aliase_name.c_str());
     return false;
    }
    sbind->_host;
    auto s_t_bind=std::static_pointer_cast<bind_var_t<char_50>>(sbind);
    strcpy((*s_t_bind)._value.value,svalue.value);
    return true;
}

} // namespace vg