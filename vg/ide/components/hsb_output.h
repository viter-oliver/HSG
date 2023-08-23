#pragma once
#include "control_fac.h"
class hsb_output
{
	control_base& _pj;
public:
	hsb_output(control_base&pr_root) :_pj(pr_root){}
	~hsb_output(){}
	void output_hsb(const char* afb_file);
};

