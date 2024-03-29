#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "shader.h"
namespace vg
{
	class screen_image_distortion
	{
          GLuint g_VboHandle;
		GLuint _fboId;
		GLint _prev_fbo;
		GLuint _colorTextId;
		GLuint _depthStencilTextId;
          GLuint _Ul_customMtx, _Ul_customDelta;
		float _width, _height;
		GLuint _txtDistortion;
		ps_shader _pshader;
          float g_cs_a = 1.f, g_sn_a = 0.f, g_ox = 0.f, g_oy = -0.3333f;
	public:
		screen_image_distortion(float width, float height);
		~screen_image_distortion();
          void set_rotate_angle( float angle );
          void set_rotate_axis_pos( float px, float py );
		void get_size(float& width, float& height)
		{
			width = _width;
			height =_height;
		}
		void bind_framebuffer();
		void draw();
		void disbind_framebuffer();
	};

}