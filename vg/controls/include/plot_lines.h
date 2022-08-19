#pragma once
#include "control_fac.h"
namespace vg
{ 
    #define MAX_VALUE_COUNT 50
	class AFG_EXPORT plot_lines :
		public control_base
	{

		DEF_STRUCT_WITH_INIT(intl_pt,_pt,
			(int, _v_count, {10}),
			(float, _min, {-1.f}),
			(float, _max, {1.f}))
		float _values[MAX_VALUE_COUNT] ;
		int _algebra{ 0 };
	public:
		plot_lines();
		
		void draw();
	};
	REG_CTL_2_FAC(plot_lines);
}
