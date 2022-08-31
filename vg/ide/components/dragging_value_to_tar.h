#pragma once
#include <functional>
#include <memory>
namespace vg{
  namespace dragging_value_to_tar{
    struct dragging_value{
        virtual void dumy(){}
    };
    using ptr_dragging_value=std::shared_ptr<dragging_value>;
    using accept_fun=std::function<void(ptr_dragging_value,int)>;
    void reg_a_value(ptr_dragging_value pvalue);
    void try_dragging();
    int reg_accept_fun(accept_fun acc_fun);
    void report_hit(int key,int subkey);
  }
}