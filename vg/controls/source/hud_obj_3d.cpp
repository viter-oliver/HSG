#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "hud_obj_3d.h"
#include "hud_projector.h"
const char* hud_obj_shd_name = "sd_hud_obj";
const char* hud_obj_prm_name = "prm_hud_obj";
GLfloat vertices[] = {
     -0.5f, -0.5f,0.f, 0.f, 1.f,
     0.5f, -0.5f, 0.f,1.f, 1.f,
     -0.5f, 0.5f, 0.f,0.f, 0.f,
     0.5f, 0.5f,0.f, 1.f, 0.f,
};
const char* hud_sd_vs = R"glsl(
#version 300 es
precision mediump float;
layout(location=0) in vec3 position;
layout(location=1) in vec2 textCoord;
out vec2 TextCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    TextCoord = textCoord;
}
)glsl";
const char* hud_sd_fs = R"glsl(
#version 300 es
precision mediump float;
in vec2 TextCoord;
out vec4 o_clr;
uniform sampler2D text_at;
void main()
{
	o_clr = texture(text_at, TextCoord);
}
)glsl";
namespace vg
{
     ps_shader hud_obj_3d::_phud_sd =nullptr;
     ps_primrive_object hud_obj_3d::_ps_prm = nullptr;
     
     hud_obj_3d::hud_obj_3d()
     {

     }

     hud_obj_3d::~hud_obj_3d()
     {
     
     }

     void hud_obj_3d::link()
     {
          auto iat = g_dic_indep_texture.find( _pt_tb._attached_image );
          if (iat!=g_dic_indep_texture.end())
          {
               _pat_image = iat->second;
          }
          if( !hud_obj_3d::_phud_sd )
          {
               hud_obj_3d::_phud_sd = make_shared<vg_shader>( hud_sd_vs, hud_sd_fs );
               hud_obj_3d::_ps_prm = make_shared<primitive_object>();
               _ps_prm->set_ele_format( { 3, 2 } );
               _ps_prm->load_vertex_data( vertices, sizeof( vertices ) / sizeof( float ) );
          }
     }

     void hud_obj_3d::draw()
     {
          if (!_pat_image)
          {
               return;
          }
          hud_projector* p_prj = (hud_projector*)get_parent();
          vec3* pview_pos = p_prj->get_view_pos();
          * pcenter = p_prj->get_center_of_prj();
          * pup = p_prj->get_up();
          glm::vec3 cam_pos( pview_pos->x, pview_pos->y, pview_pos->z );
          glm::vec3 cam_dir( pcenter->x, pcenter->y, pcenter->z );
          glm::vec3 cam_up( pup->x, pup->y, pup->z );
          glm::mat4 view = glm::lookAt( cam_pos, cam_dir, cam_up );
          _phud_sd->use();
          _phud_sd->uniform( "view", glm::value_ptr( view ) );
          float w, h;
          p_prj->get_size( w, h );
          float aspect = w / h;
          glm::mat4 proj = glm::perspective( glm::radians( p_prj->get_fovy() ), aspect, p_prj->get_near(), p_prj->get_far() );
          _phud_sd->uniform( "projection", glm::value_ptr( proj ) );
          glm::mat4 trans;
          trans = glm::translate(
               trans,
               glm::vec3( _pt_tb._tanslation_x, _pt_tb._tanslation_y, _pt_tb._tanslation_z )
               );
          trans = glm::scale( trans, glm::vec3( _in_p._sizew, _in_p._sizeh,1 ) );
          _phud_sd->uniform( "model", glm::value_ptr( trans ) );
          glActiveTexture( GL_TEXTURE0 );
          glBindTexture( GL_TEXTURE_2D, _pat_image->_txt_id() );
          _phud_sd->uniform( "text_at", 0 );
          glBindVertexArray( _ps_prm->_vao );
          glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
     }

}