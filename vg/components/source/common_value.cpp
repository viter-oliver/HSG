#include "common_value.h"
namespace vg {
using namespace std;
mp_sd_base_common_type g_base_prp_dic;
base_common_type::base_common_type( string bty )
{
     _type = bty;
     if( _type == "int" )
     {
          _pbase = new int;
          _size = sizeof( int );
     }
     else if( _type == "float" )
     {
          _pbase = new float;
          _size = sizeof( float );
     }
     else if( _type == "double" )
     {
          _pbase = new double;
          _size = sizeof( double );
     }
     else if( _type == "bool" )
     {
          _pbase = new bool;
          _size = sizeof( bool );
     }
     else if( _type == "vi2" )
     {
          _pbase = new vi2;
          _size = sizeof( vi2 );
     }
     else if( _type == "vi3" )
     {
          _pbase = new vi3;
          _size = sizeof( vi3 );
     }
     else if( _type == "vi4" )
     {
          _pbase = new vi4;
          _size = sizeof( vi4 );
     }
     else if( _type == "vec2" )
     {
          _pbase = new vec2;
          _size = sizeof( vec2 );
     }
     else if( _type == "vec3" )
     {
          _pbase = new vec3;
          _size = sizeof( vec3 );
     }
     else if( _type == "vec4" )
     {
          _pbase = new vec4;
          _size = sizeof( vec4 );
     }
     memset( _pbase, 0, _size );
}

void base_common_type::override_param_list()
{
     for (auto& itm_pm:_param_list)
     {
          auto& pgidx = itm_pm._page_index;
          auto& fdidx = itm_pm._field_index;
          auto& pobj = itm_pm._pobj;
          pobj->set_prop_fd_value( pgidx, fdidx, _pbase);

     }
}
bool prp_is_catched_by_base_prp_type(prop_ele_position &prp_pos,
                                     base_common_type &base_prp) {
  const auto &plist = base_prp._param_list;
  for (auto &iprp : plist) {
    if (iprp == prp_pos) {
      return true;
    }
  }
  return false;
}
bool cover_common_value(string name_of_common_value) {
  const auto &icmv = g_base_prp_dic.find(name_of_common_value);
  if (icmv != g_base_prp_dic.end()) {
    auto &btp = *icmv->second;
    btp.override_param_list();
    return true;
  }
  return false;
}
} // namespace vg