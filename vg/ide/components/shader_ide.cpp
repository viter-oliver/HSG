#include "shader_ide.h"
#include "common_functions.h"
#include "dir_output.h"
#include "ide_common_functions.h"
#include "material_ide.h"
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
namespace shader {

    void shader_ide::select_item(std::string& key) {
        strcpy(vs_code, _vs_code.c_str());
        vs_code[_vs_code.size()] = 0;
        strcpy(fs_code, _fs_code.c_str());
        fs_code[_fs_code.size()] = 0;
    }
    void shader_ide::edit_item() {
        if (ImGui::Button("Reload")) {
            string vs_shd_path =
                g_current_directory + shaders_fold + _vs_name;
            
            get_file_data(vs_shd_path, [&](int file_sz) {
                _vs_code.resize(file_sz);
                return &_vs_code[0];
                });
            strcpy(vs_code, _vs_code.c_str());
            vs_code[_vs_code.size()] = 0;

            string fs_shd_path =
                g_current_directory + shaders_fold + _fs_name;

            get_file_data(fs_shd_path, [&](int file_sz) {
                _fs_code.resize(file_sz);
                return &_fs_code[0];
                });
            strcpy(fs_code, _fs_code.c_str());
            fs_code[_fs_code.size()] = 0;
        }
        ImGui::SameLine(300);
        if (ImGui::Button("Compile")) {
            build_vs_code(vs_code);
            build_fs_code(fs_code);
            if (link()) {
                _vs_code = vs_code;
                _fs_code = fs_code;
            }
        }
        ImGui::BeginChild("shader code", ImVec2(0, 0), true);
        ImGui::Columns(2);

        auto edit_ht = ImGui::GetTextLineHeight() * 32;
        auto txt_flag = ImGuiInputTextFlags_AllowTabInput;
        if (!vs_valid()) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1., 0., 0., 1.));
        }
        ImGui::InputTextMultiline("Vertex Code:", vs_code, TXT_BUFF_SZ,
            ImVec2(-1.0f, edit_ht), txt_flag);
        if (!vs_valid()) {
            ImGui::PopStyleColor();
        }
        ImGui::NextColumn();
        if (!fs_valid()) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1., 0., 0., 1.));
        }
        ImGui::InputTextMultiline("Fragment Code:", fs_code, TXT_BUFF_SZ,
            ImVec2(-1.0f, edit_ht), txt_flag);
        if (!fs_valid()) {
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();
    }
}
void create_item(shader::mp_sd_shader_ide& shader_list) {
    static char shader_name[FILE_NAME_LEN] = { 0 };
    static char vs_shader_name[FILE_NAME_LEN] = { 0 };
    static char fs_shader_name[FILE_NAME_LEN] = { 0 };
    static bool first_call = true;
    if (first_call) {
        shader_name[0] = 0;
        vs_shader_name[0] = 0;
        fs_shader_name[0] = 0;
        first_call = false;
    }
    ImGui::InputText("Shader name:", shader_name, FILE_NAME_LEN);
    ImGui::InputText("Vertex shader file:", vs_shader_name, FILE_NAME_LEN);
    ImGui::SameLine();
    if (ImGui::Button("...")) {
        OPENFILENAME ofn = { sizeof(OPENFILENAME) };
        ofn.hwndOwner = GetForegroundWindow();
        // ofn.lpstrFilter = "ttf file:\0*.ttf\0\0";
        char strFileName[MAX_PATH] = { 0 };
        ofn.nFilterIndex = 1;
        ofn.lpstrFile = strFileName;
        ofn.nMaxFile = sizeof(strFileName);
        ofn.lpstrTitle = "Loading vertex shader file...";
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
        if (GetOpenFileName(&ofn)) {
            string sd_dir_path = g_current_directory + shaders_fold;
            string sd_file_path = strFileName;
            string sd_src_file_name =
                sd_file_path.substr(sd_file_path.find_last_of('\\') + 1);
            string sd_test_full_name = sd_dir_path + sd_src_file_name;
            getUniqueFileName(sd_test_full_name);
            string str_cmd = "copy " + sd_file_path + " " + sd_test_full_name;
            system(str_cmd.c_str());
            string sd_file_name = sd_test_full_name.substr(sd_dir_path.size());
            strcpy(vs_shader_name, sd_file_name.c_str());
            vs_shader_name[sd_file_name.size()] = 0;

            get_file_data(sd_test_full_name, [&](int file_sz) {
                vs_code[file_sz] = 0;
                return vs_code;
                });
        }
    }

    ImGui::InputText("Fragment shader file:", fs_shader_name, FILE_NAME_LEN);
    ImGui::SameLine();
    if (ImGui::Button("Load fragment shader file...")) {
        OPENFILENAME ofn = { sizeof(OPENFILENAME) };
        ofn.hwndOwner = GetForegroundWindow();
        // ofn.lpstrFilter = "ttf file:\0*.ttf\0\0";
        char strFileName[MAX_PATH] = { 0 };
        ofn.nFilterIndex = 1;
        ofn.lpstrFile = strFileName;
        ofn.nMaxFile = sizeof(strFileName);
        ofn.lpstrTitle = "Loading fragment file...";
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
        if (GetOpenFileName(&ofn)) {
            string sd_dir_path = g_current_directory + shaders_fold;
            string sd_file_path = strFileName;
            string sd_src_file_name =
                sd_file_path.substr(sd_file_path.find_last_of('\\') + 1);
            string sd_test_full_name = sd_dir_path + sd_src_file_name;
            getUniqueFileName(sd_test_full_name);
            string str_cmd = "copy " + sd_file_path + " " + sd_test_full_name;
            system(str_cmd.c_str());
            string sd_file_name = sd_test_full_name.substr(sd_dir_path.size());
            strcpy(fs_shader_name, sd_file_name.c_str());
            fs_shader_name[sd_file_name.size()] = 0;
            get_file_data(sd_test_full_name, [&](int file_sz) {
                fs_code[file_sz] = 0;
                return fs_code;
                });
        }
}
    bool be_enabled = strlen(shader_name) > 0 && strlen(vs_shader_name) > 0 &&
        strlen(fs_shader_name) > 0;
    if (!be_enabled) {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    if (ImGui::Button("New Shader...")) {
        string shader_test_name = shader_name;
        while (shader_list.find(shader_test_name) != shader_list.end()) {
            shader_test_name += "_";
        }
        auto sd_shd_i = make_shared<shader::shader_ide>();
        string sd_dir_path = g_current_directory + shaders_fold;
        string vs_shader_file = sd_dir_path + vs_shader_name;
        if (!fileExist(vs_shader_file.c_str())) {
            _vs_code = vs_default_code;
        }
        else {
            get_file_data(vs_shader_file, [&](int file_sz) {
                _vs_code.resize(file_sz);
                return const_cast<char*>(_vs_code.c_str());
                });
        }
        string fs_shader_file = sd_dir_path + fs_shader_name;
        if (!fileExist(fs_shader_file.c_str())) {
            _fs_code = fs_default_code;
        }
        else {
            get_file_data(vs_shader_file, [&](int file_sz) {
                _fs_code.resize(file_sz);
                return const_cast<char*>(_fs_code.c_str());
                });
        }
        _vs_name = vs_shader_name;
        _fs_name = fs_shader_name;
        shader_list[shader_test_name] = sd_shd_i;
        first_call = true;
        ImGui::CloseCurrentPopup();
    }
    if (!be_enabled) {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
}
} // namespace vg