#pragma once
#include "vg_type.h"
#include <map>
#include <memory>
#include <string>
/**
 * @brief definition of filechunk
 * 
 * file chunk is used to store some structural data referenced by objects of controls,for example sequence of frames of animation
 * 
 */
namespace vg {
struct file_chunk_unit {
  void *_pbin = {nullptr};
  u32 _fsize = {0};
  file_chunk_unit(u32 fsize);
  void re_alloc(u32 fsize);
  ~file_chunk_unit();
};
using sd_file_chunk_unit = std::shared_ptr<file_chunk_unit>;
} // namespace vg