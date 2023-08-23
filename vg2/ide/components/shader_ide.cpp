#include "fstream"
#include "shader_ide.h"
#include "common_functions.h"
#include "ide_common_functions.h"
#include "dir_output.h"
#include <Commdlg.h>
#include <ShlObj.h>
#include <imgui.h>
#include <windows.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

namespace vg {
using namespace std;

extern string g_current_directory;
const char *vs_default_code = R"glsl(
#version 300 es
in vec3 position;
in vec3 color;
out vec3 vcolor;
uniform mat4 trans;
void main()
{
    gl_Position = trans * vec4(position, 1.0);
    vcolor=color;
}
)glsl";
const char *fs_default_code = R"glsl(
#version 300 es
in vec3 vcolor;
out vec4 color;
void main()
{
    color = vcolor;
}
)glsl";

static char vs_code[TXT_BUFF_SZ];
static char fs_code[TXT_BUFF_SZ];
shader_ide::sd_shader_ide shader_ide::new_instance() {
static char shader_name[FILE_NAME_LEN] = { 0 };
    static char vs_shader_name[FILE_NAME_LEN] = { 0 };
    static char fs_shader_name[FILE_NAME_LEN] = { 0 };
    static bool first_call = true;
    static string vs_name, fs_name;
    sd_shader_ide new_sd_shader = nullptr;
    if (first_call) {
        shader_name[0] = 0;
        vs_shader_name[0] = 0;
        fs_shader_name[0] = 0;
        first_call = false;
    }
    ImGui::InputText("Shader name:", shader_name, FILE_NAME_LEN);
    ImGui::InputText("Vertex shader file:", vs_shader_name, FILE_NAME_LEN);
    ImGui::SameLine();
    if (ImGui::Button("...##vscode")) {
        vs_name.clear();
        open_file_to_folder(vs_name, shaders_fold,
            "Loading new vertex shader");
        strcpy(vs_shader_name, vs_name.c_str());
        vs_shader_name[vs_name.size()] = 0;
    }

    ImGui::InputText("Fragment shader file:", fs_shader_name, FILE_NAME_LEN);
    ImGui::SameLine();
    if (ImGui::Button("...##fscode")) {
        fs_name.clear();
        open_file_to_folder(fs_name, shaders_fold,
            "Loading new fragment shader");
        strcpy(fs_shader_name, fs_name.c_str());
        fs_shader_name[fs_name.size()] = 0;
    }
    bool be_enabled = strlen(shader_name) > 0 && strlen(vs_shader_name) > 0 &&
        strlen(fs_shader_name) > 0;
    if (!be_enabled) {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    if (ImGui::Button("Confirm...")) {
        string shader_test_name = shader_name;

        auto sd_shd_i = make_shared<shader_ide>();
        string sd_dir_path = g_current_directory + shaders_fold;
        const string vs_shader_file = sd_dir_path + vs_shader_name;
        if (!fileExist(vs_shader_file.c_str())) {
            sd_shd_i->_vs_code = vs_default_code;
            ofstream fout(vs_shader_file); // (file_full_path);
            fout << sd_shd_i->_vs_code;
        }
        else {
            get_file_data(vs_shader_file, [&](int file_sz)->char* {
                sd_shd_i->_vs_code.resize(file_sz);
                return const_cast<char*>(sd_shd_i->_vs_code.c_str());
                });
        }
        string fs_shader_file = sd_dir_path + fs_shader_name;
        if (!fileExist(fs_shader_file.c_str())) {
            sd_shd_i->_fs_code = fs_default_code;
            ofstream fout(fs_shader_file);
            fout << sd_shd_i->_fs_code;
        }
        else {
            get_file_data(vs_shader_file, [&](int file_sz)->char* {
                sd_shd_i->_fs_code.resize(file_sz);
                return const_cast<char*>(sd_shd_i->_fs_code.c_str());
                });
        }
        new_sd_shader= sd_shd_i;
        first_call = true;
        ImGui::CloseCurrentPopup();
    }
    if (!be_enabled) {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
    }
    return new_sd_shader;
}

void shader_ide::edit(shader_ide& sd_u) {
    if (ImGui::Button("Reload")) {
        string vs_shd_path =
            g_current_directory + shaders_fold + sd_u._vs_name;

        get_file_data(vs_shd_path, [&](int file_sz) {
            sd_u._vs_code.resize(file_sz);
            return &sd_u._vs_code[0];
            });
        strcpy(vs_code, sd_u._vs_code.c_str());
        vs_code[sd_u._vs_code.size()] = 0;

        string fs_shd_path =
            g_current_directory + shaders_fold + sd_u._fs_name;

        get_file_data(fs_shd_path, [&](int file_sz) {
            sd_u._fs_code.resize(file_sz);
            return &sd_u._fs_code[0];
            });
        strcpy(fs_code, sd_u._fs_code.c_str());
        fs_code[sd_u._fs_code.size()] = 0;
    }
    ImGui::SameLine(300);
    if (ImGui::Button("Compile")) {
        sd_u.build_vs_code(sd_u._vs_code.c_str());
        sd_u.build_fs_code(sd_u._fs_code.c_str());
        if (sd_u.link()) {
            sd_u._vs_code = vs_code;
            sd_u._fs_code = fs_code;
        }
    }
    ImGui::BeginChild("shader code", ImVec2(0, 0), true);
    ImGui::Columns(2);

    auto edit_ht = ImGui::GetTextLineHeight() * 32;
    auto txt_flag = ImGuiInputTextFlags_AllowTabInput;
    if (!sd_u.vs_valid()) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1., 0., 0., 1.));
    }
    ImGui::InputTextMultiline("Vertex Code:", vs_code, TXT_BUFF_SZ,
        ImVec2(-1.0f, edit_ht), txt_flag);
    if (!sd_u.vs_valid()) {
        ImGui::PopStyleColor();
    }
    ImGui::NextColumn();
    if (!sd_u.fs_valid()) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1., 0., 0., 1.));
    }
    ImGui::InputTextMultiline("Fragment Code:", fs_code, TXT_BUFF_SZ,
        ImVec2(-1.0f, edit_ht), txt_flag);
    if (!sd_u.fs_valid()) {
        ImGui::PopStyleColor();
    }
    ImGui::EndChild();
}
} // namespace vg