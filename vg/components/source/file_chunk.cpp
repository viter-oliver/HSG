#include "file_chunk.h"
#include <stdlib.h>
namespace vg {
file_chunk_unit::file_chunk_unit(u32 fsize) : _fsize(fsize) {
  _pbin = malloc(fsize);
}
void file_chunk_unit::re_alloc(u32 fsize) {
  auto rt = realloc(_pbin, fsize);
  if (rt) {
    _fsize = fsize;
  }
}
file_chunk_unit::~file_chunk_unit() { free(_pbin); }
} // namespace vg
