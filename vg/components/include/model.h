#pragma once
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "packing_texture_container.h"
#include "primitive_container.h"
namespace vg{
namespace model{
struct bounding_box
{
	float _xmin{ 0.f }, _xmax{ 0.f };
	float _ymin{ 0.f }, _ymax{ 0.f };
	float _zmin{ 0.f }, _zmax{ 0.f };
};
struct mesh
{
	std::vector<std::string> _text_diffuse_list;
	std::vector<std::string> _text_specular_list;
	std::vector<std::string> _text_height_list;
	std::vector<std::string> _text_ambient_list;
	std::string _prm_id;
	std::vector<sd_packing_texturedef> _ps_text_diffuse_list;
	std::vector<sd_packing_texturedef> _ps_text_specular_list;
	std::vector<sd_packing_texturedef> _ps_text_height_list;
	std::vector<sd_packing_texturedef> _ps_text_ambient_list;
	sd_primitivbe_base _ps_prm_id;
	bounding_box _box;
};
extern const char* assimp_support_format;
using mesh_list = std::vector<mesh>;
using sd_mesh_list = std::shared_ptr<mesh_list>;
using mp_sd_mesh_list=std::map<std::string, sd_mesh_list>;
extern mp_sd_mesh_list g_mmodel_list;	
}
}

//extern const char* model_group_name;
