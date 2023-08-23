#ifdef WITHIN_IDE
#include "file_chunk_ide.h"
using file_chunk_def=vg::file_chunk_ide;
#else
#include "file_chunk.h"
using file_chunk_def=vg::file_chunk_unit;
#endif
namespace vg{
  using sd_file_chunk_def = std::shared_ptr<file_chunk_def>;
  using mp_file_chunk = std::map<std::string, sd_file_chunk_def>;
  extern mp_file_chunk _mp_file_chunk;
}