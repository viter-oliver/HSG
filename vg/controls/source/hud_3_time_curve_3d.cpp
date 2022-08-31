#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "hud_3_time_curve_3d.h"
#include "hud_projector.h"
const char* hud_4_curve_shd_name = "sd_hud_4_curve";
const char* hud_4_curve_prm_name = "prm_hud_4_curve";
const char* hud_sd_4_curve_vs = R"glsl(
#version 300 es
precision mediump float;
layout(location=0) in vec3 position;
layout(location=1) in vec2 textCoord;
out vec2 TextCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float c[4];
uniform float w;
uniform int lectl_boder;
void main()
{
    vec3 pos=position;
    float posx0=pos.x;
    float z=pos.z*0.01;
    const float fs=100.0;
    if(lectl_boder>0)
    {
         if(posx0<0.1)
         {
              pos.x=(c[3]*z*z*z+c[2]*z*z+c[1]*z+c[0])*fs-w;
         }
         else
         {
              pos.x=(c[3]*z*z*z+c[2]*z*z+c[1]*z+c[0])*fs;
         }
    }
    else
    {
         if(posx0<0.1)
         {
              pos.x=(c[3]*z*z*z+c[2]*z*z+c[1]*z+c[0])*fs;
         }
         else
         {
              pos.x=(c[3]*z*z*z+c[2]*z*z+c[1]*z+c[0])*fs+w;
         }

    } 
    gl_Position = projection * view * model * vec4(pos, 1.0);
    TextCoord = textCoord;
}
)glsl";
const char* hud_sd_4_curve_fs = R"glsl(
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
    shader::sd_shader_base hud_3_time_curve_3d::_phud_sd = nullptr;
    primitive::sd_primitive_unit hud_3_time_curve_3d::_ps_prm = nullptr;
    using namespace std;
    using namespace shader;
    using namespace primitive;
     hud_3_time_curve_3d::hud_3_time_curve_3d()
     {
        _pt_tb._coeff_hac[ 0 ] = _pt_tb._coeff_hac[ 1 ] = _pt_tb._coeff_hac[ 2 ] = _pt_tb._coeff_hac[ 3 ] = 0.f;
     }

     hud_3_time_curve_3d::~hud_3_time_curve_3d()
     {

     }
     const int curve_len = 100;
     const float unit_len = 100.f;// 1000.f;
     const int point_cnt = curve_len * 2 + 2;

     void hud_3_time_curve_3d::link()
     {
        
          if( !hud_3_time_curve_3d::_phud_sd )
          {
              hud_3_time_curve_3d::_phud_sd = make_shared<shader_base>();
              hud_3_time_curve_3d::_phud_sd->build_vs_code(hud_sd_4_curve_vs);
              hud_3_time_curve_3d::_phud_sd->build_fs_code(hud_sd_4_curve_fs);
              hud_3_time_curve_3d::_phud_sd->link();
               hud_3_time_curve_3d::_ps_prm = make_shared<primitive_unit>();
               int demension = 5;
               auto data_cnt = point_cnt*demension;
               GLfloat* vertices = new GLfloat[ data_cnt ];
               float uv_unit = 1.f / (float)curve_len;
               for( int ix = 0; ix < curve_len + 1; ++ix )
               {
                    auto base_id = ix * 2 * demension;
                    vertices[ base_id ] = 0;//x->
                    vertices[ base_id + 1 ] = 0;//y->
                    vertices[ base_id + 2 ] = ix*unit_len;//z->
                    vertices[ base_id + 3 ] = 0;
                    vertices[ base_id + 4 ] = uv_unit * ix;

                    vertices[ base_id + 5 ] = 1;//x->
                    vertices[ base_id + 6 ] = 0;//y->
                    vertices[ base_id + 7 ] = ix*unit_len;//z->
                    vertices[ base_id + 8 ] = 1;
                    vertices[ base_id + 9 ] = uv_unit * ix;
               }
               hud_3_time_curve_3d::_ps_prm->load_vertex_data( vertices, data_cnt );
               delete[] vertices;
          }
     }

     void hud_3_time_curve_3d::draw()
     {

          hud_projector* p_prj = (hud_projector*)get_parent();
          vec3& pview_pos = p_prj->get_view_pos();
          vec3& pcenter = p_prj->get_center_of_prj();
          vec3& pup = p_prj->get_up();
          glm::vec3 cam_pos( pview_pos.x, pview_pos.y, pview_pos.z );
          glm::vec3 cam_dir( pcenter.x, pcenter.y, pcenter.z );
          glm::vec3 cam_up( pup.x, pup.y, pup.z );
          glm::mat4 view = glm::lookAt( cam_pos, cam_dir, cam_up );
          _phud_sd->use();
          _phud_sd->uniform( "view", glm::value_ptr( view ) );
          auto& pr_sz = p_prj->size();
          float aspect = pr_sz.x/ pr_sz.y;
          glm::mat4 proj = glm::perspective( glm::radians( p_prj->get_fovy() ), aspect, p_prj->get_near(), p_prj->get_far() );
          _phud_sd->uniform( "projection", glm::value_ptr( proj ) );
          glm::mat4 trans;
          trans = glm::translate(
               trans,
               glm::vec3( _pt_tb._tanslation_x, _pt_tb._tanslation_y, _pt_tb._tanslation_z )
               );
          _phud_sd->uniform( "model", glm::value_ptr( trans ) );
          _phud_sd->uniform( "c[0]", _pt_tb._coeff_hac );
          int ilectl_border = _pt_tb._lectl_border;
          _phud_sd->uniform( "lectl_boder", &ilectl_border );
          _phud_sd->uniform( "w", &_pt_tb._width );
          glActiveTexture( GL_TEXTURE0 );
          glBindTexture( GL_TEXTURE_2D, _pat_image->_txt_id() );
          _phud_sd->uniform( "text_at", 0 );
          glBindVertexArray( _ps_prm->_vao );
          glDrawArrays( GL_TRIANGLE_STRIP, 0, point_cnt );
     }
}