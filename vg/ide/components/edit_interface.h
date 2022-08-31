#include <string>
namespace vg{
  struct edit_interface{
    virtual void select_item(int index){}
    virtual void select_item(std::string& key ){}
    virtual void edit_item()=0;
  };
}