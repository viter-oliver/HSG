#pragma once
#include <stack>
#include "user_control_imgui.h"
#include "vertex_format.h"
#include "internal_vertex.h"
#include <igl/per_vertex_normals.h>
#include <igl/readOFF.h>
#include <igl/readOBJ.h>
#include <igl/readPLY.h>
#include <igl/readTGF.h>
#include <igl/readDMAT.h>
#include <igl/readMESH.h>
#if 0
#include <igl/readMSH.h>
#include <igl/readSTL.h>
#include <igl/readWRL.h>
#include <igl/readBF.h>
#endif
#include <igl/readNODE.h>
#include <Eigen/Core>
#include "ifile_open.hpp"
namespace vg{
    sd_vertex get_vertex_from_file(const std::string& filepath) {
    
        Eigen::MatrixXd V;
        Eigen::MatrixXi T;
        Eigen::MatrixXi F;
        std::string extension = filepath.substr(filepath.find_last_of('.') + 1);
        bool be_loaded = false;
        if ("off" == extension) {
            be_loaded = igl::readOFF(filepath, V, F);
        }
        else if ("obj" == extension) {
            be_loaded = igl::readOBJ(filepath, V, F);
        }
        else if ("ply" == extension) {
            be_loaded = igl::readPLY(filepath, V, F);
        }
        else if ("tgf" == extension) {
            be_loaded = igl::readTGF(filepath, V, F);
        }
        else if ("dmat" == extension) {
            be_loaded = igl::readDMAT(filepath, V);
        }
        else if ("mesh" == extension) {
            be_loaded = igl::readMESH(filepath, V, T, F);
        }
        else if ("node" == extension) {
            be_loaded = igl::readNODE(filepath, V, F);
        }
        else {
            printf("format is unsupported!\n");
            return nullptr;
        }
        if (!be_loaded) {
            printf("fail to load vertex file:%s\n", filepath.c_str());
            return nullptr;
        }
        Eigen::MatrixXd N;
        igl::per_vertex_normals(V, F, N);
        std::vector<float> buffer;
        buffer.reserve(V.rows() * 9);
        for (int i = 0; i < V.rows(); ++i) {
            // Position
            buffer.push_back((float)V(i, 0));
            buffer.push_back((float)V(i, 1));
            buffer.push_back((float)V(i, 2));

            // Normal
            buffer.push_back((float)N(i, 0));
            buffer.push_back((float)N(i, 1));
            buffer.push_back((float)N(i, 2));

            // Color
            buffer.push_back(std::abs((float)N(i, 0))); // R
            buffer.push_back(std::abs((float)N(i, 1))); // G
            buffer.push_back(std::abs((float)N(i, 2))); // B
        }
        vtx_format fmt = { 3,3,3 };
        Eigen::Matrix<u32, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> F_u32 = F.cast<u32>();
        auto sd_vx_loaded = std::make_shared<vertex>(
            fmt,
            buffer.data(),
            (u32)V.rows(),
            GL_STATIC_DRAW,
            true,
            F_u32.data(),
            (u32)F.size());
		return sd_vx_loaded;
    }
        
