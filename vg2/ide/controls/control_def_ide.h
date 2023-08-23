#include "control_def.h"

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
class AFG_EXPORT control_def_ide : public control_def {

protected:
  const float edit_unit_len = 5.0f;
public:
  void ex_init_fun();
  virtual void draw_outline();
  virtual void draw_sel_anchor();
  void draw_propertys();
  void draw_corners();

  static int hit_key;
};
} // namespace vg