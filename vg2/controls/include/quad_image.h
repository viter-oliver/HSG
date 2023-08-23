#pragma once
#include "control_fac.h"
namespace vg
{

	class AFG_EXPORT quad_image :
		public control_base
	{
	
		DEF_STRUCT_WITH_INIT(_img_pt,
		(vec2_3, _imge_pos),
		(vui2, _texture_index_txt))
	public:	
		quad_image();// : base(){}
		void draw();
		DECLARE_EX_INT
		DECLARE_DRAW_SEL_ANCHOR
		bool contain(vec2& tarpos) {
			auto same_s = [](float nm1, float nm2) {
				auto s = nm1 * nm2;
				return s >= 0;
			};
			auto tar_pr1 = tarpos - absolute_coordinate_of_base_pos();
			auto tar_pr2 = tarpos - _img_pt._imge_pos[0];
			auto tar_pr3 = tarpos - _img_pt._imge_pos[1];
			auto tar_pr4 = tarpos - _img_pt._imge_pos[2];
			auto tp1_x_tp2 = tar_pr1.x_ply(tar_pr2);
			auto tp2_x_tp3 = tar_pr2.x_ply(tar_pr3);
			if (!same_s(tp1_x_tp2, tp2_x_tp3)) { return false; };
			auto tp3_x_tp4 = tar_pr3.x_ply(tar_pr4);
			if (!same_s(tp2_x_tp3, tp3_x_tp4)) { return false; };
			auto tp4_x_tp1 = tar_pr4.x_ply(tar_pr1);
			if (!same_s(tp4_x_tp1, tp3_x_tp4)) { return false; };
			return true;
		}
	};

	REG_CTL_2_FAC(quad_image);
}
