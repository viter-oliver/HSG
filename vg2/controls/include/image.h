#pragma once
#include "control_fac.h"
namespace vg {
class AFG_EXPORT image : public control_base {

  DEF_STRUCT_WITH_INIT(_img_pt, 
  (vec2, _rotate_center),
  (vec2, _offset_scale_srd),
  (vui2,  _texture_index_txt),
  (vec4, _tint_clr),
  (vec4, _border_clr),
  (float,_angle_srd, {0.f}))
public:
  image(); // : ctl_base(){}
  void set_texture_id(vi2 texture_id) {
    _img_pt._texture_index_txt = texture_id;
  }
  vec2 start_point(){
    auto offset=size()*_img_pt._offset_scale_srd;
    auto ab_base_pos=absolute_coordinate_of_base_pos();
    auto st_pt=ab_base_pos-offset;
    return st_pt;
  }
  vui2 get_texture_id() { return _img_pt._texture_index_txt; }
  void rotate(float angle) { 
    _img_pt._angle_srd = angle; 
  }
  bool contain(vec2 &tar_pos){
    auto rotated=abs(_img_pt._angle_srd)>=0.1;
    if(!rotated){
      auto left_top=start_point();
      auto right_bottom=left_top+size();
      area_f area_={left_top,right_bottom};
      return area_.contain(tar_pos);
    } else {
      vec2 p1=start_point();
      vec2 p2={p1.x+size().x,p1.y};
      vec2 p3=p1+size();
      vec2 p4={p1.x,p1.y+size().y};
      auto& center=_img_pt._rotate_center;
      auto angle=_img_pt._angle_srd;
      auto pr1=p1.rotate_by(center,angle);
      auto pr2=p2.rotate_by(center,angle);
      auto pr3=p3.rotate_by(center,angle);
      auto pr4=p4.rotate_by(center,angle);

      auto tar_pr1=tar_pos-pr1;
      auto tar_pr2=tar_pos-pr2;
      auto tar_pr3=tar_pos-pr3;
      auto tar_pr4=tar_pos-pr4;  
      auto same_s=[](float nm1,float nm2){
        auto s=nm1*nm2;
        return s>=0;
      };
      auto tp1_x_tp2=tar_pr1.x_ply(tar_pr2);
      auto tp2_x_tp3=tar_pr2.x_ply(tar_pr3);
      if(!same_s(tp1_x_tp2,tp2_x_tp3)){return false;};
      auto tp3_x_tp4=tar_pr3.x_ply(tar_pr4);
      if(!same_s(tp2_x_tp3,tp3_x_tp4)){return false;};
      auto tp4_x_tp1=tar_pr4.x_ply(tar_pr1);
      if(!same_s(tp4_x_tp1,tp3_x_tp4)){return false;};
      return true;
    }
    
  }
  void draw();
};

REG_CTL_2_FAC(image);
} // namespace vg