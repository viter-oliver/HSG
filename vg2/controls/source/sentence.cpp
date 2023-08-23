#include "sentence.h"

namespace vg
{
     sentence::sentence( ps_font_unit& pfont, wstring wssentence )
          :_pfont_unit(pfont),_wssentence(wssentence )
     {
          _r_bottom_edge = _l_top_edge = _endpos = { 0.f, 0.f };
     }

     sentence::~sentence()
     {
     }

     bool sentence::contains( float posx, float posy )
     {
          ImVec2 abpos = absolute_coordinate_of_base_pos();
          ImVec2 winpos = ImGui::GetWindowPos();
          ImVec2 pos0 = { abpos.x + winpos.x, abpos.y + winpos.y };
          ImVec2 mouse_pos( posx, posy );
          if( !is_same_line() )
          {
              
               ImVec2 pos_top_r( _r_bottom_edge.x, _r_bottom_edge.y );
               ImRect top_area( pos0, pos_top_r );
               if (top_area.Contains(mouse_pos))
               {
                    return true;
               }
               ImVec2 ltp( _l_top_edge.x, _l_top_edge.y );
               ImRect middle_area( ltp, pos_top_r );
               if (middle_area.Contains(mouse_pos))
               {
                    return true;
               }
             
               ImVec2 bp( _endpos.x, _endpos.y );
               ImRect bottom_area( ltp, bp );
               if (bottom_area.Contains(mouse_pos))
               {
                    return true;
               }
               return false;
          }
          else
          {
               
               ImRect sarea( pos0,ImVec2( _endpos.x,_endpos.y) );
               return sarea.Contains( mouse_pos );
          }
     }

     void sentence::draw()
     {
          ImVec2 abpos = absolute_coordinate_of_base_pos();
          ImVec2 winpos = ImGui::GetWindowPos();
          ImVec2 dpos = abpos + winpos;
           draw_pos = { dpos.x, dpos.y };
          font_manager::draw_wstring( _pfont_unit, _font_size,
                                              draw_pos, _endpos,
                                              1.f, _wssentence,
                                              _txt_clr, _l_top_edge,
                                              _r_bottom_edge,line_spacing, false );
     }

     bool sentence::is_head_of_a_paragraph()
     {
          if (_wssentence.size()>0)
          {
               auto& ch = _wssentence[ 0 ];
               if( _wssentence[ 0 ] == 0xd)
               {
                    return true;
               }
          }
          return false;
     }

     bool sentence::is_same_line()
     {
          return _r_bottom_edge.y < 0.1f;
     }

}