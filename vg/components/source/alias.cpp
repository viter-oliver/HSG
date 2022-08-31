#include "alias.h"
namespace vg{
  
aliase_map g_aliase_dic;
using namespace std;
bool set_property_aliase_value(std::string prp_aliase_name, void* pvalue,u32 value_sz)
{
  const auto& ialiase = g_aliase_dic.find(prp_aliase_name);
  if (ialiase==g_aliase_dic.end())
  {
    printf("unknown alias name:%s\n", prp_aliase_name.c_str());
    return false;
  }
  auto& prop_pos =*ialiase->second;
  return prop_pos._pobj->set_prop_fd_value(prop_pos._page_index, prop_pos._field_index, pvalue, value_sz);
}
bool set_property_txt_aliase_value(std::string prp_aliase_name, int sub_id, int group_id )
{
     vui2 txt_value = { group_id, sub_id };
     return set_property_aliase_value( prp_aliase_name, (void*)&txt_value,sizeof vui2);
}
prop_ele_value_dic g_lazy_value_buff;
bool set_property_aliase_lazy_value(std::string prp_aliase_name, int during, void* pvalue )
{
    const auto& ialiase = g_aliase_dic.find( prp_aliase_name );
     if( ialiase == g_aliase_dic.end() )
     {
          printf( "unknown alias name:%s\n", prp_aliase_name.c_str() );
          return false;
     }
    const auto& ilazy = g_lazy_value_buff.find( prp_aliase_name );
    if( ilazy != g_lazy_value_buff.end() )
    {
         printf( "alias:%s is already lazying state\n" );
         return false;
    }
    auto& prop_pos = *ialiase->second;
    auto& pgidx = prop_pos._page_index;
    auto& fdidx = prop_pos._field_index;
    auto& field = prop_pos._pobj->get_filed_ele( pgidx, fdidx );

    g_lazy_value_buff[ prp_aliase_name ]._during = during;
    g_lazy_value_buff[ prp_aliase_name ]._value.resize(field._tpsz);
    g_lazy_value_buff[ prp_aliase_name ]._start = steady_clock::now();
    return true;
}

void execute_lazy_value()
{
     auto currentTime = steady_clock::now();
     for( auto ilazy = g_lazy_value_buff.begin(); ilazy != g_lazy_value_buff.end(); )
     {
          auto& lzvalue=ilazy->second;
          int delta = chrono::duration_cast<chrono::duration<int, std::milli>>( currentTime - lzvalue._start ).count();
          if (delta>lzvalue._during)
          {
               auto& alias_name = ilazy->first;
               set_property_aliase_value( alias_name, &lzvalue._value[ 0 ] );
               ilazy = g_lazy_value_buff.erase( ilazy );
          }
          else
          {
               ilazy++;
          }
     }
}
sd_control_base get_aliase_ui_control(std::string prp_aliase_name)
{
  const auto& ialiase = g_aliase_dic.find(prp_aliase_name);
  if (ialiase == g_aliase_dic.end())
  {
    printf("unknown alias name:%s\n", prp_aliase_name.c_str());
    return nullptr;
  }
  auto& prop_pos = *ialiase->second;
  return prop_pos._pobj;
}
}