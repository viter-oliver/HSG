#pragma once
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "res_output.h"
#include "cp_primitive_object.h"
using namespace std;
struct bounding_box
{
	float _xmin{ 0.f }, _xmax{ 0.f };
	float _ymin{ 0.f }, _ymax{ 0.f };
	float _zmin{ 0.f }, _zmax{ 0.f };
};
struct cp_mesh
{
	vector<string> _text_diffuse_list;
	vector<string> _text_specular_list;
	vector<string> _text_height_list;
	vector<string> _text_ambient_list;
	string _prm_id;
	vector<ps_cp_texture> _ps_text_diffuse_list;
	vector<ps_cp_texture> _ps_text_specular_list;
	vector<ps_cp_texture> _ps_text_height_list;
	vector<ps_cp_texture> _ps_text_ambient_list;
	ps_primrive_object _ps_prm_id;
	bounding_box _box;
};
extern const char* assimp_support_format;
using cp_model = vector<cp_mesh>;
using cp_mesh_list = cp_model;
using mp_model=map<string, shared_ptr<cp_model>>;
extern mp_model g_mmodel_list;
//extern const char* model_group_name;
