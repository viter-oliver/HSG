#include "macro_tool.h"
#ifdef WITHIN_IDE
#include "texture_ide.h"
using texture_def=vg::texture_ide;
#else
#include "texture_ex.h"
using texture_def=vg::texture_ex;
#endif
namespace vg{
  DEFINE_SMART(texture_def)
  inline m_sd_texture_def _m_sd_texture_def;
}