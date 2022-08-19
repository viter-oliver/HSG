#include "vg_texture.h"
#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include "glad.h"
#endif
namespace vg {
namespace texture {
texture_unit::~texture_unit(){
  if (txt_id>0) {
    glDeleteTextures(1,&txt_id);
  }
 }
 mp_texture _gl_textures;
} // namespace gl_texture
} // namespace vg