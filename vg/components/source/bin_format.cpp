#include "bin_format.h"
#include "SOIL.h"
#include <GL/gl3w.h>
extern "C" {
#include "image_DXT.h"
}
namespace vg {
namespace bin_format {
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif
output_bin_format _bin_format;
void get_texture_buff(u32 txt_id, u32 width, u32 height, texture_buff txtBuff) {
  int txt_size = width * height * 4;
  txtBuff.resize(txt_size);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &txtBuff[0]);
  if (_bin_format._txt_fmt == en_uncompressed_txt) {
    return;
  } else if (_bin_format._txt_fmt = en_dxt5) {
    int out_sz = 0;
    u8 *pDDS_data = convert_image_to_DXT5(&txtBuff[0], width, height,
                                          SOIL_LOAD_RGBA, &out_sz);
    if (out_sz > 0) {
      txtBuff.resize(out_sz);
      memcpy_s(&txtBuff[0], out_sz, pDDS_data, out_sz);
      free(pDDS_data);
    }
  }
}
u32 get_texture_id(u8* txtBuff,u32 buff_size,u32 width,u32 height){
  u32 txt_id;
  glGenTextures(1,&txt_id);
  glBindTexture(GL_TEXTURE_2D,txt_id);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if(_bin_format._txt_fmt == en_uncompressed_txt){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
										 0, GL_RGBA, GL_UNSIGNED_BYTE, txtBuff);
  } else if(_bin_format._txt_fmt = en_dxt5){
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, width, height, 0, buff_size, txtBuff);
  }
  glBindTexture(GL_TEXTURE_2D,0);
  return txt_id;
}
int out_texture_format() { return _bin_format._txt_fmt; }
} // namespace bin_format
} // namespace vg