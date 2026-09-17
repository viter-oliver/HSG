#include "control_def.h"

namespace vg {
#undef DECLARE_EX_INT
#define DECLARE_EX_INT void ex_init_fun();


#undef DECLARE_DRAW_OUTLINE
#define DECLARE_DRAW_OUTLINE void draw_outline();

#undef DECLARE_DRAW_SEL_ANCHOR
#define DECLARE_DRAW_SEL_ANCHOR void draw_sel_anchor();

#undef DECLARE_DRAW_PROPERTY
#define DECLARE_DRAW_PROPERTY void draw_properties();

class AFG_EXPORT control_def_ide : public control_def {

protected:
  using user_edit=std::function<void()>;
  std::map<int, user_edit> _user_properties_handlor;
  
  const float edit_unit_len = 5.0f;
public:
  virtual void ex_init_fun();
  virtual void draw_outline();
  virtual void draw_sel_anchor();
  virtual void draw_properties();
};
} // namespace vg