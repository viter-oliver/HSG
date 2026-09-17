#pragma once
#include "components_edit.h"
#include "ifile_open.hpp"
namespace vg{
  class pipepline_edit:public base_component_edit
  {
    public:
      pipepline_edit(std::string& name);
      ~pipepline_edit();
      void show();
      void clear();
  };
  REG_SIMPLE_COPONENT_EDIT_CONTAINER(pipepline_edit)
  struct open_models:public IFileOpener
  {
      bool tryOpen(const std::string& path);
  };
} 
