#include "listbox_ex.h"
#include "bind.h"
#include "user_control_imgui.h"
namespace vg
{

     listbox_ex::listbox_ex()
     {
         size() = { 100,200 };
     }

     void listbox_ex::draw()
     {
          ImVec2 apos = absolute_coordinate_of_base_pos();
#if defined(WITHIN_IDE)
          if( !is_selected())
          {
               auto offset = ImGui::GetCursorScreenPos();
               auto cur_pos = apos + offset;
               ImGui::SetCursorScreenPos( cur_pos );
               ImGui::InvisibleButton( _in_p._name, ImVec2( _in_p._sizew, _in_p._sizeh ) );
               if( ImGui::IsItemActive() && ImGui::IsMouseDragging() )
               {
                    auto ms_delta = ImGui::GetIO().MouseDelta;
                    if( _pt._vertical )
                    {
                         _pt._inner_top += ms_delta.y;
                    }
                    else
                    {
                         _pt._inner_left += ms_delta.x;

                    }
               }
          }

#endif
          ImVec2 winpos = ImGui::GetWindowPos();
          auto sizew = _in_p._sizew;
          auto sizeh = _in_p._sizeh;
          ImVec2 pos0 = { apos.x + winpos.x, apos.y + winpos.y };
          ImVec2 pos1 = { pos0.x, pos0.y + sizeh };
          ImVec2 pos2 = { pos0.x + sizew, pos0.y + sizeh };
          ImVec2 pos3 = { pos0.x + sizew, pos0.y };
          ImGui::PushClipRect( pos0, pos2, true );
          if( _pt._vertical )
          {
               float spos_y = _pt._inner_top;
               for( auto item : _vchilds )
               {
                    auto bpos = item->base_pos();
                    float w, h;
                    item->get_size( w, h );
                    if( bpos.y != spos_y )
                    {
                         item->set_base_posy( spos_y );
                         prop_ele_position pep { item, 0, 1 };
                         calcu_bind_node( pep );
                    }
                    spos_y += h;
                    spos_y += _pt._interval;
                    item->draw_frames();
               }
               _pt._inner_bottom = spos_y;
          }
          else
          {
               float spos_x = _pt._inner_left;
               for( auto item : _vchilds )
               {
                    auto bpos = item->base_pos();
                    float w, h;
                    item->get_size( w, h );
                    if( bpos.x != spos_x )
                    {
                         item->set_base_posy( spos_x );
                         prop_ele_position pep { item, 0, 0 };
                         calcu_bind_node( pep );
                    }
                    spos_x += w;
                    spos_x += _pt._interval;
                    item->draw_frames();
               }
               _pt._inner_right = spos_x;
          }
          ImGui::PopClipRect();

     }
}