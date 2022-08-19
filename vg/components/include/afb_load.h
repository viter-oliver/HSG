#pragma once
#include "control_common_def.h"
#include <msgpack.hpp>
//#include <functional>
using namespace vg;
class afb_load
{
	control_common_def*& _pj;
	//function<void(control_common_def*)> _impl;
	void init_ui_component_by_mgo(control_common_def*&ptar, msgpack::v2::object& obj);
public:
	afb_load(control_common_def*&pr_root) :_pj(pr_root){}
	~afb_load(){}
	
	void load_afb(const char* afb_file);
};

