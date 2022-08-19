#pragma once
#include "control_fac.h"
namespace vg
{

	class AFG_EXPORT quad_image :
		public control_base
	{
	
		DEF_STRUCT_WITH_INIT(intl_pt,_img_pt,
			(vec2, _imge_pos[4]),
			(vi2, _texture_index_txt))
	public:	
		quad_image();// : base(){}
		
		void draw();
	};

	REG_CTL_2_FAC(quad_image);
}
