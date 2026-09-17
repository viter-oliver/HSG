#include <chrono>
#include "font_manager.h"
#include "textblock.hpp"
#include "common_functions.h"
#include "user_control_imgui.h"
#include "imgui_internal.h"
namespace vg
{
	using namespace std;
	using namespace chrono;


	textblock::textblock()
	{
		_txt_area = { 0.f,0.f };
		//_pfont_res_set = make_shared<vg_font_res_set>(*g_pfont_face_manager);
		//_txt_pt._content.value[0] = 0;
		//memset(&_txt_pt._font_key,0,1);

		_txt_pt._txt_clr = { 1.f, 1.f, 1.f,1.f};
	}
	
	void textblock::draw()
	{
		auto dpos = get_draw_pos();
		/*
		float font_scale = _txt_pt._font_scale;

		auto base_draw_pos = get_draw_pos();
		imvec2 pos0 = to_imvec2(base_draw_pos);
		imvec2 pos1 = { pos0.x, pos0.y + size().y };
		imvec2 pos2 = { pos1.x + size().x, pos1.y };
		imvec2 pos3 = { pos2.x, pos0.y };
		*/
		
		auto ifu = font_unit_map_.find(_txt_pt._font_key);
		if (ifu == font_unit_map_.end()) return;

		_pfont_unit = ifu->second;
		wstring draw_content = utf8ToWstring(_txt_pt._content.value);

		const ImVec2 ctnt_size = 
			ImVec2{ _txt_area.right_bottom.x() - _txt_area.left_top.x(),_txt_area.right_bottom.y() - _txt_area.left_top.y()};
          vec2 draw_pos;
          draw_pos.x() = dpos.x() - ctnt_size.x * _txt_pt._txt_alignh_nml;
          draw_pos.y() = dpos.y() - ctnt_size.y * _txt_pt._txt_alignv_nml;
          vec2 end_pos;

		//wstring draw_content = utf8ToWstring(content());
		bool be_new = false;
		if (draw_content != _str_bk)
		{
			be_new = true;
			_str_bk = draw_content;
		}
		float width = _txt_pt._width;
		if (_txt_pt._width_limit)
		{
			width = _txt_pt._width;
		}
		
		auto str_sz = draw_content.size();

		if (str_sz > 0)
		{
			//const GLuint max_pixel_size = 512 * 512;
			static int start_id = 0;
			static steady_clock::time_point  lastTime;
			if (be_new)
			{
				font_manager::draw_wstring(*_pfont_unit, (FT_Long)_txt_pt._font_size,
                                                        draw_pos, end_pos,
                                                        _txt_pt._font_scale,draw_content,
                                                        _txt_pt._txt_clr,width,
														_txt_pt._omit_rest,
														_txt_pt._font_line_offset,
														false);
                    auto new_sz = end_pos - draw_pos;
                    draw_pos.x() = dpos.x() - new_sz.x() * _txt_pt._txt_alignh_nml;
                    draw_pos.y() = dpos.y() - new_sz.y() * _txt_pt._txt_alignv_nml;

				start_id = 0;
				lastTime = steady_clock::now();
				
			}
			if (_txt_pt._playing)
			{
				wstring sub_content = draw_content.substr(start_id);
				auto cnt_content= sub_content.size();
				auto cnt_draw_char = font_manager::draw_wstring(*_pfont_unit, _txt_pt._font_size,
                                                                             draw_pos, end_pos,
                                                                             _txt_pt._font_scale,sub_content,
                                                                             _txt_pt._txt_clr,width,
																			 _txt_pt._omit_rest,
																			 _txt_pt._font_line_offset,
																			 false);
				auto currentTime = steady_clock::now();
				int delta = chrono::duration_cast<chrono::duration<int, std::milli>>(currentTime - lastTime).count();
				auto circle_time = delta * 0.001f;
				if (circle_time>_txt_pt._play_circle)
				{
					start_id++;
					if (cnt_content == cnt_draw_char)
						start_id = 0;
					lastTime = currentTime;
				}
			}
			else
			{
				font_manager::draw_wstring(*_pfont_unit, _txt_pt._font_size,
                                                        draw_pos, end_pos,
                                                        _txt_pt._font_scale, draw_content,
                                                        _txt_pt._txt_clr, width,
                                                        _txt_pt._omit_rest,
														_txt_pt._font_line_offset,
														false);
			}
			
			//gray@2024.09.23 �ȼ�ʵ�ֹ��ܣ��������Ż�
			//�ⲿ������Ŀǰ��������contain��draw_sel_anchor
			auto cur_size = end_pos - draw_pos;
			auto left_bottle = absolute_coordinate_of_base_pos();

			vec2 left_up = { left_bottle.x(),left_bottle.y() - _txt_pt._font_line_offset};
			
			vec2 right_bottom
			{ 
				(left_up.x() + cur_size.x()) ,
				(left_up.y() + cur_size.y())
			};

			_txt_area.left_top = left_up;
			_txt_area.right_bottom = right_bottom;

            
		}
		//internal_image("null", pos0, pos1, pos2, pos3);

		//base::draw();
		
		
//#endif
	}

#if 0
     void textblock::draw_outline()
     {
          ImVec2 abpos = absolute_coordinate_of_base_pos();
          ImVec2 winpos = ImGui::GetWindowPos();
          ImVec2 dpos = abpos + winpos;
          auto real_size = _txt_area.Max - _txt_area.Min;

          ImVec2 draw_pos;
          draw_pos.x = dpos.x - real_size.x*_txt_pt._txt_alignh_nml;
          draw_pos.y = dpos.y - real_size.y*_txt_pt._txt_alignv_nml;
          ImVec2 pos[ 4 ];
          pos[ 0 ] = draw_pos;
          pos[ 1 ] = { pos[0].x, pos[0].y + real_size.y };
          pos[ 2 ] = { pos[0].x + real_size.x, pos[0].y + real_size.y };
          pos[ 3 ] = { pos[0].x + real_size.x, pos[0].y };
          ImDrawList* draw_list = ImGui::GetWindowDrawList();
          ImU32 col = ImGui::ColorConvertFloat4ToU32( ImVec4( 0.7, 0.7, 0.7, 1.f ) );
          float thickness = 1.f;
          if( _selected )
          {
               col = ImGui::ColorConvertFloat4ToU32( ImVec4( 1, 0, 0, 1 ) );
          }
          if (!is_visible())
          {
               col = ImGui::ColorConvertFloat4ToU32( ImVec4( 0.5, 0.5, 0.5, 0.4 ) );
          }
          draw_list->AddPolyline( pos, 4, col, true, thickness );
          for( auto& bc : _vchilds )
          {
               bc->draw_outline();
          }
     }
#endif
 

}