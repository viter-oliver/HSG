#include "ctl_scene.h"
#include "common_functions.h"
namespace vg
{
	ctl_scene::ctl_scene()
		:ctl_base()
		, _fboId(0), _colorTextId(0), _depthStencilTextId(0)
	{
		_in_p._sizew = 640;
		_in_p._sizeh = 480;
		_sn_pt._bk_clr = { 0.2f, 0.2f, 0.5f, 0.5f };
#if defined(WITHIN_IDE)
		reg_property_handle(&_sn_pt, 0, [this](void* memb_adress)
		{
			ImGui::ColorEdit4("Color of background", (float*)&_sn_pt._bk_clr, ImGuiColorEditFlags_RGB);
			if (!_fboId)
			{
				if (ImGui::Button("Create frame buffer"))
				{
					prepareFBO1(_colorTextId, _depthStencilTextId, _fboId, _in_p._sizew, _in_p._sizeh);
				}
			}
			else
			{
				if (ImGui::Button("Recreate frame buffer"))
				{
					release_resource();
					prepareFBO1(_colorTextId, _depthStencilTextId, _fboId, _in_p._sizew, _in_p._sizeh);
				}
			}
		});
#endif
	}
	void ctl_scene::release_resource()
	{
		if(_colorTextId)
			glDeleteTextures(1, &_colorTextId);
		if(_depthStencilTextId)
			glDeleteTextures(1, &_depthStencilTextId);
		if (_fboId)
		{
			glDeleteFramebuffers(1, &_fboId);
			_fboId=0;
		}
	}
	ctl_scene::~ctl_scene()
	{
		release_resource();
	}
	void ctl_scene::link()
	{
		prepareFBO1(_colorTextId, _depthStencilTextId, _fboId, _in_p._sizew, _in_p._sizeh);

	}

     void ctl_scene::draw_frames()
	{
		if (!_fboId)
		{
			return;
		}
		GLint last_viewport[4];
		glGetIntegerv(GL_VIEWPORT, last_viewport);
		GLint last_fmid;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last_fmid);
		glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
		glViewport(0, 0, _in_p._sizew, _in_p._sizeh);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
          //glDisable( GL_CULL_FACE );
          if (_sn_pt._test_depth)
          {
               glEnable( GL_DEPTH_TEST );
          }
          else
          {
               glDisable( GL_DEPTH_TEST );
          }
		//glDisable(GL_SCISSOR_TEST);
		glClearColor(_sn_pt._bk_clr.x, _sn_pt._bk_clr.y, _sn_pt._bk_clr.z, _sn_pt._bk_clr.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          for( auto it : _vchilds )
          {
               if( it->is_visible() )
               {
                    it->draw_frames();
               }
          }
		glBindFramebuffer(GL_FRAMEBUFFER, last_fmid);
		glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
		ImVec2 abpos = absolute_coordinate_of_base_pos();
		ImVec2 winpos = ImGui::GetWindowPos();
		float sizew = _in_p._sizew;
		float sizeh = _in_p._sizeh;
		ImVec2 pos1 = { abpos.x + winpos.x, abpos.y + winpos.y };
		ImVec2 pos2 = { pos1.x, pos1.y + sizeh };
		ImVec2 pos3 = { pos1.x + sizew, pos1.y + sizeh };
		ImVec2 pos4 = { pos1.x + sizew, pos1.y };
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, _colorTextId);
		//ImGui::ImageQuad((ImTextureID)_colorTextId, pos1, pos2, pos3, pos4, ImVec2(0, 0), ImVec2(0, 1), ImVec2(1, 1), ImVec2(1, 0));
		ImGui::ImageQuad((ImTextureID)_colorTextId, pos1, pos2, pos3, pos4, ImVec2(1, 1), ImVec2(1, 0), ImVec2(0, 0), ImVec2(0, 1));
#if defined(WITHIN_IDE)
		if (is_selected())//draw envelope
		{
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

	bool ctl_scene::handle_mouse()
	{
		return true;
	}
}