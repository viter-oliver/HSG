#pragma once
#include "control_fac.h"
#include "file_chunk.h"
namespace vg
{
	class AFG_EXPORT slider :
		public control_base
	{

		DEF_STRUCT_WITH_INIT(intl_pt, _slider_pt,
			(float, _bg_txtw, { 20.f }),
			(float, _bg_txth, { 20.f }),
      (char, _track_path_text[ FILE_NAME_LEN ] ),
			(float, _hd_txtw, {20.f}),
			(float, _hd_txth, { 20.f }),
			(float, _tb_txtw, { 20.f }),
			(float, _tb_txth, { 20.f }),
			(float, _progress_nml, { 0.f }),
			(float, _bg_aposx, {0.f}),
			(float, _bg_aposy, {0.f}),
			(float, _bg_angle_nml, {0.f}),
			(vi2, _texture_bg_index_txt),
			(float, _hd_posx, { 20.f }),
			(float, _hd_posy, { 20.f }),
			(vi2, _texture_head_index_txt),
			(float, _tb_height, { 20.f }),
			(float, _tb_offset, { 0.f }),
			(bool, _thumb_visible, {false}),
			(vi2, _texture_thumb_index_txt))
	public:
		slider();
		~slider(){}
		void link();
		void set_progress(float value){ _slider_pt._progress_nml = value; }
		float get_progress(){ return _slider_pt._progress_nml; }

		void draw();
	private:
		bool read_point_position_file(const char *str);
		//point_pair_vec _custom_envelope;
		sd_file_chunk_unit _ps_track_file;
		using v_vec2=std::vector<vec2>;
		v_vec2 _custom_track0;
		v_vec2 _custom_track1;
		v_vec2 _custom_mid_track;
		std::vector<float> _custom_track_segment;
		float _custom_trace_length{ 0.f };
	};

	REG_CTL_2_FAC(slider);
}
