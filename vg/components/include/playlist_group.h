#pragma once
#include "platform_def.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
namespace vg{
struct playlist_unit
{
	std::string _st_name;
	int _playlist_id={0};
	playlist_unit(std::string& trans_name, int pl_id) :_st_name(trans_name), _playlist_id(pl_id){}
};
using playlist_unit_list = std::vector<playlist_unit>;
using sp_playlist_group_unit = std::shared_ptr<playlist_unit_list>;
using playlist_group_list = std::map<std::string, sp_playlist_group_unit>;

extern playlist_group_list g_playlist_group_list;
extern bool AFG_EXPORT play_playlist_group(std::string gp_name);
}