#pragma once
#include <string>
#include "macro_tool.h"
#include "uniform_interface.h"
namespace vg{
  struct bind_var;
  DEFINE_SMART(bind_var)
  struct var_base {
    virtual void get_memory_info(void*& address,int& len );
    virtual sd_bind_var get_bind(const std::string& name) = 0;
  };
  using sd_var_base = std::shared_ptr<var_base>;
  struct IVarEditor {
      virtual ~IVarEditor() = default;
      virtual void edit_var(const std::string& name, void* value, int type_id,int type_count) = 0;
  };
  struct bind_var:public uniform_interface{
    // FIXME?
    bool _calculated{false};

    //virtual void edit(){};
    //virtual sd_var_base spawn() = 0;
    virtual void edit(IVarEditor* editor) = 0;
    virtual void edit()=0;
    virtual sd_bind_var clone() = 0;
    virtual std::string var_name()=0;
    virtual void get_void_ptr(void*& pvalue) = 0;
    virtual bool assign(sd_var_base& sbase) = 0;
    virtual bool scale(sd_var_base from, sd_var_base to, double s) {
      return false;
    };
  };
  struct get_sd_bind_var_member{
    virtual sd_bind_var get_member(int id) = 0;
  };
}