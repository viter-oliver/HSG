#include <cmath>
#include "color_node.h"
#include "light_scene.h"
#include "transformation.h"
#include "shader_source_code.h"

namespace vg
{
	 shader::sd_shader_base color_node::_pcolor_node_sd = nullptr;
	 using namespace std;
	 using namespace shader;
     color_node::color_node()
     {
		 _pt._attached_obj[0] = '\0';
		 _pt._ambient_clr = { 0, 0, 0 };
		 _pt._diffuse_clr = { 0, 0, 0 };
		 _pt._specular_clr = { 0, 0, 0 };
		 float transM[16] = {1, 0, 0, 0,
							 0, 1, 0, 0,
							 0, 0, 1, 0, 
							 1, 0, 0, 1, };
		 memcpy(_pt._trans_mat4x4, transM, 16 * sizeof (float));
		 if (!_pcolor_node_sd)
		 {
			 _pcolor_node_sd = make_shared<shader_base>();// modeling_vs_col_d, modeling_fs_col_d);
			 _pcolor_node_sd->build_vs_code(modeling_vs_col_d);
			 _pcolor_node_sd->build_fs_code(modeling_fs_col_d);
			 _pcolor_node_sd->link();
		 }
		 
#if !defined(IMGUI_DISABLE_DEMO_WINDOWS)
		 
#endif
     }
     void color_node::transform( glm::mat4& model )
     {
		 glm::mat4 transM = glm::make_mat4(_pt._trans_mat4x4);
		 model = model*transM;
     }
	 void color_node::link(){
		 auto iprm = vertex::_mp_vertex_base_units.find(_pt._attached_obj);
		 if (iprm != vertex::_mp_vertex_base_units.end())
		 {
			 _ps_prm = iprm->second;
		 }
	 }
	 void color_node::draw(){
		 auto cnt = child_count();
		 for (int ix = 0; ix < cnt;++ix) {
			 auto pchild = get_child(ix);
			 pchild->draw();
		 }
		 if (_ps_prm==nullptr){
			 return;
		 }
		 glm::mat4 model;
		 transform(model);
		 auto pd=get_parent();
		 light_scene* pscene = nullptr;
		 while (pd)
		 {
			 if (typeid(*pd) == typeid(color_node)){
				 color_node* pnode = static_cast <color_node*>(pd);
				 pnode->transform(model);
			 } else if (typeid(*pd) == typeid(light_scene)){
				 pscene = static_cast<light_scene*>(pd);
				 break;
			 } else if (typeid(*pd) == typeid(transformation)){
				 transformation* pnode = static_cast<transformation*>(pd);
				 pnode->transform(model);
			 }
			 pd = pd->get_parent();
		 } 
		 
		 if (pscene == nullptr)
		 {
			 return;
		 }
		 //pscene->transform(model);
		 _pcolor_node_sd->use();
		 _pcolor_node_sd->uniform("model", glm::value_ptr(model));

		 auto pview = pscene->get_view_pos();
		 auto pcenter = pscene->get_center_of_prj();
		 auto pup = pscene->get_up();
		 glm::vec3 cam_pos(pview->x, pview->y, pview->z);
		 glm::vec3 cam_dir(pcenter->x, pcenter->y, pcenter->z);
		 glm::vec3 cam_up(pup->x, pup->y, pup->z);
		 glm::mat4 view = glm::lookAt(cam_pos, cam_dir, cam_up);
		 _pcolor_node_sd->uniform("view", glm::value_ptr(view));
		 _pcolor_node_sd->uniform("viewPos", glm::value_ptr(cam_pos));
		 float w, h;
		 pscene->get_size(w, h);
		 float aspect = w / h;
		 glm::mat4 proj = glm::perspective(glm::radians(pscene->get_fovy()), aspect, pscene->get_near(), pscene->get_far());
		 _pcolor_node_sd->uniform("projection", glm::value_ptr(proj));
		 
		 auto& light_position = pscene->get_light_pos();
		 _pcolor_node_sd->uniform("light_position", (float*)&light_position);
		 auto& light_ambient = pscene->get_light_ambient();
		 _pcolor_node_sd->uniform("light_ambient", (float*)&light_ambient);
		 auto& light_diffuse = pscene->get_light_diffuse();
		 _pcolor_node_sd->uniform("light_diffuse", (float*)&light_diffuse);
		 auto& light_specular = pscene->get_light_specular();
		 _pcolor_node_sd->uniform("light_specular", (float*)&light_specular);
		 
		 //_pcolor_node_sd->uniform("col_ambient", (float*)&_pt._ambient_clr);
		 _pcolor_node_sd->uniform("col_diffuse", (float*)&_pt._diffuse_clr);
		 _pcolor_node_sd->uniform("col_specular", (float*)&_pt._specular_clr);
		 glBindVertexArray(_ps_prm->_vao);
		 if (0 == _ps_prm->_ele_buf_len){
			 glDrawArrays(GL_TRIANGLES, 0, _ps_prm->_vertex_buf_len);
		 }
		 else {
			 glDrawElements(GL_TRIANGLES, _ps_prm->_ele_buf_len, GL_UNSIGNED_INT, 0);
		 }
	 }
}