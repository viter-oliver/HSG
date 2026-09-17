#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <algorithm>
#include "common_functions.h"
#include "scene.hpp"
#include "user_control_imgui.h"
namespace vg {
     using namespace std;
     scene::scene() {
       size() = {1500,1500};
        _pt._view_pos = { 0.f, 206.f, -600.f };
        _pt._center_of_prj = { 0.f, 90.f, 0.f };
        _pt._up = { 0, 1.f, 0 };
        _pt._background_clr = {0.2f, 0.2f, 0.2f, 1.f};
        _pt._light_position = { 0.f,2.46f,-0.97f };
        _pt._light_direction = { 0.f, -0.80f, -0.60 };
        _pt._light_ambient_clr = { 0.17, 0.17, 0.17 };
        _pt._light_diffuse_clr = { 1, 1, 1 };
        _pt._light_specular_clr = { 1, 1, 1 };
        prepareFBO_ex(_texture_id, _FBO_id, _RBO_id, size().x(),
                    size().y());
     }
    void scene::release_resource() {
      if (_texture_id)
        glDeleteTextures(1, &_texture_id);
      if( _RBO_id )
            glDeleteRenderbuffers( 1, &_RBO_id );
      if( _FBO_id )
      {
            glDeleteFramebuffers( 1, &_FBO_id );
      }
    }
     scene::~scene(){
          release_resource();
     }
   void scene::calculate_properties( m_sd_bind_var& child_properties ) {
 #define EXPVEC3(v3) v3.x(),v3.y(),v3.z()
#if 0
          glm::vec3 cam_pos(EXPVEC3(_pt._view_pos));
          auto iview = child_properties.find("view");
          if (iview != child_properties.end()) {
               glm::vec3 cam_dir(EXPVEC3(_pt._center_of_prj));
               glm::vec3 cam_up(EXPVEC3(_pt._up));
               glm::mat4 view = glm::lookAt(cam_pos, cam_dir, cam_up);
               auto s_bind_v = iview->second;
               auto s_bind_v_16 = dynamic_pointer_cast<f32_16>(s_bind_v);
               if (s_bind_v_16) {
                 auto &bind_v_16 = *s_bind_v_16;
                 auto pview = glm::value_ptr(view);
                 auto copy_size = sizeof(float) * 16;
                 memcpy_s(bind_v_16.value, copy_size, pview, copy_size);
                 child_properties.erase(iview);
               }
          }
          auto iviewPos = child_properties.find("viewPos");
          if (iviewPos != child_properties.end()) {
               auto s_bind_v = iviewPos->second;
               auto s_bind_f3 = dynamic_pointer_cast<f32_3>(s_bind_v);
               if (s_bind_f3) {
                 auto &bind_f3 = *s_bind_f3;
                 auto pviewPos = (float *)&_pt._view_pos;
                 auto copy_size = sizeof(float) * 3;
                 memcpy_s(bind_f3.value, copy_size, pviewPos, copy_size);
                 child_properties.erase(iviewPos);
               }
          }
          auto iprojection = child_properties.find("projection");
          if (iprojection != child_properties.end()) {
               glm::mat4 projection = glm::perspective(
                   glm::radians(_pt._fovy), _pt._aspect, _pt._near, _pt._far);
               auto s_bind_v = iprojection->second;
               auto s_bind_v_16 = dynamic_pointer_cast<f32_16>(s_bind_v);
               if (s_bind_v_16) {
                 auto &bind_v16 = *s_bind_v_16;
                 auto pprojection = glm::value_ptr(projection);
                 auto copy_size = sizeof(float) * 16;
                 memcpy_s(bind_v16.value, copy_size, pprojection, copy_size);
                 child_properties.erase(iprojection);
               }
          }
          auto ilight_dir = child_properties.find("light_direction");
          if (ilight_dir != child_properties.end()) {
               auto s_bindv = ilight_dir->second;
               auto s_bind_f3 = dynamic_pointer_cast<f32_3>(s_bindv);
               if (s_bind_f3) {
                 auto &bind_f3 = *s_bind_f3;
                 auto plight_direction = (float *)&_pt._light_direction;
                 auto copy_size = sizeof(vec3);
                 memcpy_s(bind_f3.value, copy_size, plight_direction,
                          copy_size);
                 child_properties.erase(ilight_dir);
               }
          }
          auto ilight_pos = child_properties.find("light_position");
          if (ilight_pos != child_properties.end()) {
               auto s_bindv = ilight_pos->second;
               auto s_bind_f3 = dynamic_pointer_cast<f32_3>(s_bindv);
               if (s_bind_f3) {
                 auto &bind_f3 = *s_bind_f3;
                 auto plight_pos = (float *)&_pt._light_position;
                 auto copy_size = sizeof(vec3);
                 memcpy_s(bind_f3.value, copy_size, plight_pos, copy_size);
                 child_properties.erase(ilight_pos);
               }
          }
          auto ilight_ambient = child_properties.find("light_ambient");
          if (ilight_ambient != child_properties.end()) {
               auto s_bindv = ilight_ambient->second;
               auto s_bind_f3 = dynamic_pointer_cast<f32_3>(s_bindv);
               if (s_bind_f3) {
                 auto &bind_f3 = *s_bind_f3;
                 auto plight_ambient = (float *)&_pt._light_ambient_clr;
                 auto copy_size = sizeof(vec3);
                 memcpy_s(bind_f3.value, copy_size, plight_ambient, copy_size);
                 child_properties.erase(ilight_ambient);
               }
          }
          auto ilight_diffuse = child_properties.find("light_diffuse");
          if (ilight_diffuse != child_properties.end()) {
               auto s_bindv = ilight_diffuse->second;
               auto s_bind_f3 = dynamic_pointer_cast<f32_3>(s_bindv);
               if (s_bind_f3) {
                 auto &bind_f3 = *s_bind_f3;
                 auto plight_diffuse = (float *)&_pt._light_diffuse_clr;
                 auto copy_size = sizeof(vec3);
                 memcpy_s(bind_f3.value, copy_size, plight_diffuse, copy_size);
                 child_properties.erase(ilight_diffuse);
               }
          }
          auto ilight_specular = child_properties.find("light_specular");
          if (ilight_specular != child_properties.end()) {
               auto s_bindv = ilight_specular->second;
               auto s_bind_f3 = dynamic_pointer_cast<f32_3>(s_bindv);
               if (s_bind_f3) {
                 auto &bind_f3 = *s_bind_f3;
                 auto plight_specular = (float *)&_pt._light_specular_clr;
                 auto copy_size = sizeof(vec3);
                 memcpy_s(bind_f3.value, copy_size, plight_specular, copy_size);
                 child_properties.erase(ilight_specular);
               }
          }
#else
         glm::vec3 cam_pos(EXPVEC3(_pt._view_pos));
         FIND_ITEM(view)
         const auto copy_size_16 = sizeof(float) * 16;
         const auto copy_size_3 = sizeof(float) * 3;
         if (iview != child_properties.end()) {
               glm::vec3 cam_dir(EXPVEC3(_pt._center_of_prj));
               glm::vec3 cam_up(EXPVEC3(_pt._up));
               glm::mat4 view = glm::lookAt(cam_pos, cam_dir, cam_up);
               auto s_bind_v = iview->second;
               auto s_bind_v_16 = dynamic_pointer_cast<bind_var_t< f32_16>>(s_bind_v);
               if (s_bind_v_16) {
                 auto &bind_v_16 = *s_bind_v_16;
                 auto pview = glm::value_ptr(view);
                 memcpy_s(bind_v_16._value.value, copy_size_16, pview, copy_size_16);
                 bind_v_16._calculated = true;
               }
         }

         FIND_ITEM(viewPos);
         if (iviewPos != child_properties.end()) {
               auto s_bind_v = iviewPos->second;
               auto s_bind_f3 =dynamic_pointer_cast<bind_var_t<f32_3>>(s_bind_v);
               if (s_bind_f3) {
                 auto &bind_f3 = *s_bind_f3;
                 auto pviewPos = (float *)&_pt._view_pos;
                 memcpy_s(bind_f3._value.value, copy_size_3, pviewPos, copy_size_3);
                 bind_f3._calculated=true;
               }
         }
         FIND_ITEM(projection)
         if (iprojection != child_properties.end()) {
               glm::mat4 projection = glm::perspective(
                   glm::radians(_pt._fovy), get_aspect(), _pt._near, _pt._far);
               auto s_bind_v = iprojection->second;
               auto s_bind_v_16 =dynamic_pointer_cast<bind_var_t<f32_16>>(s_bind_v);
               if (s_bind_v_16) {
                 auto &bind_v16 = *s_bind_v_16;
                 auto pprojection = glm::value_ptr(projection);
                 memcpy_s(bind_v16._value.value, copy_size_16, pprojection, copy_size_16);
                 bind_v16._calculated=true;
               }
         }
         FIND_ITEM(light_direction);
         if (ilight_direction != child_properties.end()) {
               auto s_bindv = ilight_direction->second;
               auto s_bind_f3 =
                   dynamic_pointer_cast<bind_var_t<f32_3>>(s_bindv);
               if (s_bind_f3) {
                 auto &bind_f3 = *s_bind_f3;
                 auto plight_direction = (float *)&_pt._light_direction;
                 memcpy_s(bind_f3._value.value, copy_size_3, plight_direction,
                          copy_size_3);
                 bind_f3._calculated=true;
               }
         }
         FIND_ITEM(light_position);
         if (ilight_position != child_properties.end()) {
               auto s_bindv = ilight_position->second;
               auto s_bind_f3 =
                   dynamic_pointer_cast<bind_var_t<f32_3>>(s_bindv);
               if (s_bind_f3) {
                 auto &bind_f3 = *s_bind_f3;
                 auto plight_pos = (float *)&_pt._light_position;
                 memcpy_s(bind_f3._value.value, copy_size_3, plight_pos, copy_size_3);
                 bind_f3._calculated=true;
               }
         }
         FIND_ITEM(light_ambient);
         if (ilight_ambient != child_properties.end()) {
               auto s_bindv = ilight_ambient->second;
               auto s_bind_f3 =
                   dynamic_pointer_cast<bind_var_t<f32_3>>(s_bindv);
               if (s_bind_f3) {
                 auto &bind_f3 = *s_bind_f3;
                 auto plight_ambient = (float *)&_pt._light_ambient_clr;
                 memcpy_s(bind_f3._value.value, copy_size_3, plight_ambient, copy_size_3);
                 bind_f3._calculated = true;
               }
         }
         FIND_ITEM(light_diffuse);
         if (ilight_diffuse != child_properties.end()) {
               auto s_bindv = ilight_diffuse->second;
               auto s_bind_f3 =
                   dynamic_pointer_cast<bind_var_t<f32_3>>(s_bindv);
               if (s_bind_f3) {
                 auto &bind_f3 = *s_bind_f3;
                 auto plight_diffuse = (float *)&_pt._light_diffuse_clr;
                 memcpy_s(bind_f3._value.value, copy_size_3, plight_diffuse, copy_size_3);
                 bind_f3._calculated=true;
               }
         }
         FIND_ITEM(light_specular);
         if (ilight_specular != child_properties.end()) {
               auto s_bindv = ilight_specular->second;
               auto s_bind_f3 =
                   dynamic_pointer_cast<bind_var_t<f32_3>>(s_bindv);
               if (s_bind_f3) {
                 auto &bind_f3 = *s_bind_f3;
                 auto plight_specular = (float *)&_pt._light_specular_clr;
                 memcpy_s(bind_f3._value.value, copy_size_3, plight_specular, copy_size_3);
                 bind_f3._calculated=true;
               }
         }
#endif
     }
	 void scene::draw_frames()
     {
          if (!visibility()) {
               return;
          }
          if( !_FBO_id )
          {
               return;
          }
          rescale_framebufffer(_texture_id, _RBO_id, size().x(), size().y());
          GLint last_viewport[ 4 ];
          glGetIntegerv( GL_VIEWPORT, last_viewport );
          GLint last_fmid;
          glGetIntegerv( GL_FRAMEBUFFER_BINDING, &last_fmid );
          glBindFramebuffer( GL_FRAMEBUFFER, _FBO_id );
          glViewport(0, 0, size().x(), size().y());
          glEnable( GL_BLEND );
          glBlendEquation( GL_FUNC_ADD );
          glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
          //glEnable(GL_DEPTH_TEST);
          //glDepthFunc(GL_LESS);
          //glDisable( GL_CULL_FACE );
          if( _pt._test_depth )
          {
               glEnable( GL_DEPTH_TEST );
          }
          else
          {
               glDisable( GL_DEPTH_TEST );
          }
          //glDisable(GL_SCISSOR_TEST);
          glClearColor(_pt._background_clr.x(), _pt._background_clr.y(),
                       _pt._background_clr.z(), _pt._background_clr.w());
          glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
          //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          for( auto it : _vchilds )
          {
               if( it->visibility() )
               {
                    it->draw_frames();
               }
          }
          glBindFramebuffer( GL_FRAMEBUFFER, last_fmid );
          glViewport( last_viewport[ 0 ], last_viewport[ 1 ], (GLsizei)last_viewport[ 2 ], (GLsizei)last_viewport[ 3 ] );

          auto base_draw_pos = get_draw_pos();
          ImVec2 pos1 = to_imvec2(base_draw_pos);
          ImVec2 pos2 = {pos1.x, pos1.y + size().y()};
          ImVec2 pos3 = {pos1.x + size().x(), pos1.y + size().y()};
          ImVec2 pos4 = {pos1.x + size().y(), pos1.y};
          //ImGui::ImageQuad((ImTextureID)_texture_id, pos1, pos2, pos3, pos4, ImVec2(0, 0), ImVec2(0, 1), ImVec2(1, 1), ImVec2(1, 0));
          imgui_ImageQuad((ImTextureID)_texture_id, pos1, pos2, pos3, pos4,
                           ImVec2(1, 1), ImVec2(1, 0), ImVec2(0, 0),
                           ImVec2(0, 1));

     }


} // namespace vg
