#pragma once
#include "control_fac.h"
namespace vg
{
	class AFG_EXPORT scene :
		public control_base
	{

		DEF_STRUCT_WITH_INIT(intl_pt,_sn_pt,
			(vec4, _bk_clr),
      (bool,_test_depth))
		//frame buffer
		u32 _fboId={0};
		u32 _colorTextId;
		u32 _depthStencilTextId;
		void release_resource();
	public:
		scene();
		~scene();
		void link();
        void draw();
		DECLARE_EX_INT
	};
	REG_CTL_2_FAC(scene);
}