
#pragma once
#include "user_control_imgui.h"
#include"shader_utils.h"
#include "shader_class_producer.h"
namespace vg {
    using namespace shader;

    extern std::string g_cureent_directory;
    extern const char* g_single_txt_vs;
    extern const char* g_single_txt_fs;

    extern TextEditor vs_editor;
    extern TextEditor fs_editor;

    extern TextEditor vs_debug_editor;
    extern TextEditor fs_debug_editor;

    extern std::string vs_str;
    extern std::string fs_str;
    const char *shaders_fold = "shaders\\";
    template<>
    bool create_new_pair<shader_base>(shader_base_key& skey, sd_shader_base& sd_sb) {
        std::string file_path{};
        std::string file_name{};
        bool res = 0;
        if (ImGui::Button("New Shader..."))
        {
            ImGui::OpenPopup("create shader");
        }

        if (ImGui::BeginPopupModal("create shader", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            static char shd_name_str[FILE_NAME_LEN] = "";
            std::string selected_file_path;
            std::string selected_file_name;

            //填充逻辑文件夹名称
            ImGui::InputText("shader name", shd_name_str, FILE_NAME_LEN);
            skey.get_str_value(shd_name_str);

            bool be_enabled = strlen(shd_name_str) > 0;
            if (!be_enabled)
            {
                ImGui::BeginDisabled(true);
                //ImGui::PushItemFlag(ImGuiItemFlags_None, true);
                //ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            }

            if (ImGui::Button("add new shader"))
            {
                std::string str_shader_file = g_cureent_directory + shaders_fold;
                std::string vs_code, fs_code;

                vs_code = g_default_vs_txt;
                fs_code = g_default_fs_txt;

                sd_sb = std::make_shared<shader_base>();
                shader_base& pshd = *sd_sb;

                pshd.build_vs_code(vs_code.c_str());
                pshd.build_fs_code(fs_code.c_str());

                if (pshd.fs_valid() && pshd.vs_valid())
                {
                    pshd.link();
                }
                if (pshd.valid())
                {
                    //gray@2024.08.05 todo::问下张工 
                    //gray@2024.08.13 comment::后面再说
                    //pshd.refresh_viarable_list();
                }
                memset(shd_name_str, 0, FILE_NAME_LEN);
                ImGui::CloseCurrentPopup();
                res = 1;
            }
            if (!be_enabled)
            {
                ImGui::EndDisabled();
                //ImGui::PopItemFlag();
                //ImGui::PopStyleVar(); 
            }
            ImGui::SameLine();
            if (ImGui::Button("cancel"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
            return res;
        }

        return 0;
    }

    template<>
    void res_edit<shader_base>(shader_base& shader)
    {
        static shader_base* last_shader = nullptr;
        static shader_base* last_last_shader = nullptr;

        static std::string last_vs_str{};
        static std::string last_fs_str{};

        // 获取当前窗口的可用区域大小
        ImVec2 avail_size = ImGui::GetContentRegionAvail();
        static float total_height = avail_size.y;  // 初始化 total_height

        // 每次绘制时更新 total_height
        total_height = avail_size.y;

        /*
        //gray@2024.09.12 comment::
        //用于TextEditor颜色跟随主题测试，待后续张工写完回调之后补充进去
        {
            static std::vector<float> f0{ 0.75f,0.95f }; // 定义一个静态浮点变量 f0，并初始化为 0.001
            ImGui::InputFloat("alpha", &f0[0], 0.01f, 0.1f, "%.3f");
            ImGui::InputFloat("gray", &f0[1], 0.01f, 0.1f, "%.3f");
            static bool init_editor_theme_flag = 1;
            if (init_editor_theme_flag)
            {
                //init_editor_theme(vs_editor);
                //init_editor_theme(fs_editor);
                //init_editor_theme(vs_debug_editor);
                //init_editor_theme(fs_debug_editor);

                init_editor_theme(vs_editor, f0);
                init_editor_theme(fs_editor, f0);
                init_editor_theme(vs_debug_editor, f0);
                init_editor_theme(fs_debug_editor, f0);
                //init_editor_theme_flag = 0;
            }
        }
        */
        
        

        //退出保存逻辑
        {
            if (last_shader != &shader)
            {
                if (last_shader != nullptr)
                {
                    last_vs_str = get_editor_text(vs_editor);
                    last_fs_str = get_editor_text(fs_editor);

                    if (vs_editor.CanUndo())
                    {
                        ImGui::OpenPopup("ModalPopup");
                    }
                }
            }

            //退出保存弹出窗口
            if (ImGui::BeginPopupModal("ModalPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Do you want to save changes?");
                ImGui::Separator();
                if (ImGui::Button("Yes")) {
                    //save_vs_source(*last_last_shader, last_vs_str);
                    //save_fs_source(*last_last_shader, last_fs_str);
                    last_last_shader->set_source(ShaderTargetType::VERTEX_SHADER, last_vs_str);
                    last_last_shader->set_source(ShaderTargetType::FRAGMENT_SHADER, last_fs_str);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();

                if (ImGui::Button("No")) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        //初始情况和切换列表项情况下资源的设置（主要是两个代码框）
        if (last_shader != &shader)
        {
            //if (last_shader != nullptr)
            {
                //vs_editor.SetText(get_vs_source(shader));
                //fs_editor.SetText(get_fs_source(shader));
                std::string vs_source{};
                std::string fs_source{};
                shader.get_source(ShaderTargetType::VERTEX_SHADER, vs_source);
                vs_editor.SetText(vs_source);
                vs_editor.SetErrorMarkers({});
                
                shader.get_source(ShaderTargetType::FRAGMENT_SHADER, fs_source);
                fs_editor.SetText(fs_source);
                fs_editor.SetErrorMarkers({});

            }
            last_last_shader = last_shader;
            last_shader = &shader;
        }

        {
            if (ImGui::Button("Link##vs_link"))
            {
                shader_link(shader);
            }
        }

        // 开始子窗口
        ImGui::BeginChild("shader_main");

        // 划分为左右两列
        ImGui::Columns(2);

        std::string selected_file_path;
        std::string selected_file_name;


        // 左侧：Vertex Shader 窗口
        ImGui::BeginChild("VertexShader");
        {
            // 按钮部分
            {
                if (ImGui::Button("Load##vs_load"))
                {
                    shader_load(ShaderTargetType::VERTEX_SHADER, selected_file_path, selected_file_name);
                   
                }
                ImGui::SameLine();
                if (ImGui::Button("Save##vs_save"))
                {
                    //shader.set_source(ShaderTargetType::VERTEX_SHADER, get_editor_text(vs_editor));
                    shader_save(ShaderTargetType::VERTEX_SHADER,shader);

                }
                ImGui::SameLine();
                if (ImGui::Button("Compile##vs_compile"))
                {
                    shader_compile(ShaderTargetType::VERTEX_SHADER, shader);

                }
                ImGui::SameLine();
               
                if (ImGui::Button("Save&Compile##vs_save_copile"))
                {
                    shader_save(ShaderTargetType::VERTEX_SHADER, shader);
                    shader_compile(ShaderTargetType::VERTEX_SHADER, shader);
                }
            }

            // 代码编辑部分
            {
                static float code_height{};
                static float compile_info_height{};

                // 初始化界面布局
                static bool init_flag = 1;
                if (init_flag)
                {
                    code_height = total_height * 0.85f;
                    compile_info_height = total_height * 0.15f;
                    init_flag = 0;
                }

                // 代码文本框
                ImGui::BeginChild("CodeEditor##vs", ImVec2(0, code_height));
                ImGui::Text("Vertex Shader Code:");
                vs_editor.Render("##vs_code");
                ImGui::EndChild();

                //自定义垂直分割器
                vertical_splitter(code_height, compile_info_height, total_height);

                // 编译信息框
                ImGui::BeginChild("CompileInfo##vs", ImVec2(0, compile_info_height));
                ImGui::Text("Compile Info:");
                vs_debug_editor.Render("##vs_debug_code");
                ImGui::EndChild();
            }
        }
        ImGui::EndChild();//Vertex Shader 窗口

        ImGui::SameLine();
        ImGui::NextColumn();

        // 右侧：Fragment Shader 窗口
        ImGui::BeginChild("FragmentShader");
        {

            // 按钮部分
            {
                if (ImGui::Button("Load##fs_load"))
                {
                    shader_load(ShaderTargetType::FRAGMENT_SHADER, selected_file_path, selected_file_name);
                }
                ImGui::SameLine();
                if (ImGui::Button("Save##fs_save"))
                {
                    //shader.set_source(ShaderTargetType::FRAGMENT_SHADER, get_editor_text(fs_editor));
                    shader_save(ShaderTargetType::FRAGMENT_SHADER, shader);

                }
                ImGui::SameLine();
                if (ImGui::Button("Compile##fs_compile"))
                {
                    shader_compile(ShaderTargetType::FRAGMENT_SHADER, shader);
                }

                ImGui::SameLine();
                if (ImGui::Button("Save&Compile##fs_save_copile"))
                {
                    shader_save(ShaderTargetType::FRAGMENT_SHADER, shader);
                    shader_compile(ShaderTargetType::FRAGMENT_SHADER, shader);
                }

            }

            // 代码编辑部分
            {
                static float code_height{};
                static float compile_info_height{};

                // 初始化界面布局
                static bool init_flag = 1;
                if (init_flag)
                {
                    code_height = total_height * 0.85f;
                    compile_info_height = total_height * 0.15f;
                    init_flag = 0;
                }

                //代码文本框
                ImGui::BeginChild("CodeEditor##fs", ImVec2(0, code_height));
                ImGui::Text("Fragment Shader Code:");
                fs_editor.Render("##fs_code");
                ImGui::EndChild();

                //自定义垂直分割器
                vertical_splitter(code_height, compile_info_height, total_height);

                // 编译信息框
                ImGui::BeginChild("CompileInfo##fs", ImVec2(0, compile_info_height));
                ImGui::Text("Compile Info:");
                fs_debug_editor.Render("##fs_debug_code");
                ImGui::EndChild();
            }
        }
        ImGui::EndChild(); // Fragment Shader 窗口

        ImGui::EndChild(); // shader_main 窗口
    }

    REG_MAP_COPONENT_EDIT_CONTAINER(shader_base)
    inline struct shader_base_init{
        shader_base_init() { 
          auto &edit = *shader_base_assist_._sd_o;
            edit("Create Shader Class...", [](shader_base &sd_,shader_base_key& key_) {
                 std::string key_str = key_.value;
                 produce_shader_class(sd_, key_str);
            });
        }
    } shader_base_init_;
    }