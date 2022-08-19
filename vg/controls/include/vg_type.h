#pragma once
//#prama execution_character_set("utf-8")
#define TXT_BUFF_SZ 1024 * 16
#include <math.h>
namespace vg {
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using s8 = char;
using s16 = short;
using s32 = int;

template <class T> struct vt2 {
  T x, y;
  // cp_vt2() = default;
  vt2 &operator+=(vt2 &tar) {
    this->x += tar.x;
    this->y += tar.y;
    return *this;
  }
  vt2 &operator-=(vt2 &tar) {
    this->x -= tar.x;
    this->y -= tar.y;
    return *this;
  }
  vt2 operator+(vt2 &tar) {
    vt2 av{this->x + tar.x, this->y + tar.y};
    return av;
  }
  vt2 operator-(vt2 &tar) {
    vt2 av{this->x - tar.x, this->y - tar.y};
    return av;
  }
  bool operator!=(vt2 &tar) { return x != tar.x || y != tar.y; }
  bool operator==(vt2 &tar) { return x == tar.x && y == tar.y; }

  vt2 &operator*(T scl) {
    this->x *= scl;
    this->y *= scl;
    return *this;
  }
	vt2& operator* (vt2& tar) {
		this->x *= tar.x;
		this->y *= tar.y;
		return *this;
	}
  T x_ply(vt2 tar){
		return this->x*tar.y-this->y*tar.x;
	}
	vt2 rotate_by(vt2& tar,T angle){
		vt2 des;
		des.x=(this->x-tar.x)*cos(angle)-(this->y-tar.x)*sin(angle)+tar.x;
		des.y=(this->x-tar.x)*sin(angle)-(this->y-tar.x)*cos(angle)+tar.y;
		return des;
	}
  T norm() { return sqrt(x * x + y * y); }
};
using vi2 = vt2<s32>;
using vui2 = vt2<u32>;
using vs2 = vt2<s16>;
using vec2 = vt2<float>;
template <class T> struct vt3 {
  T x, y, z;
  // vt2() = default;
  vt3 &operator+=(vt3 &tar) {
    this->x += tar.x;
    this->y += tar.y;
    this->z += tar.z;
    return *this;
  }
  vt3 &operator-=(vt3 &tar) {
    this->x -= tar.x;
    this->y -= tar.y;
    this->z -= tar.z;
    return *this;
  }
  vt3 &operator*(T scl) {
    this->x *= scl;
    this->y *= scl;
    this->z *= scl;
    return *this;
  }
  vt3 operator+(vt3 &tar) {
    vt3 av{this->x + tar.x, this->y + tar.y, this->z + tar.z};
    return av;
  }
  vt3 operator-(vt3 &tar) {
    vt3 av{this->x - tar.x, this->y - tar.y, this->z - tar.z};
    return av;
  }
  T norm() { return sqrt(x * x + y * y + z * z); }
};
using vi3 = vt3<int>;
using vui3 = vt3<unsigned int>;
using vec3 = vt3<float>;
template <class T> struct vt4 {
  T x, y, z, w;
  // vt2() = default;
  //vt4(T _x,T _y,T _z,T _w):x(_x),y(_y),z(_z),w(_w){}
  vt4 &operator+=(vt4 &tar) {
    this->x += tar.x;
    this->y += tar.y;
    this->w += tar.w;
    this->z += tar.z;
    return *this;
  }
  vt4 &operator-=(vt4 &tar) {
    this->x -= tar.x;
    this->y -= tar.y;
    this->w -= tar.w;
    this->z -= tar.z;
    return *this;
  }
  vt4 &operator*(T scl) {
    this->x *= scl;
    this->y *= scl;
    this->z *= scl;
    this->w *= scl;
    return *this;
  }
  vt4 operator+(vt4 &tar) {
    vt4 av{this->x + tar.x, this->y + tar.y, this->z + tar.z, this->w + tar.w};
    return av;
  }
  vt4 operator-(vt4 &tar) {
    vt4 av{this->x - tar.x, this->y - tar.y, this->z - tar.z, this->w - tar.w};
    return av;
  }
  T norm() { return sqrt(x * x + y * y + z * z + w * w); }
};
using vi4 = vt4<int>;
using vui4 = vt4<unsigned int>;
using vec4 = vt4<float>;
template <typename T> struct area {
  vt2<T> left_top, right_bottom;
  bool contain(vt2<T> &tar) {
    return tar.x >= left_top.x && tar.x <= right_bottom.x &&
           tar.y >= left_top.y && tar.y <= right_bottom.y;
  }
};
using area_f = area<float>;
struct camera {
  vec3 _position;
  vec3 _direction;
  vec3 _up;
};
struct directional_light {
  vec3 _color;
  vec3 _direction;
};
struct point_light {
  vec3 _color;
  vec3 _position;
};

struct transformation {
  vec3 _scale;
  vec3 _rotation;
  vec3 _translation;
  bool _same_scale;
};

struct projection {
  float _fovy;
  // float _aspect;
  float _near;
  float _far;
};
struct point_pair {
  vec2 _point0;
  vec2 _point1;
};

#define FILE_NAME_LEN 50
} // namespace vg