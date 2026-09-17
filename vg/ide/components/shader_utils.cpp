#include "shader_utils.h"
namespace vg {

    const char* g_default_vs_txt = R"glsl(#version 300 es
	precision mediump float;
	in vec3 position;
	in vec3 normal;
	in vec2 textCoord;

	out vec2 TextCoord;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	uniform float voffset;
	void main()
	{
		vec3 cpos = position * length(normal);
		gl_Position = projection * view * model * vec4(cpos, 1.0);
		TextCoord = vec2(textCoord.x,textCoord.y + voffset);
	}
	)glsl";

    const char* g_default_fs_txt = R"glsl(#version 300 es
	precision mediump float;
	in vec2 TextCoord;
	uniform sampler2D tex1;
	out vec4 color;
	void main()
	{
		color = texture(tex1, TextCoord);
	}
	)glsl";


    const char* const keywords[] = {
    "attribute", "const", "uniform", "varying", "layout", "centroid", "flat", "smooth", "noperspective",
    "break", "continue", "do", "for", "while", "switch", "case", "default", "if", "else",
    "subroutine", "in", "out", "inout", "float", "double", "int", "void", "bool", "true", "false",
    "invariant", "discard", "return", "mat2", "mat3", "mat4", "dmat2", "dmat3", "dmat4",
    "mat2x2", "mat2x3", "mat2x4", "mat3x2", "mat3x3", "mat3x4", "mat4x2", "mat4x3", "mat4x4",
    "vec2", "vec3", "vec4", "ivec2", "ivec3", "ivec4", "bvec2", "bvec3", "bvec4", "uint",
    "uvec2", "uvec3", "uvec4", "lowp", "mediump", "highp", "precision", "sampler1D", "sampler2D",
    "sampler3D", "samplerCube", "sampler1DShadow", "sampler2DShadow", "samplerCubeShadow",
    "sampler1DArray", "sampler2DArray", "sampler1DArrayShadow", "sampler2DArrayShadow",
    "isampler1D", "isampler2D", "isampler3D", "isamplerCube", "isampler1DArray", "isampler2DArray",
    "usampler1D", "usampler2D", "usampler3D", "usamplerCube", "usampler1DArray", "usampler2DArray",
    "sampler2DRect", "sampler2DRectShadow", "isampler2DRect", "usampler2DRect",
    "samplerBuffer", "isamplerBuffer", "usamplerBuffer", "sampler2DMS", "isampler2DMS",
    "usampler2DMS", "sampler2DMSArray", "isampler2DMSArray", "usampler2DMSArray",
    "samplerCubeArray", "samplerCubeArrayShadow", "isamplerCubeArray", "usamplerCubeArray"
    };

    const char* const identifiers[] = {
    "radians", "degrees", "sin", "cos", "tan", "asin", "acos", "atan",
    "pow", "exp", "log", "exp2", "log2", "sqrt", "inversesqrt",
    "abs", "sign", "floor", "trunc", "round", "roundEven", "ceil", "fract", "mod", "modf",
    "min", "max", "clamp", "mix", "step", "smoothstep",
    "length", "distance", "dot", "cross", "normalize", "faceforward", "reflect", "refract",
    "matrixCompMult", "outerProduct", "transpose", "determinant", "inverse",
    "lessThan", "lessThanEqual", "greaterThan", "greaterThanEqual", "equal", "notEqual",
    "any", "all", "not",
    "textureSize", "texture", "textureProj", "textureLod", "textureGrad",
    "dFdx", "dFdy", "fwidth",
    "noise1", "noise2", "noise3", "noise4",
    "EmitVertex", "EndPrimitive",
    "barrier", "memoryBarrier", "atomicCounterIncrement", "atomicCounterDecrement",
    "atomicCounter", "atomicAdd", "atomicMin", "atomicMax", "atomicAnd", "atomicOr", "atomicXor",
    "atomicExchange", "atomicCompSwap"
    };

    const char* const glsl_builtin_variables[] = {
    "gl_Position", "gl_FragCoord", "gl_FragColor", "gl_FragData",
    "gl_PointCoord", "gl_PointSize", "gl_ClipDistance", "gl_CullDistance",
    "gl_VertexID", "gl_InstanceID", "gl_PrimitiveID", "gl_InvocationID",
    "gl_Layer", "gl_ViewportIndex", "gl_FrontFacing", "gl_FragDepth",
    "gl_NumWorkGroups", "gl_WorkGroupID", "gl_LocalInvocationID", "gl_GlobalInvocationID",
    "gl_LocalInvocationIndex", "gl_WorkGroupSize"
    };

	TextEditor vs_editor{};
	TextEditor fs_editor{};

	TextEditor vs_debug_editor{};
	TextEditor fs_debug_editor{};

    TextEditor::LanguageDefinition lang_GLSL;
    TextEditor::LanguageDefinition lang_CPP;

	constexpr int BUFF_SIZE = 1024;
	std::string vs_str{};
	std::string fs_str{};

    //���Ը�����ʼ������
    inline void init_editor_lang(TextEditor::LanguageDefinition& lang, EditorLangType type)
    {
        if (type == EditorLangType::GLSL)
        {
            lang = TextEditor::LanguageDefinition::GLSL();

            for (auto& k : keywords)
                lang.mKeywords.insert(k);

            for (auto& k : identifiers)
            {
                TextEditor::Identifier id;
                id.mDeclaration = "Built-in function";
                lang.mIdentifiers.insert(std::make_pair(std::string(k), id));
            }

            for (auto& var : glsl_builtin_variables)
            {
                TextEditor::Identifier id;
                id.mDeclaration = "Built-in variable";
                lang.mIdentifiers.insert(std::make_pair(std::string(var), id));
            }
        }
        else if (type == EditorLangType::CPP)
        {
            lang = TextEditor::LanguageDefinition::CPlusPlus();
        }
    }
    //TextEditor������������
    void init_editor_theme2(TextEditor& editor)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        TextEditor::Palette palette;
        
        palette[(int)TextEditor::PaletteIndex::Default] = ImGui::ColorConvertFloat4ToU32(ImColor(colors[ImGuiCol_Text]));                      // Normal text
        palette[(int)TextEditor::PaletteIndex::Comment] = ImGui::ColorConvertFloat4ToU32(ImColor(colors[ImGuiCol_TextDisabled]));              // Comments
        palette[(int)TextEditor::PaletteIndex::MultiLineComment] = ImGui::ColorConvertFloat4ToU32(ImColor(colors[ImGuiCol_TextDisabled]));     // Multiline comments
        palette[(int)TextEditor::PaletteIndex::Background] = ImGui::ColorConvertFloat4ToU32(ImColor(colors[ImGuiCol_WindowBg]));               // Background
        palette[(int)TextEditor::PaletteIndex::Cursor] = ImGui::ColorConvertFloat4ToU32(ImColor(colors[ImGuiCol_TextLink]));                   // Cursor
        palette[(int)TextEditor::PaletteIndex::Selection] = ImGui::ColorConvertFloat4ToU32(ImColor(colors[ImGuiCol_TextSelectedBg]));          // Selection
        palette[(int)TextEditor::PaletteIndex::LineNumber] = ImGui::ColorConvertFloat4ToU32(ImColor(colors[ImGuiCol_ScrollbarGrab]));          // Line numbers
        palette[(int)TextEditor::PaletteIndex::CurrentLineFill] = ImGui::ColorConvertFloat4ToU32(ImColor(colors[ImGuiCol_TitleBg]));           // Current line fill
        palette[(int)TextEditor::PaletteIndex::CurrentLineFillInactive] = ImGui::ColorConvertFloat4ToU32(ImColor(colors[ImGuiCol_TitleBgCollapsed])); // Current line fill inactive
        palette[(int)TextEditor::PaletteIndex::CurrentLineEdge] = ImGui::ColorConvertFloat4ToU32(ImColor(colors[ImGuiCol_TitleBgActive]));     // Current line edge

        editor.SetPalette(palette);
    }

    // �� ImGui ��ʽ��ɫӳ�䵽 TextEditor ��ɫ��
    void init_editor_theme(TextEditor& editor,std::vector<float>& var) {
        // ��ȡ��ǰ�� ImGui ��ʽ
        ImGuiStyle& style = ImGui::GetStyle();

        // ��ȡ TextEditor �ĵ�ɫ��
        auto palette = editor.GetPalette();

        // �� ImGui ��ʽ��ɫӳ�䵽 TextEditor �ĵ�ɫ��
        palette[(int)TextEditor::PaletteIndex::Default] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);
        //palette[(int)TextEditor::PaletteIndex::Keyword] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_PlotHistogramHovered]);
        //palette[(int)TextEditor::PaletteIndex::Number] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_TextSelectedBg]);
        //palette[(int)TextEditor::PaletteIndex::String] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_ButtonHovered]);
        //palette[(int)TextEditor::PaletteIndex::CharLiteral] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_FrameBgHovered]);

        //palette[(int)TextEditor::PaletteIndex::Punctuation] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Separator]);
        //palette[(int)TextEditor::PaletteIndex::Preprocessor] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_ResizeGripHovered]);
        //palette[(int)TextEditor::PaletteIndex::Identifier] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_PlotLines]);
        //palette[(int)TextEditor::PaletteIndex::KnownIdentifier] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_PlotHistogram]);
        //palette[(int)TextEditor::PaletteIndex::PreprocIdentifier] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_HeaderActive]);

        //palette[(int)TextEditor::PaletteIndex::Comment] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_TextDisabled]);
        //palette[(int)TextEditor::PaletteIndex::MultiLineComment] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_TextDisabled]);
        //palette[(int)TextEditor::PaletteIndex::Background] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_WindowBg]);
        
        ImVec4 color = style.Colors[ImGuiCol_WindowBg];
        //color.x *= 0.95f;
        //color.y *= 0.95f;
        //color.z *= 0.95f;
        color.x *= var[1];
        color.y *= var[1];
        color.z *= var[1];
        color.w *= var[0];

        palette[(int)TextEditor::PaletteIndex::Background] = ImGui::ColorConvertFloat4ToU32(color);

        palette[(int)TextEditor::PaletteIndex::Punctuation] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);

        //palette[(int)TextEditor::PaletteIndex::Cursor] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);
        //palette[(int)TextEditor::PaletteIndex::Selection] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_HeaderHovered]);
        //palette[(int)TextEditor::PaletteIndex::ErrorMarker] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_PlotLinesHovered]);

        //palette[(int)TextEditor::PaletteIndex::Breakpoint] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_CheckMark]);
        //palette[(int)TextEditor::PaletteIndex::LineNumber] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_TextDisabled]);
        //palette[(int)TextEditor::PaletteIndex::CurrentLineFill] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_TableRowBg]);
        //palette[(int)TextEditor::PaletteIndex::CurrentLineFillInactive] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_TableRowBgAlt]);
        //palette[(int)TextEditor::PaletteIndex::CurrentLineEdge] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_SeparatorHovered]);

        // ���޸ĺ��ĵ�ɫ��Ӧ�õ� TextEditor
        editor.SetPalette(palette);
    }


    //editor������ʼ������
    inline void init_editor(TextEditor& editor, EditorType type)
    {

        if (type == CODE) {
            editor.SetLanguageDefinition(lang_GLSL);
        }
        else if (type == DEBUG_INFO)
        {
            editor.SetLanguageDefinition(lang_CPP);
        }
        //init_editor_theme(editor);
        editor.SetPalette(TextEditor::GetDarkPalette());
    }
    

    inline struct init {
        init() {
            {
                init_editor_lang(lang_GLSL, GLSL);
                init_editor_lang(lang_CPP, CPP);

                init_editor(vs_editor, CODE);
                init_editor(fs_editor, CODE);
                init_editor(vs_debug_editor, DEBUG_INFO);
                init_editor(fs_debug_editor, DEBUG_INFO);

                vs_editor.SetText(g_default_vs_txt);
                fs_editor.SetText(g_default_fs_txt);
                vs_debug_editor.SetText("debug info");
                fs_debug_editor.SetText("debug info");

                vs_str.reserve(1024);
                fs_str.reserve(1024);
            }
        }
    }init_edit_;
    
    //�򿪼����ļ����ڲ���ȡ�ļ�·��������
    bool open_get_File(std::string& file_path, std::string& file_name)
    {
        OPENFILENAME ofn = { sizeof(OPENFILENAME) };
        char strFileName[MAX_PATH] = { 0 };
        ofn.hwndOwner = GetForegroundWindow();
        //ofn.lpstrFilter = "All Files\0*.*\0";
        //gray@2024.09.12 comment::��ʱ����ֻ�ܼ������ֺ�׺���ļ�
        ofn.lpstrFilter = "GLSL Files\0*.glsl\0Vertex Shader Files\0*.vs\0Fragment Shader Files\0*.fs\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFile = strFileName;
        ofn.nMaxFile = sizeof(strFileName);
        ofn.lpstrTitle = "Load File";
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

        if (GetOpenFileName(&ofn)) {
            file_path = strFileName;
            file_name = file_path.substr(file_path.find_last_of('\\') + 1);
            return 1;
        }

        return 0;
    }
    //��ȡ����1�ļ�תΪ�ַ��������ŵ�����2
    bool LoadFilie2String(const std::string& file_path, std::string& result_str)
    {
        std::ifstream if_sd;
        if_sd.open(file_path);
        if (if_sd.is_open())
        {
            result_str.clear();

            std::filebuf* pbuf = if_sd.rdbuf();
            size_t sz_code = pbuf->pubseekoff(0, if_sd.end, if_sd.in);
            pbuf->pubseekpos(0, if_sd.in);
            result_str.reserve(sz_code);
            result_str.resize(sz_code);
            pbuf->sgetn(&result_str[0], sz_code);
            if_sd.close();
            return 1;
        }
        return 0;
    }
    //�������������Ӵ���
    std::string check_shader_error(GLuint target, ShaderCheckType type)
    {
        int success = 0;
        constexpr int LOG_SIZE = 1024;
        char info_log[LOG_SIZE]{};
        if (type == COMPILE)
        {
            glGetShaderiv(target, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(target, LOG_SIZE, NULL, info_log);
                return std::string(info_log);
            }
        }
        else if (type == LINK)
        {
            glGetProgramiv(target, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(target, 1024, NULL, info_log);
                return std::string(info_log);
            }
        }
        return "no error";
    }
    //��ȡ�༭������
    std::string get_editor_text(TextEditor& editor)
    {
        std::string result_str = editor.GetText();
        /*for (int i = result_str.size() - 1; i >= 0; i--)
        {
            if (result_str[i] == '\n' || result_str[i] == '\0' || result_str[i] == '\r')
                result_str.pop_back();
            else break;
        }
        result_str.push_back('\0');*/
        return result_str;
    }
    
    /*
    {
        //���涥����ɫ������
        void save_vs_source(vg::shader::shader_base& shader,const std::string& str) {
            //����д��bug
            //const char* cur_vs_code = vs_editor.GetText().c_str();

            const char* cur_vs_code = str.c_str();
            if (shader.get_vertex_shader() == 0) {
                shader.set_vertex_shader(glCreateShader(GL_VERTEX_SHADER));
            }
            glShaderSource(shader.get_vertex_shader(), 1, &cur_vs_code, 0);
        }
        //����Ƭ����ɫ������
        void save_fs_source(vg::shader::shader_base& shader,const std::string& str)
        {
            const char* cur_fs_code = str.c_str();
            if (shader.get_fragment_shader() == 0) {
                shader.set_fragment_shader(glCreateShader(GL_FRAGMENT_SHADER));
            }
            glShaderSource(shader.get_fragment_shader(), 1, &cur_fs_code, 0);
        }

        //��ȡ������ɫ������
        std::string get_vs_source(vg::shader::shader_base& shader)
        {
            GLint source_length = 0;
            std::string str;
            glGetShaderiv(shader.get_vertex_shader(), GL_SHADER_SOURCE_LENGTH, &source_length);
            if (source_length > 0) {
                str.resize(source_length);
                glGetShaderSource(shader.get_vertex_shader(), source_length, nullptr, &str[0]);
            }
            return str;
        }
        //��ȡƬ����ɫ������
        std::string get_fs_source(vg::shader::shader_base& shader)
        {
            GLint source_length = 0;
            std::string str;
            glGetShaderiv(shader.get_fragment_shader(), GL_SHADER_SOURCE_LENGTH, &source_length);
            if (source_length > 0) {
                str.resize(source_length);
                glGetShaderSource(shader.get_fragment_shader(), source_length, nullptr, &str[0]);
            }
            return str;
        }
    }
    */

    void hightlight_editor(TextEditor& editor, const std::string& debug_info)
    {
        //�����ϴεĴ�������
        editor.SetErrorMarkers({});

        std::regex errorPattern(R"(ERROR: \d+:(\d+):.*?:\s*(.*))");
        std::smatch match;
        std::map<int, std::string> errmsg_mp;
        auto begin = std::sregex_iterator(debug_info.begin(), debug_info.end(), errorPattern);
        auto end = std::sregex_iterator();

        if (begin != end) 
        {
            for (std::sregex_iterator i = begin; i != end; ++i) {
                std::smatch match = *i;
                std::string lineNumber = match[1];  // Captures the line number
                std::string errorInfo = match[2];   // Captures the error message

                errmsg_mp[atoi(lineNumber.c_str())] += errorInfo+'\n';
            }
            editor.SetErrorMarkers(errmsg_mp);
        }
    }
    void vertical_splitter(float& code_height, float& compile_info_height, const float& total_height)
    {
        const float splitter_thickness = 5.0f;  // 分割条的厚度
        const float min_height = 40.0f;         // 每个区域的最小高度

        // 设置分割条的光标样式为上下调整
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::Button("##splitter", ImVec2(-1, splitter_thickness));
        ImGui::PopStyleVar();

        // 分割条被激活时处理鼠标拖动逻辑
        if (ImGui::IsItemActive()) {
            float mouse_delta = ImGui::GetIO().MouseDelta.y;

            // 更新两个区域的高度
            code_height += mouse_delta;
            compile_info_height -= mouse_delta;

            // 边界检查：确保高度不会小于最小值或超过总高度
            if (code_height < min_height) {
                code_height = min_height;
                compile_info_height = total_height - code_height - splitter_thickness;
            }
            if (compile_info_height < min_height) {
                compile_info_height = min_height;
                code_height = total_height - compile_info_height - splitter_thickness;
            }

            // 严格防止超出总高度
            float current_total_height = code_height + compile_info_height + splitter_thickness;
            if (current_total_height > total_height) {
                float excess = current_total_height - total_height;
                // 按比例调整两个区域的高度
                float code_ratio = code_height / (code_height + compile_info_height);
                code_height -= excess * code_ratio;
                compile_info_height -= excess * (1.0f - code_ratio);
            }

            // 确保调整后仍然满足最小高度要求
            if (code_height < min_height) {
                compile_info_height -= (min_height - code_height);
                code_height = min_height;
            }
            if (compile_info_height < min_height) {
                code_height -= (min_height - compile_info_height);
                compile_info_height = min_height;
            }
        }

        // 如果鼠标悬停在分割条上，改变光标样式
        if (ImGui::IsItemHovered()) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
        }
    }
    void shader_load(ShaderTargetType target_type, std::string& path, std::string& name)
    {
        std::string str{};
        if (open_get_File(path, name))
        {
            bool ret = LoadFilie2String(path, str);
            if (ret)
            {
                if (target_type == ShaderTargetType::VERTEX_SHADER)
                    vs_editor.SetText(str);
                else if (target_type == ShaderTargetType::FRAGMENT_SHADER)
                    fs_editor.SetText(str);
            }
        }
    }
    void shader_save(ShaderTargetType target_type, vg::shader::shader_base& shader)
    {
        if (target_type == ShaderTargetType::VERTEX_SHADER)
            shader.set_source(ShaderTargetType::VERTEX_SHADER, get_editor_text(vs_editor));
        else if (target_type == ShaderTargetType::FRAGMENT_SHADER)
            shader.set_source(ShaderTargetType::FRAGMENT_SHADER, get_editor_text(fs_editor));
    }
    void shader_compile(ShaderTargetType target_type, vg::shader::shader_base& shader)
    {
        std::string error_info{ "no error" };
        if (target_type == ShaderTargetType::VERTEX_SHADER)
        {
            shader.compile(ShaderTargetType::VERTEX_SHADER, error_info);
            vs_debug_editor.SetText(error_info.c_str());
            hightlight_editor(vs_editor, error_info);
        }
        else if (target_type == ShaderTargetType::FRAGMENT_SHADER)
        {
            shader.compile(ShaderTargetType::FRAGMENT_SHADER, error_info);
            fs_debug_editor.SetText(error_info.c_str());
            hightlight_editor(fs_editor, error_info);
        }

    }
    void shader_link(vg::shader::shader_base& shader)
    {
        std::string error_info{ "no error" };
        shader.link(error_info);
        vs_debug_editor.SetText(error_info.c_str());
        hightlight_editor(vs_editor, error_info);
        fs_debug_editor.SetText(error_info.c_str());
        hightlight_editor(fs_editor, error_info);

    }

}
