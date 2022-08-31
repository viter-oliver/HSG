#include "file_chunk.h"
#include "edit_interface.h"
namespace vg{
  struct file_chunk_ide:public file_chunk_unit,edit_interface{
    void edit_item();
  };
  using sd_file_chunk_ide=std::shared_ptr<file_chunk_ide>;
}