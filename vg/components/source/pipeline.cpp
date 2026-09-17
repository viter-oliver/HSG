
#include <ranges>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "pipeline.h"
#include "common_functions.h"
namespace vg { 
    model_transform::model_transform() {
        glm::mat4 model = glm::mat4(1.0f);
        const float* fptr = glm::value_ptr(model);
        memcpy(_pt.base_value.value, fptr, sizeof(glm::mat4));
        _pt.translate = { 0.f,0.f,0.f };
        _pt.scale = { 1.f,1.f,1.f };
        _pt.roatation_axis = { 0.f,0.f,1.f };
    }
#define EXPAND_VEC3(vec3) vec3.x(), vec3.y(), vec3.z()
    bool model_transform::calculate(fun_find_unifoms fd_sd_sd){
        v_sd_uniform_interface unifoms;
        fd_sd_sd(op, unifoms);
        if (unifoms.size()==0) {
            return false;
        }
        glm::mat4 model = glm::make_mat4(_pt.base_value.value);
        glm::vec3 translate(EXPAND_VEC3(_pt.translate));
        glm::vec3 scale(EXPAND_VEC3(_pt.scale));
        glm::vec3 rotation_axis(EXPAND_VEC3(_pt.roatation_axis));
        std::function<void()> f_transflom[transform_order::transform_order_count] = {
            [&]() {
                model = glm::rotate(model, _pt.angle, rotation_axis);
                model = glm::scale(model, scale);
                model = glm::translate(model, translate);
            },
            [&]() {
                model = glm::scale(model, scale);
                model = glm::rotate(model, _pt.angle, rotation_axis);
                model = glm::translate(model, translate);
            },
            [&]() {
                model = glm::scale(model, scale);
                model = glm::translate(model, translate);
                model = glm::rotate(model, _pt.angle, rotation_axis);
            },
            [&]() {
                model = glm::translate(model, translate);
                model = glm::rotate(model, _pt.angle, rotation_axis);
                model = glm::scale(model, scale);
            },
            [&]() {
                model = glm::scale(model, scale);
                model = glm::translate(model, translate);
                model = glm::rotate(model, _pt.angle, rotation_axis);
            },
            [&]() {
                model = glm::translate(model, translate);
                model = glm::scale(model, scale);
                model = glm::rotate(model, _pt.angle, rotation_axis);
            },
        };
        f_transflom[_pt.order]();
        auto pmodel=glm::value_ptr(model);
        for (auto&sd_sd : unifoms) {
            sd_sd->set_location(pmodel);
        }
        return true;
    }
    camera::camera() {
        _pt.position = { 0.f,206.f,-600.f };
        _pt.direction = { 0.f,90.f,0.f };
        _pt.up = { 0.f,1.f,0.f };
    }
    bool camera::calculate(fun_find_unifoms fd_sd_sd){
        v_sd_uniform_interface unifoms;
        fd_sd_sd(op, unifoms);
        if (unifoms.size() == 0) {
            return false;
        }
        glm::vec3 position(EXPAND_VEC3(_pt.position));
        glm::vec3 direction(EXPAND_VEC3(_pt.direction));
        glm::vec3 up(EXPAND_VEC3(_pt.up));
        glm::mat4 view=glm::lookAt(position, direction,up);
        auto pview=glm::value_ptr(view);
        for (auto&sd_sd : unifoms) {
            sd_sd->set_location(pview);
        }
        return true;
    }

    bool projection::calculate(fun_find_unifoms fd_sd_sd){
        v_sd_uniform_interface unifoms;
        fd_sd_sd(op, unifoms);
        if (unifoms.size() == 0) {
            return false;
        }
        glm::mat4 projection=glm::perspective(glm::radians(_pt.fovy), _pt.aspect, _pt._near, _pt._far);
        auto pprojection=glm::value_ptr(projection);
        for (auto&sd_sd : unifoms) {
            sd_sd->set_location(pprojection);
        }
        return true;
    }
    camera_ex::camera_ex() {
        _pt.position = { 0.f,206.f,-600.f };
        _pt.direction = { 0.f,90.f,0.f };
        _pt.up = { 0.f,1.f,0.f };
    }
    bool camera_ex::calculate(fun_find_unifoms fd_sd_sd){
        v_sd_uniform_interface unifoms;
        fd_sd_sd(op, unifoms);
        if (unifoms.size() == 0) {
            return false;
        }
        glm::vec3 position(EXPAND_VEC3(_pt.position));
        glm::vec3 direction(EXPAND_VEC3(_pt.direction));
        glm::vec3 up(EXPAND_VEC3(_pt.up));
        glm::mat4 view=glm::lookAt(position, direction,up);
        glm::mat4 projection=glm::perspective(glm::radians(_pt.fovy), _pt.aspect, _pt._near, _pt._far);
        glm::mat4 vp = projection * view;
        auto pvp=glm::value_ptr(vp);
        for (auto&sd_sd : unifoms) {
            sd_sd->set_location(pvp);
        }
        unifoms.clear();
		fd_sd_sd(camPos, unifoms);
        for (auto& sd_sd : unifoms) {
            sd_sd->set_location(glm::value_ptr(position));
        }
        return true;
	}

