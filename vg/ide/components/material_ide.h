#include "material.h"
#include "shader_ide.h"
#include "edit_interface.h"
namespace vg {
namespace shader {
	class material_ide : public material,edit_interface{
	public:
	  std::string _shader_name;
	  bool _sel{false};
	  void edit_item();
		//sd_material_ide create_item();
	};
	using sd_material_ide=std::shared_ptr<material_ide>;
} // namespace shader

using mp_sd_material_ide=std::map<std::string,shader::sd_material_ide>;
void create_item(mp_sd_material_ide& material_list);
} // namespace vg