#pragma once
#include "ctl_base.h"
namespace vg
{
     class ctl_paragraph :
          public ctl_base
     {
     public:
          ctl_paragraph(wstring& wsparagraph);
          ~ctl_paragraph();
     };
}