    bool lua_script::calculate(fun_find_unifoms fd_sd_sd){
        v_sd_uniform_interface unifoms;
        fd_sd_sd(0, unifoms);
        if (unifoms.size() == 0) {
            return false;
        }
        return true;
    }
    static struct {
        int texture_unit_id = {0};
        int allocate() {
            return texture_unit_id++;
        }
        void free_all() { texture_unit_id = 0; }
    }texture_unit_mg;
    bool texture_unit::calculate(fun_find_unifoms fd_sd_sd){
        v_sd_uniform_interface unifoms;
        fd_sd_sd(op, unifoms);
        if (unifoms.size() == 0) {
            return false;
        }
        if (!sd_txt) {
            auto isd_txt = texture_map_.find(_pt.tkey);
            if (isd_txt != texture_map_.end()) {
                sd_txt = isd_txt->second;
                
            }
        }
        if (sd_txt) {
			int txt_id = texture_unit_mg.allocate();
            glActiveTexture(GL_TEXTURE0 + txt_id);
            glBindTexture(GL_TEXTURE_2D, sd_txt->_txt_id);
            for (auto& sd_sd : unifoms) {
                sd_sd->set_location(txt_id);
            }
			return true;
        }
        return false;
    }

    bool pipeline::update_shader_vertex() {
        auto isd_shader = shader_base_map_.find(_pt.skey);
        if (isd_shader != shader_base_map_.end()) {
            _sd_shader = isd_shader->second;
            data_inputs.clear();
			_sd_shader->iterate_uniforms([&](const std::string& name, sd_shader_variable_type sd_type) {
                data_inputs.emplace_back(std::make_pair(name, sd_type));
				});
        }
        auto ivtx = vertex_map_.find(_pt.vkey);
        if (ivtx != vertex_map_.end()) {
            _sd_data = ivtx->second;
        }
        return true;
	}
    bool pipeline::ready() {
        
        if(!_sd_data||!_sd_shader){
            return false;
		}
        return _sd_shader->match_format(_sd_data->_format);
    }
    
    bool pipeline::calculate(fun_find_unifoms fd_sd_sd) {
        v_sd_uniform_interface unifoms;
        fd_sd_sd(op, unifoms);
        if (unifoms.size() == 0) {
            return false;
        }
        render();
        return true;    
    }
    
    bool pipeline::render() {
        if(_sd_data){
            auto draw_model_ = draw_model[_pt.draw_model_id];
            _sd_data->draw(draw_model_);
            texture_unit_mg.free_all();
        }

        return true;
    }
    bool pipeline::prepare() {
        if (ready()) {
            _sd_shader->use();
            return true;
        }
        else
            return false;
    }
    canvas::canvas() {
        _pt.background_clr = { 0.5, 0.5, 0., 1. };
        _pt.frame_size = { 800,600 };
        create_fbo();
    }
    void canvas::create_fbo() {
        prepareFBO_ex(texture_id, FBO_id, RBO_id, _pt.frame_size.x(),
            _pt.frame_size.y());
	}
    canvas::~canvas() {
        release_resource();
    }
    bool canvas::calculate(fun_find_unifoms fd_sd_sd) {
        glBindFramebuffer(GL_FRAMEBUFFER, last_framebuffer_id);
        glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
        glDepthMask(last_depth_mask);
		//ImVec2 img_size = ImVec2((float)_pt.frame_size.x(), (float)_pt.frame_size.y());
		//ImGui::Image((void*)(uintptr_t)texture_id, img_size, ImVec2(0, 1), ImVec2(1, 0));
		//ImGui::Image((ImTextureID)(uintptr_t)texture_id, img_size);
        return true;
    }
    bool canvas::prepare() {
        glGetIntegerv(GL_VIEWPORT, last_viewport);
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last_framebuffer_id);
        glGetBooleanv(GL_DEPTH_WRITEMASK, &last_depth_mask);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO_id);
        glViewport(0, 0, _pt.frame_size.x(), _pt.frame_size.y());
        glDepthMask(GL_TRUE);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (_pt.test_depth)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
        glDisable(GL_SCISSOR_TEST);
        glClearColor(_pt.background_clr.x(), _pt.background_clr.y(),
            _pt.background_clr.z(), _pt.background_clr.w());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return true;
    }
    void canvas::release_resource() {
        if (RBO_id) {
            glDeleteRenderbuffers(1, &RBO_id);
            RBO_id = 0;
        }
        if (texture_id) {
            glDeleteTextures(1, &texture_id);
            texture_id = 0;
        }
        if (FBO_id) {
            glDeleteFramebuffers(1, &FBO_id);
            FBO_id = 0;
        }
	}

    void calculate_node_ex(int in_node_id, int& execute_count) {
        auto& calcu_u_in = *v_sd_calculate_unit_[in_node_id];
        if (!calcu_u_in.prepare()) {
            printf("node[%d]fail to prepare!\n", in_node_id);
            return;
        }
		int input_count = calcu_u_in.input_count();
        for (int id = 0; id < input_count; id++) {
            auto [begin,end] = links_ex_.equal_range({ in_node_id, id });
            for (auto it = begin; it != end; it++)
            {
				calculate_node_ex(it->second.in_node_id, execute_count);
            }
        }
        calcu_u_in.calculate([&](int output_id, v_sd_uniform_interface& v_sd_unifoms) {
            for (auto&& [key, value] : links_ex_ | std::views::filter([&](auto const& kv) {
                return kv.second.in_node_id == in_node_id && kv.second.in_sub_id == output_id;
                }))
            {
                auto& calcu_u_out = *v_sd_calculate_unit_[key.out_node_id];
                auto bind_u = calcu_u_out.get_input_uniform(key.out_sub_id);
                v_sd_unifoms.emplace_back(bind_u);
                value.thickness = 15;
                execute_count++;
            }
        });

    }
}