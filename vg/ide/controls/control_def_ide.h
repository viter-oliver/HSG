#include "control_def.h"
#include "dragging_value_to_tar.h"
#include <map>
#include <nlohmann/json.hpp>
namespace vg {
#undef DECLARE_EX_INT
#define DECLARE_EX_INT void ex_init_fun();

#undef DECLARE_PROVIDE_DRAGGING_VALUE
#define DECLARE_PROVIDE_DRAGGING_VALUE                                         \
  virtual void provide_value(                                                  \
      dragging_value_to_tar::ptr_dragging_value pdragging_value);
#undef DECLARE_DRAW_OUTLINE
#define DECLARE_DRAW_OUTLINE void draw_outline();

#undef DECLARE_DRAW_SEL_ANCHOR
#define DECLARE_DRAW_SEL_ANCHOR virtual void draw_sel_anchor();

bool edit_file_ele(field_ele &fele);
class AFG_EXPORT control_def_ide : public control_def {

protected:
  const float edit_unit_len = 5.0f;
  struct st_member_key {
    void *_address;
    void *_field_address;
    st_member_key(void *address, void *field_address)
        : _address(address), _field_address(field_address) {}
    bool operator<(const st_member_key &skey) const {
      if (_address != skey._address) {
        return _address < skey._address;
      } else {
        return _field_address < skey._field_address;
      }
    }
  };
  using property_handle = std::function<void(void)>;
  using mp_property_handle = std::map<st_member_key, property_handle>;
  mp_property_handle _unique_property_handles;
  void reg_property_handle(void *page_address, void *field_address,
                           property_handle phandle) {
    _unique_property_handles[st_member_key(page_address, field_address)] =
        phandle;
  }

public:
  DECLARE_EX_INT
  DECLARE_PROVIDE_DRAGGING_VALUE
  DECLARE_DRAW_OUTLINE
  DECLARE_DRAW_SEL_ANCHOR
  bool _selected{false};
  void draw_propertys();
  void draw_corners();
  void init_property_from_json(nlohmann::json &jvalue);
  void save_property_to_json(nlohmann::json &junit);
  static int hit_key;
};
} // namespace vg