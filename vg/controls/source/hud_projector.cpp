#include "hud_projector.h"
#include "common_functions.h"
namespace vg
{
     hud_projector::hud_projector()
     {
          size()={640,480};
          _pj_pt._view_pos = { -41.f, 130.f, -257.f };
          _pj_pt._center_of_prj = { -41.f, 90.f, 542.f };
          _pj_pt._up = { 0, 1.f, 0 };
          _pj_pt._fovy = 3;
          _pj_pt._near = 20;
          _pj_pt._far = 100000.f;
          _pj_pt._test_depth = false;

     }
     void hud_projector::release_resource()
     {
          if( _colorTextId )
               glDeleteTextures( 1, &_colorTextId );
          if( _depthStencilTextId )
               glDeleteTextures( 1, &_depthStencilTextId );
          if( _fboId )
          {
               glDeleteFramebuffers( 1, &_fboId );
               _fboId = 0;
          }
     }
     hud_projector::~hud_projector()
     {
          release_resource();
     }
     void hud_projector::link()
     {
          prepareFBO1( _colorTextId, _depthStencilTextId, _fboId, _in_p._sizew, _in_p._sizeh );

     }
     void hud_projector::draw()
     {
          if( !_fboId )
          {
               return;
          }
          GLint last_viewport[ 4 ];
          glGetIntegerv( GL_VIEWPORT, last_viewport );
          GLint last_fmid;
          glGetIntegerv( GL_FRAMEBUFFER_BINDING, &last_fmid );
          glBindFramebuffer( GL_FRAMEBUFFER, _fboId );
          glViewport( 0, 0, size().x, size().y );
          glEnable( GL_BLEND );
          glBlendEquation( GL_FUNC_ADD );
          glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
          //glEnable(GL_DEPTH_TEST);
          //glDepthFunc(GL_LESS);
          //glDisable( GL_CULL_FACE );
          if( _pj_pt._test_depth )
          {
               glEnable( GL_DEPTH_TEST );
          }
          else
          {
               glDisable( GL_DEPTH_TEST );
          }
          //glDisable(GL_SCISSOR_TEST);
          glClearColor( _pj_pt._bk_clr.x, _pj_pt._bk_clr.y, _pj_pt._bk_clr.z, _pj_pt._bk_clr.w );
          glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
          //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          for( auto it : _vchilds )
          {
               if( it->visibility() )
               {
                    it->draw();
               }
          }
          glBindFramebuffer( GL_FRAMEBUFFER, last_fmid );
          glViewport( last_viewport[ 0 ], last_viewport[ 1 ], (GLsizei)last_viewport[ 2 ], (GLsizei)last_viewport[ 3 ] );
          auto abpos = absolute_coordinate_of_base_pos();
          ImVec2 draw_sz={size().x,size().y};
          auto cur_cursor_pos = ImGui::GetCursorPos();
          ImGui::SetCursorPos(ImVec2(abpos.x, abpos.y));
          ImGui::Image((ImTextureID)_colorTextId,draw_sz,ImVec2(1,1),ImVec2(0,0));
     }

}

