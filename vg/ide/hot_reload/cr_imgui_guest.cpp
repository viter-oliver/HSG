#include <string>
#include "cr.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/gl3w.h> 
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <windows.h>
#include <Shlwapi.h>
#include <locale.h>
#include <ShlObj.h>
#include <Commdlg.h>
#include "file_open_chain.hpp"
#include "controls.h"
#include "components.h"
#include "project.h"
#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#include "resource.h"
#endif
#include "res_internal.h"
#include "internal_shader.h"
#include "user_style_pool.h"
#include "ide_events.h"
#include "from_lanchor.h"
#include "RegImGuiEditors.h"
#include "components_fun.h"
using namespace vg;
struct open_hsg :public IFileOpener {
    bool tryOpen(const std::string& path) {
        size_t pos = path.find_last_of('.');
        if (pos != std::string::npos) {
            std::string ext = path.substr(pos + 1);
            if (ext == "hsg") {
                open_project(path.c_str());
                return  true;
            }
        }
        return false;
    }
};

HINSTANCE g_cur_instance;
static void error_callback(int error, const char *description)
{
     fprintf(stderr, "Error %d: %s\n", error, description);
}
GLFWwindow *g_window;
FileOpenChain g_file_open_chain;
std::string project_path;
HINSTANCE hInst; // DLLÊµÀý¾ä±ú
//extern void vg::force_link_ImGuiEditors();
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    vg::force_link_ImGuiEditors();
     if (fdwReason == DLL_PROCESS_ATTACH) {
       hInst = hinstDLL;
     }
     return TRUE;
}
int  glfw_init(){
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;
	//register file open handlers
	g_file_open_chain.addHandler(std::make_shared<open_hsg>());
	g_file_open_chain.addHandler(std::make_shared<open_texture>());
	g_file_open_chain.addHandler(std::make_shared<open_font>());
	g_file_open_chain.addHandler(std::make_shared<open_vertex>());
	g_file_open_chain.addHandler(std::make_shared<open_models>());
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
    GLFWmonitor *pmornitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(pmornitor);
    int iw, ih;
    iw = mode->width;
    ih = mode->height;
    //printf("x%", this);
    GLFWwindow *window = glfwCreateWindow(iw, ih, "HardStoneGraphics designer", NULL, NULL);
    g_window=window;
    HWND hwnd_main = glfwGetWin32Window(window);
    SendMessage(hwnd_main, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)));
    if( !window )
    {
         glfwTerminate();
         return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    execute_event(prepare_opegl_context);
    gl3wInit();   
    glfwSetDropCallback( window, [](GLFWwindow *wh, int cnt, const char **fpaths){
        for (int ix = 0; ix < cnt; ix++) {
			std::string fp = fpaths[ix];
            //find extension
            g_file_open_chain.open(fp);
        }
    });
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    io.ConfigFlags |= ImGuiCol_DockingEmptyBg;
    ImGui::StyleColorsClassic();
    // When viewports are enabled we tweak WindowRounding / WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle & style = ImGui::GetStyle();
    //style.Colors[ImGuiCol_TableRowBg] =ImVec4(0.30f, 0.00f, 0.10f, 1.00f);
    if( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
    {
         style.WindowRounding = 0.0f;
         style.Colors[ ImGuiCol_WindowBg ].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL( window, true );
    ImGui_ImplOpenGL3_Init( glsl_version );
    internal::init_texture_from_rc();
    shader::initialize_internal_shader();
    if (!project_path.empty()) {
         open_project(project_path.c_str());
    }
    return 0;
}
void imgui_render(){
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport();
    //imgui controls
    if (ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File")){
            if (ImGui::MenuItem("New Project", "Ctrl+N")){
                //open_project();clear all of resources and controls
                for( auto& ele : v_sd_base_component_edit_ ){
                    ele->clear();
                }
            }
            if (ImGui::MenuItem("Open Project...", "Ctrl+O")){
                open_project();
            }
            if (ImGui::BeginMenu("Open Recent")){
                ImGui::MenuItem("haima.afg");
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")){
                save_project();
            }
            if (ImGui::BeginMenu("Preference")) {
                if (ImGui::BeginMenu("Theme")) {
                    static int style_idx = 0;
#define _PIKSTR(x) #x
#define PIKSTR(x) _PIKSTR(x)
#define themeselector(item, expression)                                        \
    if (ImGui::RadioButton(PIKSTR(item), style_idx == item)) {                 \
        style_idx = item;                                                      \
        execute_event(before_style_select);\
        expression                                                             \
        execute_event(after_stle_select);\
    }
#define GEN_V5(r, data, elem)                                                  \
    themeselector(BOOST_PP_TUPLE_ELEM(2, 0, elem),                             \
                  BOOST_PP_TUPLE_ELEM(2, 1, elem))
#define DEF_EUM_I_E(i, x) x,
#define GEN_V4(r, data, i, elem) DEF_EUM_I_E(i, BOOST_PP_TUPLE_ELEM(2, 0, elem))
#define themeselector_ex(...)                                                  \
    enum {                                                                     \
      BOOST_PP_SEQ_FOR_EACH_I(GEN_V4, _,                                       \
                              BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))           \
    };                                                                         \
    BOOST_PP_SEQ_FOR_EACH(GEN_V5, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

                    themeselector_ex((Classic, ImGui::StyleColorsClassic();),
                                     (Cream, StyleColorsCream();),
                                     (Forest, StyleColorsForest();),
                                     (Dark, ImGui::StyleColorsDark();),
                                     (Light, ImGui::StyleColorsLight();))

                        ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("ResourceManager")) {
            for( auto& ele:v_sd_base_component_edit_){
                ele->menu_item();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Controls")) {
            project_menu_items();
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    for( auto& ele:v_sd_base_component_edit_){
        ele->show();
    }
    project_viewer();
    ImGui::Render();
    //....
    int display_w, display_h;
    glfwGetFramebufferSize(g_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    if( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent( backup_current_context );
    }
    glfwSwapBuffers(g_window);
}
void imgui_cleanup(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow( g_window );
    glfwTerminate();
}
CR_EXPORT int cr_main(cr_plugin *ctx, cr_op operation) {
    assert(ctx);
   
    if (ctx->userdata) {
        from_lanchor *fl = (from_lanchor *)ctx->userdata;
        project_path = fl->project_file;
        string sdk_path = fl->executable;
        sdk_path = sdk_path.substr(0, sdk_path.find_last_of('\\') + 1);
        set_sdk_path(sdk_path.c_str());
    }

    switch (operation) {
        case CR_LOAD:
            
            return glfw_init();
        case CR_UNLOAD:
            // if needed, save stuff to pass over to next instance
            return 0;
        case CR_CLOSE:
            execute_event(before_exit_app);
            imgui_cleanup();
            return 0;
        case CR_STEP:
            {
            
                if(glfwWindowShouldClose(g_window))
                {
                    return 1;
                }
                if (glfwGetWindowAttrib(g_window, GLFW_ICONIFIED))
                {
                    glfwWaitEvents();
                    return 0;
                }
                imgui_render();
            }
            return 0;
    }

    return 0;
}