#pragma once
#include "control_fac.h"
#include "file_chunk.h"
namespace vg
{
#define MAX_POS_PAIR_CNT 20
	class AFG_EXPORT polygon_image :
		public control_base
	{
		DEF_STRUCT_WITH_INIT(pg_img_pt,_img_pt,
			(char, _track_file[name_len]),
			(float, _progrss_nml, {0.}),
			(vui2, _img_txt)
			)
	public:
		void link();
		polygon_image();
		void draw();
		DECLARE_EX_INT
		DECLARE_DRAW_SEL_ANCHOR
	private:
		void read_track_file(const char* track_file);
		using v_vec2=std::vector<vec2>;
		v_vec2 _track0;
		v_vec2 _track1;
		std::vector<float> _track_segs0;
		std::vector<float> _track_segs1;
		float _seg_len0, _seg_len1;
		sd_file_chunk_unit _ps_track_file;
		bool contain(vec2& tar_pos);
	};
	REG_CTL_2_FAC(polygon_image);
}