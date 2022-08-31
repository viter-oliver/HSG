#pragma once
#include "control_fac.h"
#include "material.h"
#include "primitive.h"
namespace vg
{
	class AFG_EXPORT material_3d :
		public control_base
	{

		DEF_STRUCT_WITH_INIT(intl_pt, _pt,
			(int, _draw_mode, {en_gl_triangles}),
			(char, _primitive_name[FILE_NAME_LEN]),
			(char, _material_name[FILE_NAME_LEN]),
			(char, _uf_model_name[FILE_NAME_LEN]),
			(char, _uf_view_name[FILE_NAME_LEN]),
			(char, _uf_proj_name[FILE_NAME_LEN]),
			(camera,_cam),
			(projection,_pj),
		  (bool, _with_feedback, {false}))
		bool _matched{ false };
    bool _same_sclae { false };
		primitive::sd_primitive_unit _ps_prm;
		shader::sd_material _ps_mtl;
		u32 _gpu_outbuff = { 0 };
		std::vector<float> _output_buff;
	public:
		material_3d();
		~material_3d();
		void link();
		void draw();
		DECLARE_EX_INT
		material_3d& set_cam_posx(float posx)
		{
			_pt._cam._position.x = posx;
			return *this;
		}
		material_3d& set_cam_posy(float posy)
		{
			_pt._cam._position.y = posy;
			return *this;
		}
		material_3d& set_cam_posz(float posz)
		{
			_pt._cam._position.z = posz;
			return *this;
		}
		
		material_3d& set_cam_dirx(float dirx)
		{
			_pt._cam._direction.x = dirx;
			return *this;
		}
		material_3d& set_cam_diry(float diry)
		{
			_pt._cam._direction.y = diry;
			return *this;
		}
		material_3d& set_cam_dirz(float dirz)
		{
			_pt._cam._direction.z = dirz;
			return *this;
		}

		material_3d& set_cam_upx(float upx)
		{
			_pt._cam._up.x = upx;
			return *this;
		}
		material_3d& set_cam_upy(float upy)
		{
			_pt._cam._up.y = upy;
			return *this;
		}
		material_3d& set_cam_upz(float upz)
		{
			_pt._cam._up.z = upz;
			return *this;
		}

		material_3d& set_pj_fovy(float fovy)
		{
			_pt._pj._fovy = fovy;
			return *this;
		}
		material_3d& set_pj_near(float fnear)
		{
			_pt._pj._near = fnear;
			return *this;
		}
		material_3d& set_pj_far(float ffar)
		{
			_pt._pj._far = ffar;
			return *this;
		}

		
		bool get_output_vertex(std::vector<float>& overtex);

	};
	REG_CTL_2_FAC(material_3d);
}
