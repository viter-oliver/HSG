#pragma once
#include "control_fac.h"
#include "vg_texture.h"
namespace vg
{
	class AFG_EXPORT image :
		public control_base
	{
  DEF_STRUCT_WITH_INIT(_pt,(vec2, _right_top_offset, {0}),
    (vec2, _left_bottom_offset, {0}),
    (vec2, _right_bottom_offset, {0}),
    (texture_key, _texture_key, {0}), 
    (u16, _sub_id, {0}),
    (bool, _normal_rectangle, {true}),
	(float, _aplpha_nml,{1.f}),
	(vec3, _tin_clr))

	public:
		image();
    /*
		vec2 left_top(){ return _in_p._pos;}
    vec2 right_top() { 
      return _pt._right_top_offset + _in_p._pos; 
    }
    vec2 left_bottom() {
      return _pt._left_bottom_offset + _in_p._pos;
    }
    vec2 right_bottom() {
      return _pt._right_bottom_offset + _in_p._pos;
    }*/
    bool contain(vec2 &tar_pos) {
      if (_pt._normal_rectangle) {
        return control_base::contain(tar_pos);
      } else {
        // 判断两条线段 (p1, p2) 和 (p3, p4) 是否相交
        auto isIntersect = [](vec2& p1, vec2& p2, vec2& p3, vec2& p4) {
          // 叉积法判断线段是否相交
          auto cross = [](vec2& a, vec2& b, vec2& c) -> double {
            return (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
          };
          double d1 = cross(p1, p2, p3);
          double d2 = cross(p1, p2, p4);
          double d3 = cross(p3, p4, p1);
          double d4 = cross(p3, p4, p2);
          return (d1 * d2 < 0) && (d3 * d4 < 0);
        };
        auto  isPointInQuadrilateral=[&](vec2 & p, vec2 (&quad)[4]) {
          // 选取一条射线，比如 x 正方向 (p, p.x + 10000)
          vec2 p_inf = {p.x() + 10000, p.y()};
          int intersectCount = 0;
          // 检查射线和四边形的四条边是否相交
          for (int i = 0; i < 4; ++i) {
            vec2 p1 = quad[i];
            vec2 p2 = quad[(i + 1) % 4];
            if (isIntersect(p, p_inf, p1, p2)) {
              intersectCount++;
            }
          }
          // 如果交点个数为奇数，说明点在四边形内部
          return intersectCount % 2 == 1;
        };
        vec2 apos = absolute_coordinate_of_base_pos();
        vec2 vertex_pos[4] = {
            apos,
            apos + _pt._right_bottom_offset,
            apos + _pt._left_bottom_offset,
            apos + _pt._right_top_offset
        };
        return isPointInQuadrilateral(tar_pos, vertex_pos);
      }
      
    }
		void draw();
    DECLARE_EX_INT
	};
	REG_CTL_2_FAC(image);
}