#include "packing_texture.h"
#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include "glad.h"
#endif
namespace vg{
  namespace packing_texture{
    packing_texture_unit::~packing_texture_unit(){
      if(txt_id>0){
        glDeleteTextures(1,&txt_id);
      }
    }
    v_sd_packing_texture _v_sd_packing_texture;
  }
}