#pragma once
#include <chrono>
#include "bind.h"
namespace vg {
using sd_prop_ele_position = std::shared_ptr<prop_ele_position>;
using aliase_map = std::map<std::string, sd_prop_ele_position>;
extern aliase_map g_aliase_dic;
struct lazy_value
{
     std::string _value;
     std::chrono::steady_clock::time_point  _start;
     int _during;//milliseconds
};
using prop_ele_value_dic = std::map<std::string, lazy_value>;
extern bool AFG_EXPORT set_property_aliase_value(std::string prp_aliase_name, void* pvalue,u32 value_sz);
extern bool AFG_EXPORT set_property_txt_aliase_value( std::string prp_aliase_name, int sub_id,int group_id=0);
extern prop_ele_value_dic g_lazy_value_buff;
extern bool AFG_EXPORT set_property_aliase_lazy_value( std::string prp_aliase_name, int during, void* pvalue );
extern void AFG_EXPORT execute_lazy_value();
extern AFG_EXPORT sd_control_base  get_aliase_ui_control(std::string prp_aliase_name);


/***/template<class T> bool set_property_aliase_value_T(std::string prp_aliase_name, T pvalue )
{
     return set_property_aliase_value( prp_aliase_name, &pvalue,sizeof T);
}

template<> inline bool set_property_aliase_value_T(std::string prp_aliase_name, std::string pvalue )
{
     return set_property_aliase_value( prp_aliase_name, (void*)pvalue.c_str(),pvalue.size());
}
template<class T> bool get_prop_fd_value( prop_ele_position& pep, T& pvalue )
{
     auto& pgidx = pep._page_index;
     auto& fdidx = pep._field_index;
     auto& field = pep._pobj->get_filed_ele( pgidx, fdidx );
     if (field._tpsz!=sizeof(T))
     {
          printf("The variable you input which type is invalid for(%s_%d_%d)!\n",pep._pobj->get_name().c_str(),pgidx,fdidx);
          return false;
     }
     memcpy( &pvalue, field._address, field._tpsz );
     return true;
}
template<class T> bool get_property_aliase_value_T(std::string prp_aliase_name, T& value)
{
	const auto& ialiase = g_aliase_dic.find(prp_aliase_name);
	if (ialiase!=g_aliase_dic.end())
	{
		printf("unkown aliase name:%s\n", prp_aliase_name.c_str());
		return false;
	}
	const auto& prop_pos = *ialiase->second;
	return get_prop_fd_value<T>(prp_aliase_name, value);
}
} // namespace vg