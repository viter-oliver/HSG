#include "image_file.h"
#include "SOIL.h"
#include "res_output.h"
#include "common_functions.h"
#if defined(WITHIN_IDE)
#include <commdlg.h>
#include "resource.h"
#endif
/*
x'=(x-a)cos��+(y-b)sin��+a
y'=-(x-a)sin��+(y-b)cos��+b
*/

namespace vg
{
	image_file::image_file()
	{

	}
     bool image_file::load_image_file( char* image_path )
     {
          if (_texture_id!=0)
          {
               glDeleteTextures( 1, &_texture_id );
               _texture_id = 0;
          }
          GLubyte* imgdata = NULL;
          imgdata = SOIL_load_image( image_path, &_width, &_height, &_channels, SOIL_LOAD_RGBA );
          if (imgdata==NULL)
          {
               printf( "fail to load image from:%s\n", image_path );
               return false;
          }
          glGenTextures( 1, &_texture_id );
          glBindTexture( GL_TEXTURE_2D, _texture_id );
          glEnable( GL_BLEND );
          glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
          // Step3 �趨filter����
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
          glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, _width, _height,0, GL_RGBA, GL_UNSIGNED_BYTE, imgdata );
          SOIL_free_image_data( imgdata );
          set_size( ImVec2(_width,_height) );
          return true;
     }
	void image_file::draw()
	{
          if( _texture_id ==0)
          {
               return;
          }
          int texture_id = _texture_id;
		float sizew = _in_p._sizew;
		float sizeh = _in_p._sizeh;
		ImVec2 abpos = absolute_coordinate_of_base_pos();
		ImVec2 winpos = ImGui::GetWindowPos();
		ImVec2 pos0 = { abpos.x + winpos.x, abpos.y + winpos.y };
		ImVec2 pos1 = { pos0.x, pos0.y + sizeh };
		ImVec2 pos2 = { pos0.x + sizew, pos0.y + sizeh };
		ImVec2 pos3 = { pos0.x + sizew, pos0.y };

          ImVec2 uv0(1.f, 0.f);
		ImVec2 uv1 (1,1);
		ImVec2 uv2 (0,1);
		ImVec2 uv3 (0,0);

		float offsetx = abpos.x - base_pos().x;
		float offsety = abpos.y - base_pos().y;
		ImVec2 axisBasePos = { offsetx + _img_pt._aposx + winpos.x, offsety + _img_pt._aposy + winpos.y };
		if (_img_pt._angle_srd != 0.f)
		{
			pos0 = rotate_point_by_zaxis(pos0, _img_pt._angle_srd, axisBasePos);
			pos1 = rotate_point_by_zaxis(pos1, _img_pt._angle_srd, axisBasePos);
			pos2 = rotate_point_by_zaxis(pos2, _img_pt._angle_srd, axisBasePos);
			pos3 = rotate_point_by_zaxis(pos3, _img_pt._angle_srd, axisBasePos);
		}

		ImGui::ImageQuad((ImTextureID)texture_id, pos0, pos1, pos2, pos3, uv0, uv1, uv2, uv3);
		//ctl_base::draw();

#ifdef WITHIN_IDE
          if (is_selected())//draw envelope
		{
			ImU32 col = ImGui::GetColorU32(ImGuiCol_HeaderActive);
			ImVec2 editunit(edit_unit_len, edit_unit_len);
			ImVec2 pos1a = pos0 - editunit;
			ImVec2 pos1b = pos0 + editunit;
			ImGui::RenderFrame(pos1a, pos1b, col);
			ImVec2 pos2a = pos1 - editunit;
			ImVec2 pos2b = pos1 + editunit;
			ImGui::RenderFrame(pos2a, pos2b, col);
			ImVec2 pos3a = pos2 - editunit;
			ImVec2 pos3b = pos2 + editunit;
			ImGui::RenderFrame(pos3a, pos3b, col);
			ImVec2 pos4a = pos3 - editunit;
			ImVec2 pos4b = pos3 + editunit;
			ImGui::RenderFrame(pos4a, pos4b, col);
			ImVec2 posaa = axisBasePos - editunit;
			ImVec2 posab = axisBasePos + editunit;
			ImGui::RenderFrame(posaa, posab, col);

		}
#endif
	}
}