  template<>
  bool create_new_pair<vertex>(vertex_key& key,sd_vertex& sd_vx){
    bool be_sucessful=false;
    static char key_str[FILE_NAME_LEN]="";
    static int vertex_cnt=1;
    static int mem_id=gl_static_draw;
    static vtx_format vfmt={3};
    static std::stack<std::pair<std::string, sd_vertex>> temp_vertices;
    if (ImGui::Button("Triangle")) {
        const std::string str_tri("triangle");
        key.get_str_value(str_tri);
		vtx_format tri_vmt = { 3 ,3};
        sd_vx= std::make_shared<vertex>(tri_vmt, triangle, 3, GL_STATIC_DRAW);
        be_sucessful = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Squre")) {
		key.get_str_value("Squre");
		vtx_format sq_vmt = { 3 ,2};
        sd_vx = std::make_shared<vertex>(sq_vmt, squre, 4, GL_STATIC_DRAW);
		be_sucessful = true;
    }
	ImGui::SameLine();
    if (ImGui::Button("Load verteices...")) {
        OPENFILENAME ofn = { sizeof(OPENFILENAME) };
        // ofn.lStructSize = sizeof(ofn);
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = GetForegroundWindow();
        ofn.lpstrFilter =
            "verteices "
            "file:\0*.off;*.obj;*.ply;*.tgf;*.dmat;*.mesh;*.node;\0\0";
        
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST
            | OFN_ALLOWMULTISELECT | OFN_EXPLORER ;
        char strFileName[4096] = { 0 };
        ofn.nFilterIndex = 1;
        ofn.lpstrFile = strFileName;
        ofn.nMaxFile = sizeof(strFileName);
        ofn.lpstrTitle = "Loading vertices...";
        if (GetOpenFileName(&ofn)) {
            char* p = ofn.lpstrFile;
            std::string directory = p;
            p += directory.size() + 1;
            auto load_vertex_file=[&](const std::string& filepath) {
					auto sd_vx_loaded = get_vertex_from_file(filepath);
                    if (sd_vx_loaded)
                    {
					    std::string filename = filepath.substr(filepath.find_last_of("\\") + 1);
					    temp_vertices.push(std::make_pair(filename, sd_vx_loaded));
                    }

			    };
            if (*p == 0) {
                // ֻѡ����һ���ļ�
				load_vertex_file(directory);
            }
            else {
                // ��ѡ��Ŀ¼ + ����ļ���
                while (*p) {
                    std::string filename = p;
					load_vertex_file(directory + "\\" + filename);
                    p += filename.size() + 1;
                }
            }

        }
        //Eigen::MatrixXd V;
        //Eigen::MatrixXi F;


    }
    if (!temp_vertices.empty()) {
        auto pair_eles = temp_vertices.top();
        key.get_str_value(pair_eles.first);
        sd_vx = pair_eles.second;
        temp_vertices.pop();
        be_sucessful = true;
    }
    if (ImGui::Button("New vertex...")) {
        key_str[0] = '\0';
        vertex_cnt = 1;
        mem_id = gl_static_draw;
        vfmt = { 3 };
        ImGui::OpenPopup("New vertex");
    }
    //if (ImGui::TreeNode("Internal vertices")) {
      //  ImGui::TreePop();
    //}
    if(ImGui::BeginPopup("New vertex")){
      ImGui::InputText("Vertex name",key_str, FILE_NAME_LEN );
      ImGui::Combo("Memory usage:",&mem_id,mem_usage_show,gl_usage_cnt);
      ImGui::InputInt("Count of vertex:", &vertex_cnt);
      ImGui::Text("Element format:");
      auto vfmtsz=vfmt.size();
      for (int ix = 0; ix < vfmtsz; ix++) {
        auto& fmt=vfmt[ix];
        ImGui::Text("%d", fmt);
        std::string btn_left_cap = "##left", btn_right_cap = "##right";
        btn_left_cap += std::to_string(ix);
        btn_right_cap+=std::to_string(ix);
        if (fmt > 1) {
          ImGui::SameLine();
          if (ImGui::ArrowButton(btn_left_cap.c_str(), ImGuiDir_Left)) {
            fmt--;
          }
        }
        if (fmt < 4) {
          ImGui::SameLine();
          if (ImGui::ArrowButton(btn_right_cap.c_str(), ImGuiDir_Right)) {
            fmt++;
          }
        }
        if(ix==vfmtsz-1){
          if(vfmtsz<6)
          {
            ImGui::SameLine();
            if(ImGui::Button("+")){
              vfmt.emplace_back();
              vfmt.back() = 1;
            }
          }
          if(vfmtsz>1){
            ImGui::SameLine();
            if(ImGui::Button("-")){
              vfmt.resize(vfmtsz-1);
            }
          }
        }
      }
      if(ImGui::Button("New")){
        std::string str_key(key_str);
        if (str_key.size() != 0) {
          key.get_str_value( str_key );
          u8 std_len=0;
          for(auto& ele:vfmt){
            std_len+=ele;
          }
          auto vertex_buffer_len=std_len*sizeof(float)* vertex_cnt;
          float* pvertex=new float[vertex_buffer_len];
          sd_vx=std::make_shared<vertex>(vfmt,pvertex,vertex_cnt, GL_STATIC_DRAW );
          delete[] pvertex;
          be_sucessful=true;
        }
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine(100.f);
      if(ImGui::Button("Cancel")){
        ImGui::CloseCurrentPopup();
      }
    
      ImGui::EndPopup();
    }
    return be_sucessful;
  }
  template<>
  void res_edit<vertex>(vertex& vx){
    ImGui::Text("Count of vertex:%d", vx._vertex_count);
    ImGui::BeginTable("Vertex format:",vx._format.size(),ImGuiTableFlags_SizingFixedFit|ImGuiTableFlags_RowBg|ImGuiTableFlags_Borders);
      ImGui::TableNextRow();
      for(int col=0;col<vx._format.size();col++){
        ImGui::TableSetColumnIndex(col);
        auto& el=vx._format[col];
        ImGui::Text("%d",el);
      }
    ImGui::EndTable();
    ImGui::Text("Count of element:%d", vx._ele_count);
    static struct temp_memory
    {
      float *pvbo{0}, *pvbos[7]{0};
      u32 vbo_size, vbo_sizes[7], *pel{0}, el_size;
      void check_memory() {
        if (pvbo) {
          delete[] pvbo;
          pvbo = 0;
        }
        if (pel) {
          delete[] pel;
          pel = 0;
        }
        for (int i = 0; i < 7; i++) {
          if (pvbos[i]) {
            delete[] pvbos[i];
            pvbos[i]=0;
          }
        }
      }
      ~temp_memory() { 
        check_memory();
      }
    } tm;

    if (ImGui::Button("View vertex data..")) {
        tm.check_memory();
        if (vx._interlaced) {
          glBindBuffer(GL_ARRAY_BUFFER, vx._vbos[0]);
          glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, (GLint*) &tm.vbo_size);
          tm.pvbo = new GLfloat[tm.vbo_size / sizeof(GLfloat)];
          glGetBufferSubData(GL_ARRAY_BUFFER, 0, tm.vbo_size, tm.pvbo);
        } else {
          for (int ix = 0; ix < vx._format.size(); ix++) {
            glBindBuffer(GL_ARRAY_BUFFER, vx._vbos[ix]);
            glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE,
                                   (GLint *)&tm.vbo_sizes[ix]);
            tm.pvbos[ix] = new GLfloat[tm.vbo_size / sizeof(GLfloat)];
            glGetBufferSubData(GL_ARRAY_BUFFER, 0, tm.vbo_sizes[ix],
                               tm.pvbos[ix]);
          }
        }
        if (vx._ele_count > 0) {
          tm.el_size = vx._ele_count;
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vx._ebo);
          tm.pel = new GLuint[tm.el_size];
          glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, tm.el_size, tm.pel);
        }

      ImGui::OpenPopup("View vertex data..");
    }
    
    
    if (ImGui::BeginPopupModal("View vertex data..")) {
      if (vx._interlaced) {
          auto vtx_width = vx.stride() * 120+100;
          auto ele_width = 120;
          auto width = vtx_width + ele_width;
          ImGui::BeginChild("vertices", ImVec2(width, 1000));
          ImGui::Columns(2);
          ImGui::SetColumnWidth(0, vtx_width);
          auto dm = vx.stride();
          float *pvx = tm.pvbo;
          for (int ix = 0; ix < vx._vertex_count; ++ix) {
            ImGui::Text("%d:", ix);
            string str_show;
            for (int ii = 0; ii < dm; ii++) {
              str_show += " ";
              str_show += to_string(pvx[ii]);
            }
            str_show += " |";
            ImGui::SameLine(0, 0);
            ImGui::Text(str_show.c_str());
            pvx += dm;
          }
          ImGui::NextColumn();
          string ele_show;
          GLuint *pel = tm.pel;
          auto rm_cnt = tm.el_size % 3;
          for (int ii = 0; ii < rm_cnt; ii++) {
            ele_show = ele_show + "|"+ to_string(*pel++);
          }
          ele_show += "|";
          ImGui::Text(ele_show.c_str());
          rm_cnt = tm.el_size - rm_cnt;
          for (int ii = 0; ii < rm_cnt;ii+=3) {
            ele_show = "";
            ele_show += ele_show + "|" + to_string(*pel++);
            ele_show += ele_show + "|" + to_string(*pel++);
            ele_show += ele_show + "|" + to_string(*pel++);
            ele_show += "|";
            ImGui::Text(ele_show.c_str());
          }
          ImGui::EndChild();
      } else {
          if (ImGui::BeginTable("vertices", vx._format.size(),
                                ImGuiTableFlags_Borders)) {
            float **pvs = new float* [vx._format.size()];
            for (int ix = 0; ix < vx._format.size(); ix++) {
              string column_name = "component" + to_string(ix);
              ImGui::TableSetupColumn(column_name.c_str());
              pvs[ix] = tm.pvbos[ix];
            }
            for (int ix = 0; ix < vx._vertex_count; ++ix) {
              ImGui::PushID(ix);
              for (int ii = 0; ii < vx._format.size(); ++ii) {
                ImGui::TableSetColumnIndex(ii);
                auto fu = vx._format[ii];
                string str_u;
                for (int jj = 0; jj < fu; ++jj) {
                  str_u += "|";
                  str_u += to_string(pvs[ii][jj]);
                }
                ImGui::Text(str_u.c_str());
                pvs[ii] += fu;
              }
              ImGui::PopID();
            }
            delete[] pvs;
            ImGui::EndTable();
          }
          
      }
      if (ImGui::Button("Close")) {
          ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }
  }
  REG_MAP_COPONENT_EDIT_CONTAINER(vertex)
  struct open_vertex :public IFileOpener {
      bool tryOpen(const std::string& path) {
          size_t pos = path.find_last_of('.');
          if (pos != std::string::npos) {
              std::string ext = path.substr(pos + 1);
              const char* supported_extensions[] = {
                        "off", "obj", "ply", "tgf", "dmat", "mesh", "node", nullptr
              };
              //check supported extensions
              for (int ix = 0; supported_extensions[ix] != nullptr; ix++) {
                  if (ext == supported_extensions[ix]) {
                      std::string filename = path.substr(path.find_last_of("\\") + 1);
                      sd_vertex sd_vx = get_vertex_from_file(path);
                      if (sd_vx) {
                          vertex_key vkey;
                          vkey.get_str_value(filename);
                          vertex_assist_._sd_o->add_new_ele(vkey, sd_vx);
                          return true;
                      }
                  }
              }
          }
          return false;
      }
  };
}