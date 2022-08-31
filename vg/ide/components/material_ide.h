#include "material.h"
#include "shader_ide.h"
#include "edit_interface.h"
namespace vg {
namespace shader {
	class material_ide;
	using sd_material_ide=std::shared_ptr<material_ide>;
	class material_ide : public material,edit_interface{
	protected:
		std::string _shader_name;
	public:
	  bool _sel{false};
	  void edit_item();
		//sd_material_ide create_item();
	};
} // namespace shader
} // namespace vg