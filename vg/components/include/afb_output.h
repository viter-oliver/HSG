#pragma once
#include "control_common_def.h"
using namespace vg;
class afb_output
{
	control_common_def& _pj;
public:
	afb_output(control_common_def&pr_root) :_pj(pr_root){}
	~afb_output(){}
	void output_afb(const char* afb_file);
};

