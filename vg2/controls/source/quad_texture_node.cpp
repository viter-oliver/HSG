#include <cmath>
#include "ft_1_texture_node.h"
#include "af_primitive_object.h"
#include "ft_light_scene.h"
#include "ft_trans.h"
#include "af_shader_source_code.h"

namespace auto_future
{
	ps_shader ft_1_texture_node::_ptxt_node_sd = nullptr;
     ft_1_texture_node::ft_1_texture_node()
     {
		 _pt._attached_txt[0] = '\0';
		 _pt._attached_obj[0] = '\0';
		 float transM[16] = {1, 0, 0, 0,
							 0, 1, 0, 0,
							 0, 0, 1, 0, 
							 1, 0, 0, 1, };
		 memcpy(_pt._trans_mat4x4, transM, 16 * sizeof (float));
		 if (!ft_1_texture_node::_ptxt_node_sd)
		 {
			 ft_1_texture_node::_ptxt_node_sd = make_shared<af_shader>(modeling_vs_1_txt, modeling_fs_1_txt);
		 }
		 
#if !defined(IMGUI_DISABLE_DEMO_WINDOWS)
		 reg_property_handle(&_pt, 0, [this](void* member_address)
		 {
			 if (_ps_prm)
			 {
				 ImGui::Text("Attached primitive:%s", _pt._attached_obj);
				 ImGui::SameLine();
				 if (ImGui::Button("Delink##primitive"))
				 {
					 _ps_prm = nullptr;
				 }
			 }
			 else
			 {
				 ImGui::InputText("Attached primitive:", _pt._attached_obj, FILE_NAME_LEN);
				 if (ImGui::Button("Import"))
				 {
					 auto itxt = g_primitive_list.find(_pt._attached_obj);
					 if (itxt != g_primitive_list.end())
					 {
						 _ps_prm = itxt->second;
					 }
				 }
			 }
		 });
		 reg_property_handle(&_pt, 1, [this](void* member_address)
		 {
			 if (_ps_txt)
			 {
				 ImGui::Text("Attached image:%s", _pt._attached_txt);
				 ImGui::SameLine();
				 if (ImGui::Button("Delink##txture"))
				 {
					 _ps_txt = nullptr;
				 }
			 }
			 else
			 {
				 ImGui::InputText("Attached image:", _pt._attached_txt, FILE_NAME_LEN);
				 if (ImGui::Button("Import##texture"))
				 {
					 auto itxt = g_mtexture_list.find(_pt._attached_txt);
					 if (itxt != g_mtexture_list.end())
					 {
						 _ps_txt = itxt->second;
					 }
				 }
			 }
		 });
		 reg_property_handle(&_pt, 2, [this](void* member_address)
		 {
			 ImGui::Text("transformation:");
			 float* pf = _pt._trans_mat4x4;
			 ImGui::InputFloat4("1st vec4:", pf);
			 ImGui::InputFloat4("2nd vec4:", pf + 4);
			 ImGui::InputFloat4("3rd vec4:", pf + 8);
			 ImGui::InputFloat4("4th vec4:", pf + 12);

		 });
#endif
     }
     void ft_1_texture_node::transform( glm::mat4& model )
     {
		 glm::mat4 transM = glm::make_mat4(_pt._trans_mat4x4);
		 model = model*transM;
     }
	 void ft_1_texture_node::link(){
		 auto iprm = g_primitive_list.find(_pt._attached_obj);
		 if (iprm != g_primitive_list.end())
		 {
			 _ps_prm = iprm->second;
		 }
		 auto iat = g_mtexture_list.find(_pt._attached_txt);
		 if (iat != g_mtexture_list.end())
		 {
			 _ps_txt = iat->second;
		 }
	 }
	 void ft_1_texture_node::draw(){
		 auto cnt = child_count();
		 for (int ix = 0; ix < cnt;++ix) {
			 auto pchild = get_child(ix);
			 pchild->draw();
		 }
		 if (_ps_prm == nullptr || _ps_txt==nullptr){
			 return;
		 }
		 glm::mat4 model;
		 transform(model);
		 auto pd=get_parent();
		 ft_light_scene* pscene = nullptr;
		 while (pd)
		 {
			 if (typeid(*pd) == typeid(ft_1_texture_node)){
				 ft_1_texture_node* pnode = static_cast <ft_1_texture_node*>(pd);
				 pnode->transform(model);
			 } else if (typeid(*pd) == typeid(ft_light_scene)){
				 pscene = static_cast<ft_light_scene*>(pd);
				 break;
			 } else if (typeid(*pd) == typeid(ft_trans)){
				 ft_trans* pnode = static_cast<ft_trans*>(pd);
				 pnode->transform(model);
			 }
			 pd = pd->get_parent();
		 } 
		 
		 if (pscene == nullptr)
		 {
			 return;
		 }
		 //pscene->transform(model);
		 _ptxt_node_sd->use();
		 _ptxt_node_sd->uniform("model", glm::value_ptr(model));

		 auto pview = pscene->get_view_pos();
		 auto pcenter = pscene->get_center_of_prj();
		 auto pup = pscene->get_up();
		 glm::vec3 cam_pos(pview->x, pview->y, pview->z);
		 glm::vec3 cam_dir(pcenter->x, pcenter->y, pcenter->z);
		 glm::vec3 cam_up(pup->x, pup->y, pup->z);
		 glm::mat4 view = glm::lookAt(cam_pos, cam_dir, cam_up);
		 _ptxt_node_sd->uniform("view", glm::value_ptr(view));
		 _ptxt_node_sd->uniform("viewPos", glm::value_ptr(cam_pos));
		 float w, h;
		 pscene->get_size(w, h);
		 float aspect = w / h;
		 glm::mat4 proj = glm::perspective(glm::radians(pscene->get_fovy()), aspect, pscene->get_near(), pscene->get_far());
		 _ptxt_node_sd->uniform("projection", glm::value_ptr(proj));
		 
		 auto& light_position = pscene->get_light_pos();
		 _ptxt_node_sd->uniform("light_position", (float*)&light_position);
		 auto& light_ambient = pscene->get_light_ambient();
		 _ptxt_node_sd->uniform("light_ambient", (float*)&light_ambient);
		 auto& light_diffuse = pscene->get_light_diffuse();
		 _ptxt_node_sd->uniform("light_diffuse", (float*)&light_diffuse);
		 auto& light_specular = pscene->get_light_specular();
		 _ptxt_node_sd->uniform("light_specular", (float*)&light_specular);
		 
		 //_ptxt_node_sd->uniform("col_ambient", (float*)&_pt._ambient_clr);
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, _ps_txt->_txt_id());
		 _ptxt_node_sd->uniform("texture_diffuse", 0);

		 glBindVertexArray(_ps_prm->_vao);
		 if (0 == _ps_prm->_ele_buf_len){
			 glDrawArrays(GL_TRIANGLES, 0, _ps_prm->_vertex_buf_len);
		 }
		 else {
			 glDrawElements(GL_TRIANGLES, _ps_prm->_ele_buf_len, GL_UNSIGNED_INT, 0);
		 }
	 }
}