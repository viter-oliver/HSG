#pragma once
#include <vector>
#include <functional>
#include <string>
#include "platform_def.h "
#include "vg_type.h"
#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include"glad/glad.h"
#endif

extern std::string wstringToUtf8(const std::wstring& str);
extern bool get_file_data(const std::string& file_name, std::function<void(char*, unsigned int)>buff_handle);
extern bool get_file_data(const std::string& file_name, std::function<char*(unsigned int)>get_buff);
extern std::wstring utf8ToWstring(const std::string& str);
extern bool prepareFBO1(GLuint& colorTextId, GLuint& depthStencilTextId, GLuint& fboId, GLuint frame_width, GLuint frame_height);
extern bool cp_prepareFBO1(GLuint& colorTextId, GLuint& depthStencilTextId, GLuint& fboId, GLuint frame_width, GLuint frame_height);
extern bool prepareFBO2(GLuint& textId, GLuint& fboId, GLuint frame_width, GLuint frame_height);
extern bool fileExist(const char* fileName);
extern bool directoryExist(const char* dir);
extern bool createDirectory(const char* pathName);
extern void getUniqueFileName(std::string &file_full_name);
extern void saveStringTofile(std::string& file_full_name,std::string& strSrc);
extern std::string stoLower( std::string str );
extern std::string stoUpper( std::string str );
extern std::wstring wtoLower( std::wstring str );
extern std::wstring wtoUpper( std::wstring str );
extern bool createFileWithDirectory(const char* pathName);
extern void align_expression(std::string& exp, std::string&expo);
extern void trim_align_expression(std::string& exp, std::string&expo);
extern void convert_binary_to_string(char* pbin, int len, std::string& out_str);
extern void convert_string_to_binary(std::string& in_str, std::string& out_bin);
extern vg::u32 conver_track_buff_to_pair(char* pbuff,vg::u32 buff_len, std::vector<vg::vec2>& vtrack0, std::vector<vg::vec2>& vtrack1);
extern bool smooth_algorithm_5_points_3_times(std::vector<vg::vec2> &point_list,
                                       bool x_direction);
extern bool smooth_algoritm_3_points_average(std::vector<vg::vec2> &point_list,
                                      bool x_direction);	
extern bool smooth_algorithm_5_points_average(std::vector<vg::vec2> &point_list,
                                       bool x_direction);
extern void FromYUY2ToBGR32( LPVOID lpDest, LPVOID lpSource, LONG lWidth, LONG lHeight );
extern bool RayCastingAlgorithmPointWithin(std::vector<vg::vec2>& polygon,vg::vec2& tar);
template<class T> std::string find_a_key_from_mp(T& mp, std::string& ref_key)
{
	std::string chk_key = ref_key;
	while (mp.find(chk_key) != mp.end())
	{
		chk_key += "#";

	}
	return chk_key;
}
