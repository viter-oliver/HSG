#pragma once
#include <nlohmann/json.hpp>
#include "res_internal.h"
using namespace Json;
class texture_res_load
{
	v_sd_packing_texture& _texture_res_tar;
public:
	texture_res_load(v_sd_packing_texture&texture_res_tar) :_texture_res_tar(texture_res_tar){}

	void load_res_from_json(Value& jroot);
};
bool load_texture_info(packing_texture& rtlist, string& str_txt_pack_file, string& str_txt_data_file);

void load_internal_texture_res(mtxt_internal& mptxt, unsigned int res_font_id,unsigned int txtresid, unsigned int txtformatid);