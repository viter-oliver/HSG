#pragma once
//#prama execution_character_set("utf-8")
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/pop_front.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/variadic/elem.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#define TXT_BUFF_SZ 1024 * 16
#define vg_print(...) printf("%s:%d ",__func__,__LINE__);printf(__VA_ARGS__)
#include <math.h>
#include <string>
#include <map>
#include <memory>
#include <type_traits>
#include <stdexcept>  
namespace vg {
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using s8 = char;
using s16 = short;
using s32 = int;
using f32 = float;
using f64 = double;
struct frame_texture{};
enum {
  u8_value,
  u16_value,
  u32_value,
  s8_value,
  s16_value,
  s32_value,
  f32_value,
  f64_value,
  bool_value,
  texture_value,
  type_value_number
};
#if 0
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

struct directional_light {
  vec3 _color;
  vec3 _direction;
};
struct point_light {
  vec3 _color;
  vec3 _position;
};


struct point_pair {
  vec2 _point0;
  vec2 _point1;
};
#endif
template<typename T>
concept IsChar = std::is_same_v<T, char>;
template <typename T>
concept IsNotChar = !std::is_same_v<T, char>;
template<class T,int n>
struct vg_array {
	T value[n];
    const int size = n;
	T& operator[](int i) {
        if (i < 0 || i >= n) {
          throw std::out_of_range("Index out of range of vg_array");
        }  
		return value[i];
	}

    T& x() requires (n >= 2) { return value[0]; }
    T& y() requires (n >= 2) { return value[1]; }
    T& z() requires (n >= 3) { return value[2]; }
    T& w() requires (n == 4) { return value[3]; }

    const T& x() const requires (n >= 2) { return value[0]; }
    const T& y() const requires (n >= 2) { return value[1]; }
    const T& z() const requires (n >= 3) { return value[2]; }
    const T& w() const requires (n == 4) { return value[3]; }
    T norm() const {
      T sum = 0;
      for (int ix = 0; ix < n; ix++) {
        sum += value[ix] * value[ix];
      }
      return sqrt(sum);
    }

