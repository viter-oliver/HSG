#pragma once
#include "vg_type.h"
namespace vg { 
  struct uniform_interface
  {
    virtual bool set_location(float* pvalue) {

      return pass_value(pvalue);
    }
    virtual bool set_location(int* pvalue) {
      return pass_value(pvalue);
    }
    virtual bool set_location(double* pvalue) {
      return pass_value(pvalue);
    }
    virtual bool set_location(u32* pvalue) {
      return pass_value(pvalue);
    }
    virtual bool set_location(int ivalue) {
      return pass_value(&ivalue);
    }
    virtual bool pass_value(void* pvalue) {
		return false;
    };
    virtual void value_info(int& type_value,int& count){
      type_value = 0;
      count = 1;
    }
  };
  using sd_uniform_interface= std::shared_ptr<uniform_interface>;
  
}

