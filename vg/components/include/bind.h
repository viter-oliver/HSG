#pragma once
#include "control_fac.h"
namespace vg{
struct prop_ele_position
{
	sd_control_base _pobj;
	uint16_t _page_index;
	uint16_t _field_index;

	bool operator <(const prop_ele_position& tpp) const
	{
		if (_pobj != tpp._pobj)
		{
			return _pobj < tpp._pobj;
		}
		else
		{
			if (_page_index != tpp._page_index)
			{
				return _page_index < tpp._page_index;
			}
			else
			{
				return _field_index < tpp._field_index;
			}
		}
	}
	bool operator ==(const prop_ele_position& tpp) const
	{
		bool be_equal = _pobj == tpp._pobj&&_page_index == tpp._page_index&&_field_index == tpp._field_index;
		return be_equal;
	}
};
using vprop_pos = std::vector<prop_ele_position>;
struct prop_ele_bind_unit
{
	vprop_pos _param_list;
	std::string _expression;
};

using bind_dic = std::map<prop_ele_position, std::shared_ptr<prop_ele_bind_unit>>;
using bind_ref_dic = std::map<prop_ele_position, std::shared_ptr<vprop_pos>>;
using prop_ele_pos_index = std::vector<u32>;


extern bind_dic g_bind_dic;
extern bind_ref_dic g_bind_ref_dic;
extern void calcu_bind_node(prop_ele_position& pep);
extern void calcu_prop_ele_pos_index(const prop_ele_position& pep, prop_ele_pos_index& pep_id); 
}