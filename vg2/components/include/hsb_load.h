#pragma once
#include "control_fac.h"
#include <msgpack.hpp>
//#include <functional>

class hsb_load
{
	control_base*& _pj;
	//function<void(control_common_def*)> _impl;
	void init_ui_component_by_mgo(control_base*&ptar, msgpack::v2::object& obj);
public:
	hsb_load(control_base*&pr_root) :_pj(pr_root){}
	~hsb_load(){}
	
	void load_afb(const char* afb_file);
};

