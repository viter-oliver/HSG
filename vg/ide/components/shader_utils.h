#pragma once
#include <windows.h>
#include <locale.h>  
#include <ShlObj.h>
#include <Commdlg.h>
//#include "dir_output.h"
#include"fstream"
#include<unordered_map>
#include"shader.h"
#include"TextEditor.h"
#include<iostream>
#include<imgui.h>
namespace vg {

	
	extern const char* g_default_vs_txt;
	extern const char* g_default_fs_txt;

	extern TextEditor vs_editor;
	extern TextEditor fs_editor;

	extern TextEditor vs_debug_editor;
	extern TextEditor fs_debug_editor;

	extern std::string vs_str;
	extern std::string fs_str;


	enum EditorType
	{
		CODE,		//代码
		DEBUG_INFO	//调试信息
	};
	
	enum EditorLangType
	{
		GLSL,		
		CPP
	};


    //语言高亮初始化设置
    inline void init_lang(TextEditor::LanguageDefinition& lang, EditorLangType type);
    //editor高亮初始化设置
    inline void init_editor(TextEditor& editor, EditorType type);
	//TextEditor跟随主题设置
	 void init_editor_theme(TextEditor& editor, std::vector<float>&);

    //打开加载文件窗口并获取文件路径和名字
	bool open_get_File(std::string& file_path, std::string& file_name);

    //读取参数1文件转为字符串流存放到参数2
	bool LoadFilie2String(const std::string& file_path, std::string& result_str);

    //检查编译或链接错误
	std::string check_shader_error(GLuint target, ShaderCheckType type);

	//保存代码到shader中
	/*
	void save_vs_source(vg::shader::shader_base& shader, const std::string& str);
	void save_fs_source(vg::shader::shader_base& shader, const std::string& str);
	std::string get_vs_source(vg::shader::shader_base& shader);
	std::string get_fs_source(vg::shader::shader_base& shader);
	*/
	
	std::string get_editor_text(TextEditor& editor);
    
	//高亮调试信息
	void hightlight_editor(TextEditor& editor,const std::string& debug_info);
	//自定义垂直分割线
	void vertical_splitter(float& code_height, float& compile_info_height,const float& total_height);
	//shader编辑加载文件内容到内存
	void shader_load(ShaderTargetType target_type, std::string& path, std::string& name);
	//保存内存中code到shader
	void shader_save(ShaderTargetType target_type, vg::shader::shader_base& shader);
	//编译shader中已保存的code
	void shader_compile(ShaderTargetType target_type, vg::shader::shader_base& shader);
	//链接shader中已编译的code
	void shader_link(vg::shader::shader_base& shader);

}