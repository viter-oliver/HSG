#include <chrono>
#include "textblock.h"
#include "common_functions.h"

namespace vg
{
	using namespace std;
	using namespace chrono;


	textblock::textblock()
	{
		_txt_area = { 0.f,0.f,0.f,0.f };
		//_pfont_res_set = make_shared<vg_font_res_set>(*g_pfont_face_manager);
		memset(_txt_pt._content, 0, MAX_CONTENT_LEN);
		_txt_pt._txt_clr = { 1.f, 1.f, 1.f,1.f};
	}
	void textblock::link()
	{
		auto& nm_list = font_manager::get_dic_fonts();
		if (_txt_pt._font_id>=nm_list.size())
		{
			_txt_pt._font_id = 0;
		}
		_pfont_unit = nm_list[_txt_pt._font_id];
	}
	void textblock::draw()
	{
		if (!_pfont_unit)
		{
			return;
		}
		ImVec2 abpos = absolute_coordinate_of_base_pos();
		ImVec2 winpos = ImGui::GetWindowPos();
		ImVec2 dpos = abpos + winpos;
		auto& nm_list = font_manager::get_dic_fonts();
		auto font_cnt = nm_list.size();
		if (font_cnt==0)
		{
			return;
		}
		if (_txt_pt._font_id>=font_cnt)
		{
			_txt_pt._font_id = 0;
		}
		//string font_name = nm_list[_txt_pt._font_id]->_name;
		float font_scale = _txt_pt._font_scale;

		const ImVec2 ctnt_size = _txt_area.Max - _txt_area.Min;
          vec2 draw_pos;
          draw_pos.x = dpos.x - ctnt_size.x*_txt_pt._txt_alignh_nml;
          draw_pos.y = dpos.y - ctnt_size.y*_txt_pt._txt_alignv_nml;
           end_pos;
		wstring draw_content = utf8ToWstring(_txt_pt._content);
		bool be_new = false;
		if (draw_content != _str_bk)
		{
			be_new = true;
			_str_bk = draw_content;
		}
		auto str_sz = draw_content.size();
		float width = screenw*2;
		if (_txt_pt._width_limit)
		{
			width = _txt_pt._width;
		}
		if (str_sz > 0)
		{
			//const GLuint max_pixel_size = 512 * 512;
			static int start_id = 0;
			static steady_clock::time_point  lastTime;
			if (be_new)
			{
				font_manager::draw_wstring(_pfont_unit, _txt_pt._font_size, 
                                                        draw_pos, end_pos,
                                                        _txt_pt._font_scale,draw_content,
                                                        _txt_pt._txt_clr,width,
                                                        _txt_pt._omit_rest, true);
                    auto new_sz = end_pos - draw_pos;
                    draw_pos.x = dpos.x - new_sz.x*_txt_pt._txt_alignh_nml;
                    draw_pos.y = dpos.y - new_sz.y*_txt_pt._txt_alignv_nml;

				start_id = 0;
				lastTime = steady_clock::now();
				
			}
			if (_txt_pt._playing)
			{
				wstring sub_content = draw_content.substr(start_id);
				auto cnt_content= sub_content.size();
				auto cnt_draw_char = font_manager::draw_wstring(_pfont_unit, _txt_pt._font_size,
                                                                             draw_pos, end_pos,
                                                                             _txt_pt._font_scale,sub_content,
                                                                             _txt_pt._txt_clr,width,
                                                                             _txt_pt._omit_rest, false);
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
				font_manager::draw_wstring(_pfont_unit, _txt_pt._font_size,
                                                        draw_pos, end_pos,
                                                        _txt_pt._font_scale, draw_content,
                                                        _txt_pt._txt_clr, width,
                                                        _txt_pt._omit_rest, false);
			}
               _txt_area.Min = { draw_pos.x, draw_pos.y };
               _txt_area.Max = { end_pos.x, end_pos.y };
		}
		//base::draw();

#if defined(WITHIN_IDE)
		if (is_selected())
		{
               ze = end_pos - draw_pos;
               ImVec2 pos1 = { draw_pos.x, draw_pos.y };
			ImVec2 pos2 = { pos1.x, pos1.y + real_size.y };
			ImVec2 pos3 = { pos1.x + real_size.x, pos1.y + real_size.y };
			ImVec2 pos4 = { pos1.x + real_size.x, pos1.y };

			ImU32 col = ImGui::GetColorU32(ImGuiCol_HeaderActive);
			ImVec2 editunit(edit_unit_len, edit_unit_len);
			ImVec2 pos1a = pos1 - editunit;
			ImVec2 pos1b = pos1 + editunit;
			ImGui::RenderFrame(pos1a, pos1b, col);
			ImVec2 pos2a = pos2 - editunit;
			ImVec2 pos2b = pos2 + editunit;
			ImGui::RenderFrame(pos2a, pos2b, col);
			ImVec2 pos3a = pos3 - editunit;
			ImVec2 pos3b = pos3 + editunit;
			ImGui::RenderFrame(pos3a, pos3b, col);
			ImVec2 pos4a = pos4 - editunit;
			ImVec2 pos4b = pos4 + editunit;
			ImGui::RenderFrame(pos4a, pos4b, col);
			
		}
#endif
	}

#if defined(WITHIN_IDE)
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