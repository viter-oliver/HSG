#include <fstream>
#include <algorithm>
#include "vg_type.h"
#include "shader_class_producer.h"
#include "common_functions.h"

namespace vg{
using namespace std;
extern string g_cureent_directory;
extern string g_sdk_path;
#ifdef _DEBUG
#define TARGET_NAME "HSG_TARGETd.dll"
#else
#define TARGET_NAME "HSG_TARGET.dll"
#endif // DEBUG
const char *plugin = RUNNING_PATH CMAKE_INTDIR "//" TARGET_NAME;
const char *usr_plugin_dir = "hsg_plugin/";
const char *usr_project_dir = "usr_project/";
const char *usr_cmake_dir = "busr_project";
using namespace shader;
void produce_shader_class(shader::shader_base &sd, string &key_str) {
    //to  ensure usr_plugin_dir and usr_project_dir exists
    string plugin_dir = g_cureent_directory + usr_plugin_dir;
    if (!directoryExist(plugin_dir.c_str())){
      createDirectory(plugin_dir.c_str());
    }
    replace(plugin_dir.begin(), plugin_dir.end(), '\\', '/');
    replace(g_sdk_path.begin(), g_sdk_path.end(), '\\', '/');
    string project_dir = g_cureent_directory + usr_project_dir;
    if (!directoryExist(project_dir.c_str())) {
      createDirectory(project_dir.c_str());
    }
    string cmake_project_dir = g_cureent_directory + usr_cmake_dir;
    if (!directoryExist(cmake_project_dir.c_str())) {
      createDirectory(cmake_project_dir.c_str());
    }
    replace(cmake_project_dir.begin(),
                              cmake_project_dir.end(), '\\',
                              '/');
   
    //produce shader_class
    using uniform_list = map<string, string>;
    const uniform_list gl_type_to_vg_type = {
        {"iGL_FLOAT", "float"},           {"iGL_FLOAT_VEC2", "f32_2"},
        {"iGL_FLOAT_VEC3", "f32_3"},    {"iGL_FLOAT_VEC4", "f32_4"},
        {"iGL_FLOAT_MAT2", "f32_4"},    {"iGL_FLOAT_MAT3", "f32_9"},
        {"iGL_FLOAT_MAT4", "f32_16"},   {"iGL_FLOAT_MAT2x3", "f32_6"},
        {"iGL_FLOAT_MAT3x2", "f32_6"},  {"iGL_FLOAT_MAT2x4", "f32_8"},
        {"iGL_FLOAT_MAT4x2", "f32_8"},  {"iGL_FLOAT_MAT3x4", "f32_12"},
        {"iGL_FLOAT_MAT4x3", "f32_12"}, {"iGL_INT", "s32"},
        {"iGL_INT_VEC2", "s32_2"},        {"iGL_INT_VEC3", "s32_3"},
        {"iGL_INT_VEC4", "s32_4"},        {"iGL_BOOL", "bool"},
        {"iGL_UNSIGNED_INT", "u32"},      {"iGL_SAMPLER_2D", "texture_key"},

    };
    uniform_list uniform_list_;
    u8 ele_3d_ch = 0;
    string texture_name;
    bool has_texture = false;
    sd.iterate_uniforms([&](const string &key,
                            sd_shader_variable_type sd_vtype) {

#define _PIKSTR(x) #x
#define PIKSTR(x) _PIKSTR(x)
#define PICK_NAME(x) x
#define CHECK_TYPE(x)                                                          \
    auto sd_##x##_type = dynamic_pointer_cast<sd_vtype_##x>(sd_vtype);         \
    if (sd_##x##_type) {                                                       \
      auto itype = gl_type_to_vg_type.find(PIKSTR(x));                         \
      if (itype != gl_type_to_vg_type.end()) {                                 \
        uniform_list_[key] = itype->second;                                    \
        if (key == "model" && itype->second == "f32_16")                     \
          ele_3d_ch++;                                                         \
        if (key == "view" && itype->second == "f32_16")                      \
          ele_3d_ch++;                                                         \
        if (key == "projection" && itype->second == "f32_16")                \
          ele_3d_ch++;                                                         \
        if (itype->second == "texture_key") {                                  \
          has_texture = true;                                                  \
        }                                                                      \
      }                                                                        \
    }
#define PICK_TYPE_2_CHECK(r, data, elem) CHECK_TYPE(elem)
#define CHECK_TYPE_LIST(...)                                                   \
    BOOST_PP_SEQ_FOR_EACH(PICK_TYPE_2_CHECK, _,                                \
                          BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
      CHECK_TYPE_LIST(iGL_FLOAT, iGL_FLOAT_VEC2, iGL_FLOAT_VEC3, iGL_FLOAT_VEC4,
                      iGL_FLOAT_MAT2, iGL_FLOAT_MAT3, iGL_FLOAT_MAT4,
                      iGL_FLOAT_MAT2x3, iGL_FLOAT_MAT3x2, iGL_FLOAT_MAT2x4,
                      iGL_FLOAT_MAT4x2, iGL_FLOAT_MAT3x4, iGL_FLOAT_MAT4x3,
                      iGL_INT, iGL_INT_VEC2, iGL_INT_VEC3, iGL_INT_VEC4,
                      iGL_BOOL, iGL_UNSIGNED_INT, iGL_SAMPLER_2D)
    });
    string parent_class("shader_control");
    if (ele_3d_ch == 3) {
      parent_class = "shader_3d";
      uniform_list_.erase("model");
      uniform_list_.erase("view");
      uniform_list_.erase("projection");
    }
    string shader_class_h = project_dir + key_str + ".hpp";
    ofstream ofs_sd(shader_class_h);
#define WEIRTE_SD_LINE(x) ofs_sd<<x<<endl;
    WEIRTE_SD_LINE ("#pragma once")
    WEIRTE_SD_LINE( "#include\"" << parent_class << ".h\"")
    if (has_texture) {
      WEIRTE_SD_LINE( "#include\"vg_texture.h\"")
    }
    WEIRTE_SD_LINE("namespace vg{");
    WEIRTE_SD_LINE("class " << key_str << ":public " << parent_class << "{")
    WEIRTE_SD_LINE("DEF_STRUCT_WITH_INIT(_sd_pt")
    for (auto &[key, value] : uniform_list_) {
      ofs_sd << ",(" << value << "," << key << ")" << endl;
    }
    WEIRTE_SD_LINE( ")" )
    WEIRTE_SD_LINE("};")
    WEIRTE_SD_LINE("REG_CTL_2_FAC(" << key_str << ");" )
    WEIRTE_SD_LINE("}" )
    ofs_sd.close();
    string shader_class_s = project_dir + key_str + ".cpp";
    ofstream ofs_sd_s(shader_class_s);
    ofs_sd_s << "#include \"" << key_str << ".hpp\"" << endl;
    ofs_sd_s.close();
    //produce cmakelists.txt
    string str_cmakelists = project_dir + "CMakeLists.txt";
    ofstream fs_cm(str_cmakelists);
#define WRITE_CM_LINE(x) fs_cm << x << endl;
    WRITE_CM_LINE("cmake_minimum_required(VERSION 3.14)")
    WRITE_CM_LINE("project(usr_hsg C CXX)")
    WRITE_CM_LINE("set(CMAKE_CXX_STANDARD 20)")
    WRITE_CM_LINE("include_directories(")
    WRITE_CM_LINE(g_sdk_path << "include/components/include")
    WRITE_CM_LINE(g_sdk_path << "include/controls/include")
    WRITE_CM_LINE(g_sdk_path << "include")
    WRITE_CM_LINE(")")
    WRITE_CM_LINE("link_directories(")
    WRITE_CM_LINE(g_sdk_path)
    WRITE_CM_LINE(")")
    WRITE_CM_LINE("file(GLOB CP_HD *.h *.hpp)")
    WRITE_CM_LINE("file(GLOB CP_SRC *.cpp *.cc)")
    WRITE_CM_LINE("add_library(HSG_TARGET_usr SHARED ${CP_SRC} ${CP_HD})")
    WRITE_CM_LINE("if(CMAKE_BUILD_TYPE STREQUAL \"Debug\")")
    WRITE_CM_LINE("set(LINK_TAR \"HSG_TARGETd\")")
    WRITE_CM_LINE("elseif(CMAKE_BUILD_TYPE STREQUAL \"Release\")")
    WRITE_CM_LINE("set(LINK_TAR \"HSG_TARGET\")")
    WRITE_CM_LINE("else()")
    WRITE_CM_LINE("set(LINK_TAR \"HSG_TARGETd\")")
    WRITE_CM_LINE("endif()")
    WRITE_CM_LINE("target_link_libraries(HSG_TARGET_usr ${LINK_TAR})")
    WRITE_CM_LINE("set_target_properties(HSG_TARGET_usr PROPERTIES")
    WRITE_CM_LINE("LIBRARY_OUTPUT_DIRECTORY " << plugin_dir)
    WRITE_CM_LINE("LINKER_LANGUAGE CXX")
    WRITE_CM_LINE(")")
    fs_cm.close();
    //execute cmake command to produce usr project
    string to_cmake_dir = "cd " +cmake_project_dir;
    //system(to_cmake_dir.c_str());
    string cmake_cmd = "cmake ../usr_project";
    string cmd = to_cmake_dir + " && " + cmake_cmd;
    int result=system(cmd.c_str());
    if (result == 0) {
      printf("CMake executed successfully!\n");
    } else {
      printf("CMake execution failed with error code:%d\n",result);
    }
  }
}