  int dimention() { return n; }
  int type_value() { 
    if constexpr (std::is_same_v<T, u8>) {
      return u8_value;
    }
    if constexpr (std::is_same_v<T, u16>) {
      return u16_value;
    }
    if constexpr (std::is_same_v<T, u32>) {
      return u32_value;
    }
    if constexpr (std::is_same_v<T, s8>) {
      return s8_value;
    }
    if constexpr (std::is_same_v<T, s16>) {
      return s16_value;
    }
    if constexpr (std::is_same_v<T, s32>) {
      return s32_value;
    }
    if constexpr (std::is_same_v<T, f32>) {
      return f32_value;
    }
    if constexpr (std::is_same_v<T, f64>) {
      return f64_value;
    }
    if constexpr (std::is_same_v<T, bool>) {
        return bool_value;
    }
    return type_value_number; 
  }
	template <typename U=T>
	requires std::is_same_v<U,char>
	constexpr bool operator <( const vg_array<U,n>& char_key ) const
	{
		return std::string( value ) < std::string( char_key.value );
	}
  #if 1
    vg_array& operator =(const vg_array& other)
    {
        auto msz = n * sizeof(T);
        memcpy_s( value, msz, other.value, msz );
        return *this;
    }
    vg_array& operator -=(const vg_array& other)
    {
        for (int ix = 0; ix < n; ix++) {
        value[ix] -= other[ix];
        }
        return *this;
    }
  vg_array& operator +=( vg_array& other)
  {
      for (int ix = 0; ix < n; ix++) {
          value[ix] += other[ix];
      }
      return *this;
  }
  vg_array operator-(vg_array &other) const {
    vg_array vtmp;
    for (int ix = 0; ix < n; ix++) {
      vtmp[ix]=value[ix] - other[ix];
    }
    return vtmp;
  }
  vg_array operator+(vg_array &other) const {
    vg_array vtmp;
    for (int ix = 0; ix < n; ix++) {
      vtmp[ix] = value[ix] + other[ix];
    }
    return vtmp;
  }
	vg_array& operator=( const std::string& str ) requires IsChar<T>
	{
		//assert( n > str.size() );
		strncpy_s( value, str.c_str(), str.size() );
		value[ str.size() ] = 0;
		return *this;
	}
    bool operator==(const vg_array& other) const {
      for (int ix = 0; ix < n; ix++) {
        if (value[ix] != other[ix]) {
          return false;
        }
      }
      return true;
	}
    bool operator >(const vg_array& other) const {
		auto self_value = norm();
		auto other_value = other.norm();
        return self_value> other_value;
	}
    bool operator >=(const vg_array& other) const {
        auto self_value = norm();
        auto other_value = other.norm();
        return self_value >= other_value;
    }
  #if 0
  vg_array &operator=(const char* cstr)requires IsChar<T>
  {
    auto str_len = strlen(cstr);
    strncpy_s(value, cstr, str_len);
    value[str_len] = 0;
    return *this;
  }
  #endif
  vg_array& operator*( double s ){ 
    for (int ix = 0; ix < n; ix++) {
      value[ix] = value[ix] * s;
    }
    return *this;
  }
  #endif
	vg_array& get_str_value( const std::string& str ) requires IsChar<T>
	{
		//assert( n > str.size() );
		strncpy_s( value, str.c_str(), str.size() );
		value[ str.size() ] = 0;
		return *this;
	}

};
template <typename T> struct is_instance_of_vg_array : std::false_type {
	static constexpr bool is_vg_array = false;
};
template <typename T,int n>
struct is_instance_of_vg_array<vg_array<T, n>> : std::true_type {
    using value_type = T;          // 提取类型
    static constexpr int size = n; // 提取维度
    static constexpr bool is_vg_array = true;
};
using vi2 = vg_array<s32,2>;
using vui2 = vg_array<u32,2>;
using vs2 = vg_array<s16,2>;
using vec2 = vg_array<f32,2>;

using vi3 = vg_array<s32,3>;
using vui3 = vg_array<u32,3>;
using vec3 = vg_array<f32,3>;

using vi4 = vg_array<s32,4>;
using vui4 = vg_array<u32,4>;
using vec4 = vg_array<f32,4>;
template <typename T> struct area {
    vg_array<T,2> left_top, right_bottom;
    bool contain(vg_array<T, 2>& tar) {
        return tar.x() >= left_top.x() && tar.x() <= right_bottom.x() &&
            tar.y() >= left_top.y() && tar.y() <= right_bottom.y();
    }
};
using area_f = area<float>;
#define FILE_NAME_LEN 50
//using char_50 = vg_array<char, 50>;

#define ARRAY_T(t,n) using t##_##n=vg_array<t,n>;
#define DEF_ARRAY(t,n) ARRAY_T(t,n)
#define PRED(r, state) \
   BOOST_PP_NOT_EQUAL( \
      BOOST_PP_TUPLE_ELEM(3, 0, state), \
      BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(3, 1, state)) \
   ) 
#define OP_INC(r, state) \
   ( \
      BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(3, 0, state)), \
      BOOST_PP_TUPLE_ELEM(3, 1, state),BOOST_PP_TUPLE_ELEM(3, 2, state) \
   ) 
#define DEF_ARRAY_V3(r,data,elem) DEF_ARRAY(data,elem)
#define DEF_TYPE_ARRAY(t,...) \
BOOST_PP_SEQ_FOR_EACH(DEF_ARRAY_V3,t,BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

//GEN_ARRAY(10, 100, char)
DEF_TYPE_ARRAY( char, 20,30,50, 100, 200)
using base_key = char_50;
template<class T> struct key_t :public base_key{};
template<typename>
struct is_key_t : std::false_type {};

template<typename U>
struct is_key_t<key_t<U>> : std::true_type {};

#define DEF_MACRO(r, state) DEF_ARRAY(BOOST_PP_TUPLE_ELEM(3, 2, state),BOOST_PP_TUPLE_ELEM(3, 0, state))
#define GEN_ARRAY(from,to,type) BOOST_PP_FOR((from,to,type),PRED,OP_INC,DEF_MACRO)

//DEF_TYPE_ARRAY(float,4,16,50,70)
//DEF_TYPE_ARRAY(vec2,3,10)
GEN_ARRAY( 1, 20, f32)
GEN_ARRAY( 1, 20, s32 )
GEN_ARRAY( 1, 20, u32 )
GEN_ARRAY( 1, 20, f64 )

#define DEF_TYPE_DIC(type) using type##_key=key_t<type>;\
using sd_##type=std::shared_ptr<type>;\
using type##_map=std::map<type##_key,sd_##type>;\
inline type##_map type##_map_;
} // namespace vg