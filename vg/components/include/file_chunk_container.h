#ifdef WITHIN_IDE
#include "file_chunk_ide.h"
using file_chunk_def=vg::sd_file_chunk_ide;
#else
#include "file_chunk.h"
using file_chunk_def=vg::sd_file_chunk_unit;
#endif
namespace vg{
  using mp_file_chunk = std::map<std::string, file_chunk_def>;
  extern mp_file_chunk _mp_file_chunk;
}