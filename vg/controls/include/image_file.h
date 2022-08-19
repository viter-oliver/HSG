#pragma once
#include "control_fac.h"
namespace vg
{

	class AFG_EXPORT image_file :
		public control_base
	{
	
          DEF_STRUCT_WITH_INIT( intl_pt, _img_pt,
                                ( float, _aposx, { 0.f } ),
                                ( float, _aposy, { 0.f } ),
                                ( float, _angle_srd, { 0.f } ) )
          u32 _texture_id = { 0 };
          int _width, _height, _channels;

	public:	
		image_file();// : ctl_base(){}
		
    bool load_image_file( char* image_path );
		u32 get_texture_id()
		{
      return _texture_id;
		}
		void draw();
		void rotate(float angle){ _img_pt._angle_srd = angle; }
	};

	REG_CTL_2_FAC(image_file);
}