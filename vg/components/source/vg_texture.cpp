#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include "glad.h"
#endif
#include "vg_texture.h"
namespace vg{
    texture::~texture(){
        if(_txt_id>0){
            glDeleteTextures(1, &_txt_id);
        }
    }
}