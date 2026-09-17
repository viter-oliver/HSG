#pragma once
#include "computable_ele.h"
#include "control_def.h"
namespace vg{
     struct control_computable_ele :computable_ele
     {
          sd_control_def _host;
          int member_id;
          sd_bind_var get_sd_bind_var()
          {
               return _host->get_member( member_id );
          }
     };
}