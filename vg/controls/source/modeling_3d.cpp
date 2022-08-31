#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "modeling_3d.h"
#include "model.h"
#include "primitive_container.h"
#include "transform.h"
#include "shader_source_code.h"
namespace vg
{
    using namespace std;
     modeling_3d::modeling_3d()
     {
          memset( _pty_page._model_name, 0, FILE_NAME_LEN );
         
          _pty_page._cam = { { 0.f, 0.f, 3.f }, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f } };
          _pty_page._pj = { 60.f, 0.1f, 100.f };
         // _pty_page._trans = { { 0.15f, 0.15f, 0.15f }, { 0, 0, 0 }, { 0, 0, 0 }, true };
          _pty_page._light_ambient_clr = { 0.2f, 0.2f, 0.2f };
          _pty_page._light_diffuse_clr = { 0.5f, 0.5f, 0.5f };
          _pty_page._light_specular_clr = { 1.f, 1.f, 1.f };
          _pty_page._draw_mode = en_gl_triangles;
          //_pty_page._light_position_shd = { -1.f, 1.f, -2.f };
     }



     void modeling_3d::link()
     {
          auto imodel = model::g_mmodel_list.find( _pty_page._model_name );
          if( imodel != model::g_mmodel_list.end() )
          {
               _pmodel = imodel->second;
          }
          _pdiffuse=packing_texture::_v_sd_packing_texture[_pty_page._diffuse_txt];
          _pspecular=packing_texture::_v_sd_packing_texture[_pty_page._specular_txt];
          

       _pshd_modeling->build_fs_code(modeling_vs);
       _pshd_modeling->build_fs_code(modeling_fs);
       _pshd_modeling->link();
               
     }
	void modeling_3d::draw()
	{
		if (!_pmodel)
		{
			return;
		}
		auto& my_model = *_pmodel;
		auto& my_shader =*_pshd_modeling;
		my_shader.use();
		glm::mat4 model;
          for( auto& item : _vchilds )
          {
               transform* ptrans_item = (transform*)item.get();
               ptrans_item->apply( model );
          }
   

		glm::mat4 view;
		const auto& cam_pos = _pty_page._cam._position;
		const auto& cam_dir = _pty_page._cam._direction;
		const auto& cam_up = _pty_page._cam._up;
		glm::vec3 gcam_pos(cam_pos.x, cam_pos.y, cam_pos.z);
		glm::vec3 gcam_dir(cam_dir.x, cam_dir.y, cam_dir.z);
		glm::vec3 gcam_up(cam_up.x, cam_up.y, cam_up.z);
		view = glm::lookAt(gcam_pos, gcam_dir, gcam_up);
		glm::mat4 proj;
		const auto& cproj = _pty_page._pj;
		auto pparent = get_parent();

		float aspect = size().x / size().y;
		proj = glm::perspective(glm::radians(cproj._fovy), aspect, cproj._near, cproj._far);
		my_shader.uniform("viewPos", glm::value_ptr(gcam_pos));
		my_shader.uniform("model", glm::value_ptr(model));
		my_shader.uniform("view", glm::value_ptr(view));
		my_shader.uniform("projection", glm::value_ptr(proj));
		my_shader.uniform("light_ambient", (float*)&_pty_page._light_ambient_clr);
		my_shader.uniform("light_diffuse", (float*)&_pty_page._light_diffuse_clr);
		my_shader.uniform("light_specular", (float*)&_pty_page._light_specular_clr);
		my_shader.uniform("light_position", (float*)&_pty_page._light_posx);
		my_shader.uniform("light_constant", &_pty_page._light_constant_hac);
		my_shader.uniform("light_linear", &_pty_page._light_linear_hac);
		my_shader.uniform("light_quadratic", &_pty_page._light_quadratic_hac);

		for (auto& amesh:my_model)
		{
			
			const auto& primid=*amesh._ps_prm_id;
			
               int ix = 0, itx_cnt = 0;
               if( _pdiffuse )
               {
                    glActiveTexture( GL_TEXTURE0 + itx_cnt );
                    glBindTexture( GL_TEXTURE_2D, _pdiffuse->txt_id );

                    my_shader.uniform( "texture_diffuse0", itx_cnt++ );
               }
               if( _pspecular )
               {
                    glActiveTexture( GL_TEXTURE0 + itx_cnt );
                    glBindTexture( GL_TEXTURE_2D, _pspecular->txt_id );

                    my_shader.uniform( "texture_specular0", itx_cnt );
               }
               if( 0==itx_cnt )
               {
                 auto& ps_diffuse_list = amesh._ps_text_diffuse_list;
			     auto& ps_specular_list = amesh._ps_text_specular_list;
				 /*
			     if (ps_diffuse_list.size()==0)
			     {
				     auto& diffuse_list = amesh._text_diffuse_list;
				     for (auto& diff:diffuse_list)
				     {
					     ps_diffuse_list.emplace_back();
					     auto& txt_diff = ps_diffuse_list.back();
					const auto& itxt = g_dic_indep_texture.find(diff);
					     if (itxt!=g_dic_indep_texture.end())
					     {
						     txt_diff = itxt->second;
					     }
				     }
				
				     //auto& height_list = amesh._text_height_list;
				     //auto& ambient_list = amesh._text_ambient_list;

			     }
			     if (ps_specular_list.size() == 0)
			     {
				     auto& specular_list = amesh._text_specular_list;
				     for (auto& spec:specular_list)
				     {
					     ps_specular_list.emplace_back();
					     auto& txt_spec = ps_specular_list.back();
					const auto& itxt = g_dic_indep_texture.find(spec);
					     if (itxt != g_dic_indep_texture.end())
					     {
						     txt_spec = itxt->second;
					     }
				     }
			     }*/
			     //auto& height_list = amesh._text_height_list;
			     //auto& ambient_list = amesh._text_ambient_list;
			     for (auto& diff:ps_diffuse_list)
			     {
				     auto& txt = *diff;
				     glActiveTexture(GL_TEXTURE0 + itx_cnt);
				     glBindTexture(GL_TEXTURE_2D, txt.txt_id);
				     stringstream stm;
				     stm << "texture_diffuse" << ix++;
				     my_shader.uniform(stm.str(), itx_cnt++);
			     }
			     ix = 0;
			     for (auto& specular : ps_specular_list)
			     {
				     auto& txt = *specular;
				     glActiveTexture(GL_TEXTURE0 + itx_cnt);
				     glBindTexture(GL_TEXTURE_2D, txt.txt_id);
				     stringstream stm;
				     stm << "texture_specular" << ix++;
				     my_shader.uniform(stm.str(), itx_cnt++);
			     }

               }
           GLuint& dml = draw_model[ _pty_page._draw_mode ];
			glBindVertexArray(primid._vao);
               glDrawElements( dml, primid._ele_buf_len, GL_UNSIGNED_INT, 0 );
		}
	}

}