#pragma once
#include "control_fac.h"
#include "packing_texture_container.h"
#define FILE_NAME_LEN 50
namespace vg
{
	class AFG_EXPORT image_play :
		public control_base
	{

		DEF_STRUCT_WITH_INIT(intl_pt, _img_pt,
			(int, _attached_txt,{0}),
			(int, _frame_index, { 0 }))
	public:
		image_play();
		~image_play(){}
		int get_cur_frame_index(){ return _img_pt._frame_index; }
		void set_frame_index(int frame_index){ _img_pt._frame_index = frame_index; }
		int get_frames_count();
		void draw();
		DECLARE_EX_INT
	};

	REG_CTL_2_FAC(image_play)
}
