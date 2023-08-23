#include "texture_ex.h"
#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include "glad.h"
#endif
namespace vg {
texture_ex::~texture_ex() {
  if (txt_id > 0) {
    glDeleteTextures(1, &txt_id);
  }
}
m_sd_texture_ex g_texture_dic;
} // namespace vg