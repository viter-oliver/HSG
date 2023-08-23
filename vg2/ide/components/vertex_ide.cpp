#include <sstream>
#include "dir_output.h"
#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include "glad/glad.h"
#endif
#include "vertex_ide.h"
#include "user_control_imgui.h"
namespace vg {
using namespace std;
extern string g_current_directory;
static const char* mem_usage_str[]={
"GL_STREAM_DRAW", 
"GL_STREAM_READ", 
"GL_STREAM_COPY", 
"GL_STATIC_DRAW", 
"GL_STATIC_READ", 
"GL_STATIC_COPY", 
"GL_DYNAMIC_DRAW",
"GL_DYNAMIC_READ",
"GL_DYNAMIC_COPY"
};
static s32 const mem_usage_value[]={
GL_STREAM_DRAW,
GL_STREAM_READ,
GL_STREAM_COPY,
GL_STATIC_DRAW,
GL_STATIC_READ,
GL_STATIC_COPY,
GL_DYNAMIC_DRAW,
GL_DYNAMIC_READ,
GL_DYNAMIC_COPY
};
static s32 find_idx(s32 mug){
  for(s32 idx=0;idx<sizeof(mem_usage_value)/sizeof(s32);++idx){
    if(mem_usage_value[idx]==mug) return idx;
  }
  return -1;
}
static s32 bufflen=0,mem_usage= GL_STATIC_DRAW, mem_usage_id=0,vertex_cnt=0;
static char str_numb[0xa] = {'0', '1', '2', '3', '4',
                            '5', '6', '7', '8', '9'};
vertex_ide::sd_vertex_ide vertex_ide::new_instance() {
    static char prm_name_str[FILE_NAME_LEN] = "";
    static vector<GLubyte> prm_frm = { 3 };
    static GLint vetex_cnt = 1;
    static bool first_call = true;
    sd_vertex_ide sd_vide = nullptr;
    if (first_call) {
        mem_usage_id = 0;
        first_call = false;
    }
    ImGui::InputText("enter a name for the new vertex object", prm_name_str, FILE_NAME_LEN);
    ImGui::InputInt("enter a number for the count of vertex of new vertex object", &vetex_cnt);
    if (ImGui::Combo("Memory usage:", &mem_usage_id, mem_usage_str, sizeof(mem_usage_value) / sizeof(s32))) {
        mem_usage = mem_usage_value[mem_usage_id];
    }		ImGui::Text("Element format:");
    string str_minus("-##");
    auto& fmts = prm_frm;
    auto fsz = fmts.size();
    string str_plus("+##");
    GLuint stride = 0;
    for (int ix = 0; ix < fsz; ix++)
    {
        stride += fmts[ix];
        ImGui::Text("%d", fmts[ix]);
        if (fmts[ix] > 1)
        {
            ImGui::SameLine();
            str_minus += "1";
            if (ImGui::Button(str_minus.c_str()))
            {
                fmts[ix]--;
            }
        }
        if (fmts[ix] < 10)
        {
            ImGui::SameLine();
            str_plus += "1";
            if (ImGui::Button(str_plus.c_str()))
            {
                fmts[ix]++;
            }
        }
    }

    if (fsz < 10)
    {
        if (ImGui::Button("Add..."))
        {
            fmts.emplace_back();
            fmts.back() = 1;
        }
    }
    if (fsz > 1)
    {
        if (fsz < 10)
        {
            ImGui::SameLine();
        }
        if (ImGui::Button("Del..."))
        {
            fmts.resize(fsz - 1);
        }
    }
    if (ImGui::Button("New primitve"))
    {
        
        auto pmtv = make_shared<vertex_ide>();
        auto stride_ = pmtv->stride();
        auto vlen = stride_ * vetex_cnt;
        GLfloat* pvertex = new GLfloat[vlen];
        memset(pvertex, 0, vlen * sizeof GLfloat);
        pmtv->load_vertex_data(mem_usage, pvertex, vlen, prm_frm);
        sd_vide = pmtv;
        prm_frm = { 3 };
        vetex_cnt = 1;
        ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
    {
        ImGui::CloseCurrentPopup();
    }
    return sd_vide;
}
void vertex_ide::edit(vertex_ide& vu) {

  static float* pvertex=nullptr;
  static float adj_unit[3]={0.f,0.f,0.f};
  string str_fmt;
  for (auto &fmt_u : vu._format) {
    str_fmt += str_numb[fmt_u];
    str_fmt += ':';
  }
  str_fmt.back() = '\0';
  ImGui::Text("Element format:%s", str_fmt.c_str());
}

} // namespace vg