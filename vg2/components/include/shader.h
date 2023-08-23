#pragma once
#include <assert.h>
#include <fab/fab.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include <glad/glad.h>
#endif
#include "vg_type.h"

namespace vg {
enum
{
     en_gl_points,
     en_gl_lines,
     eg_gl_line_loop,
     en_gl_line_strip,
     en_gl_triangles,
     en_gl_triangle_strip,
     en_gl_triangle_fan,
     en_gl_count
};
extern GLuint draw_model[en_gl_count];
namespace shader {
class shader_variable;
using sd_shader_variable = std::shared_ptr<shader_variable>;
using mp_sd_shader_variable = std::map<std::string, sd_shader_variable>;
class shader_variable_type {
protected:
  u32 _location;
  u32 _ele_count;

public:
  shader_variable_type(u32 loc, u32 elc) : _location(loc), _ele_count(elc) {}
  virtual sd_shader_variable spawn_variable() = 0;
  virtual bool set_location(float *pvalue) {
    assert(false && "float variable is unmatched for this uniform!");
    return false;
  }
  virtual bool set_location(int *pvalue) {
    assert(false && "int variable is unmatched for this uniform!");
    return false;
  }
  virtual bool set_location(double *pvalue) {
    assert(false && "double variable is unmatched for this uniform!");
    return false;
  }
  virtual bool set_location(u32 *pvalue) {
    assert(false && "unsigned int variable is unmatched for this uniform!");
    return false;
  }
  virtual bool set_location(int ivalue) {
    assert(false && "texture variable is unmatched for this uniform!");
    return false;
  }
};

using sd_shader_variable_type = std::shared_ptr<shader_variable_type>;
using mp_shader_variable_type = std::map<std::string, sd_shader_variable_type>;

class shader_variable {
protected:
  sd_shader_variable_type _phost;
  u32 _buff_len = {0};

public:
  shader_variable(sd_shader_variable_type phost, u32 buff_len)
      : _phost(phost), _buff_len(buff_len) {}
  u32 get_buff_len() { return _buff_len; }
  virtual void set_data_to_host() = 0;
  // DOUBT:it maybe a bad idea to define this function
  virtual void operator=(shader_variable *pother) = 0;
  virtual void* phead_data() = 0;
  virtual u32 get_data_len() = 0;
};

template <typename T> class shader_variable_T : public shader_variable {
  T *_pdata;

public:
  shader_variable_T(sd_shader_variable_type phost, u32 buff_len)
      : shader_variable(phost, buff_len) {
    _pdata = new T[buff_len];
  }
  ~shader_variable_T() { delete[] _pdata; }
  void* phead_data() {
      return _pdata;
  }
  u32 get_data_len() {
      return sizeof(T) * _buff_len;
  }
  
  void set_data_to_host() { _phost->set_location(_pdata); }
  // FIXME:how to convert an shader_variable of other type to a variable of
  // current type?
  void operator=(shader_variable *pother) {
    if (typeid(shader_variable_T<float>) == typeid(*pother)) {
      auto cd_other = static_cast<shader_variable_T<float> *>(pother);
      *this = *cd_other;
    } else if (typeid(shader_variable_T<int>) == typeid(*pother)) {
      auto cd_other = static_cast<shader_variable_T<int> *>(pother);
      *this = *cd_other;
    } else if (typeid(shader_variable_T<u32>) == typeid(*pother)) {
      auto cd_other = static_cast<shader_variable_T<u32> *>(pother);
      *this = *cd_other;
    } else if (typeid(shader_variable_T<double>) == typeid(*pother)) {
      auto cd_other = static_cast<shader_variable_T<double> *>(pother);
      *this = *cd_other;
    }
  }
  T &operator[](unsigned int idx) {
    if (idx >= _buff_len)
      idx = 0;
    return _pdata[idx];
  }
  void init_data(std::vector<T> data) {
    auto min_len = _buff_len > data.size() ? data.size() : _buff_len;
    for (u32 ix = 0; ix < min_len; ++ix) {
      _pdata[ix] = data[ix];
    }
  }
  template <typename OT> void operator=(shader_variable_T<OT> &other) {
    auto min_len =
        _buff_len > other.get_buff_len() ? other.get_buff_len() : _buff_len;
    for (u32 ix = 0; ix < min_len; ++ix) {
      _pdata[ix] = other[ix];
    }
  }
  template <> void operator=(shader_variable_T<T> &other) {
    auto min_len = _buff_len > other._buff_len ? other._buff_len : _buff_len;
    memcpy_s(_pdata, min_len * sizeof(T), other._pdata, min_len * sizeof(T));
  }
};

struct sampler_dumy {};
template <> class shader_variable_T<sampler_dumy> : public shader_variable {
  int ivalue;

public:
  void set_data_to_host() { _phost->set_location(ivalue); }
  void operator=(shader_variable *pother) {
    if (typeid(*this) == typeid(*pother)) {
      auto cd_other = static_cast<shader_variable_T<sampler_dumy> *>(pother);
      ivalue = (*cd_other).value();
    }
  }
  void* phead_data() {
      return &ivalue;
  }
  u32 get_data_len() {
      return 4;
  }
  int &value() { return ivalue; }
};
template <int typeEnum>
class shader_variable_type_en : public shader_variable_type {};
struct fac_shadre_variable_type {
  template <typename T> struct shader_type_assist {
    static T *create_new_obj(u32 loc, u32 elc) { return new T(loc, elc); }
    shader_type_assist(u32 key) {
      get().Register(key, shader_type_assist<T>::create_new_obj);
    }
  };
  static fab::Factory<shader_variable_type, u32> &get() {
    static fab::Factory<shader_variable_type, u32> instance;
    return instance;
  }
};
#define REG_SHADER_VARIABLE_TYPE(enKey)                                        \
  static fac_shadre_variable_type::shader_type_assist<                         \
      shader_variable_type_en<enKey>>                                          \
      reg##enKey(enKey)
template<>
class shader_variable_type_en<GL_FLOAT> : public shader_variable_type {
  u32 _ele_size = 1;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<float>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(float *pvalue) {
    glUniform1fv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_FLOAT);
template<>
class shader_variable_type_en<GL_FLOAT_VEC2> : public shader_variable_type {
  u32 _ele_size = 2;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<float>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(float *pvalue) {
    glUniform2fv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_FLOAT_VEC2);
template<>
class shader_variable_type_en<GL_FLOAT_VEC3> : public shader_variable_type {
  u32 _ele_size = 3;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<float>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(float *pvalue) {
    glUniform3fv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_FLOAT_VEC3);
template<>
class shader_variable_type_en<GL_FLOAT_VEC4> : public shader_variable_type {
  u32 _ele_size = 4;

public:
  sd_shader_variable spawn_variable() {

    return std::make_shared<shader_variable_T<float>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(float *pvalue) {
    glUniform4fv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_FLOAT_VEC4);
template<>
class shader_variable_type_en<GL_FLOAT_MAT2> : public shader_variable_type {
  u32 _ele_size = 4;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<float>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(float *pvalue) {
    glUniformMatrix2fv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_FLOAT_MAT2);
template<>
class shader_variable_type_en<GL_FLOAT_MAT3> : public shader_variable_type {
  u32 _ele_size = 9;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<float>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(float *pvalue) {
    glUniformMatrix3fv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_FLOAT_MAT3);
template<>
class shader_variable_type_en<GL_FLOAT_MAT4> : public shader_variable_type {
  u32 _ele_size = 16;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<float>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(float *pvalue) {
    glUniformMatrix4fv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_FLOAT_MAT4);
template<>
class shader_variable_type_en<GL_FLOAT_MAT2x3> : public shader_variable_type {
  u32 _ele_size = 6;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<float>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(float *pvalue) {
    glUniformMatrix2x3fv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_FLOAT_MAT2x3);
template<>
class shader_variable_type_en<GL_FLOAT_MAT3x2> : public shader_variable_type {
  u32 _ele_size = 6;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<float>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(float *pvalue) {
    glUniformMatrix3x2fv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_FLOAT_MAT3x2);
template<>
class shader_variable_type_en<GL_FLOAT_MAT2x4> : public shader_variable_type {
  u32 _ele_size = 8;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<float>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(float *pvalue) {
    glUniformMatrix2x4fv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_FLOAT_MAT2x4);
template<>
class shader_variable_type_en<GL_FLOAT_MAT4x2> : public shader_variable_type {
  u32 _ele_size = 8;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<float>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(float *pvalue) {
    glUniformMatrix4x2fv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_FLOAT_MAT4x2);
template<>
class shader_variable_type_en<GL_FLOAT_MAT3x4> : public shader_variable_type {
  u32 _ele_size = 12;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<float>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(float *pvalue) {
    glUniformMatrix3x4fv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_FLOAT_MAT3x4);
template<>
class shader_variable_type_en<GL_FLOAT_MAT4x3> : public shader_variable_type {
  u32 _ele_size = 12;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<float>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(float *pvalue) {
    glUniformMatrix4x3fv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_FLOAT_MAT4x3);
template<>
class shader_variable_type_en<GL_INT> : public shader_variable_type {
  u32 _ele_size = 1;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<s32>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(int *pvalue) {
    glUniform1iv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_INT);
template<>
class shader_variable_type_en<GL_INT_VEC2> : public shader_variable_type {
  u32 _ele_size = 2;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<s32>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(int *pvalue) {
    glUniform2iv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_INT_VEC2);
template<>
class shader_variable_type_en<GL_INT_VEC3> : public shader_variable_type {
  u32 _ele_size = 3;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<s32>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(int *pvalue) {
    glUniform3iv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_INT_VEC3);
template<>
class shader_variable_type_en<GL_INT_VEC4> : public shader_variable_type {
  u32 _ele_size = 4;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<s32>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(int *pvalue) {
    glUniform4iv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_INT_VEC4);
template<>
class shader_variable_type_en<GL_BOOL> : public shader_variable_type {
  u32 _ele_size = 1;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<s32>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(int *pvalue) {
    glUniform1iv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_BOOL);
template<>
class shader_variable_type_en<GL_BOOL_VEC2> : public shader_variable_type {
  u32 _ele_size = 2;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<s32>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(int *pvalue) {
    glUniform2iv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_BOOL_VEC2);
template<>
class shader_variable_type_en<GL_BOOL_VEC3> : public shader_variable_type {
  u32 _ele_size = 3;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<s32>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(int *pvalue) {
    glUniform3iv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_BOOL_VEC3);
template<>
class shader_variable_type_en<GL_BOOL_VEC4> : public shader_variable_type {
  u32 _ele_size = 4;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<s32>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(int *pvalue) {
    glUniform4iv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_BOOL_VEC4);
template<>
class shader_variable_type_en<GL_UNSIGNED_INT> : public shader_variable_type {
  u32 _ele_size = 1;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<u32>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(u32 *pvalue) {
    glUniform1uiv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_UNSIGNED_INT);
template<>
class shader_variable_type_en<GL_UNSIGNED_INT_VEC2>
    : public shader_variable_type {
  u32 _ele_size = 2;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<u32>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(u32 *pvalue) {
    glUniform2uiv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_UNSIGNED_INT_VEC2);
template<>
class shader_variable_type_en<GL_UNSIGNED_INT_VEC3>
    : public shader_variable_type {
  u32 _ele_size = 3;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<u32>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(u32 *pvalue) {
    glUniform3uiv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_UNSIGNED_INT_VEC3);
template<>
class shader_variable_type_en<GL_UNSIGNED_INT_VEC4>
    : public shader_variable_type {
  u32 _ele_size = 4;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<u32>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(u32 *pvalue) {
    glUniform4uiv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_UNSIGNED_INT_VEC4);
#ifdef glUniform1dv
template<>
class shader_variable_type_en<GL_DOUBLE> : public shader_variable_type {
  u32 _ele_size = 1;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<double>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(double *pvalue) {
    glUniform1dv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_DOUBLE);
template<>
class shader_variable_type_en<GL_DOUBLE_VEC2> : public shader_variable_type {
  u32 _ele_size = 2;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<double>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(double *pvalue) {
    glUniform2dv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_DOUBLE_VEC2);
template<>
class shader_variable_type_en<GL_DOUBLE_VEC3> : public shader_variable_type {
  u32 _ele_size = 3;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<double>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(double *pvalue) {
    glUniform3dv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_DOUBLE_VEC3);
template<>
class shader_variable_type_en<GL_DOUBLE_VEC4> : public shader_variable_type {
  u32 _ele_size = 4;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<double>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(double *pvalue) {
    glUniform4dv(_location, _ele_count, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_DOUBLE_VEC4);
template<>
class shader_variable_type_en<GL_DOUBLE_MAT2> : public shader_variable_type {
  u32 _ele_size = 4;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<double>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(double *pvalue) {
    glUniformMatrix2dv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_DOUBLE_MAT2);
template<>
class shader_variable_type_en<GL_DOUBLE_MAT3> : public shader_variable_type {
  u32 _ele_size = 9;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<double>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(double *pvalue) {
    glUniformMatrix3dv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_DOUBLE_MAT3);
template<>
class shader_variable_type_en<GL_DOUBLE_MAT4> : public shader_variable_type {
  u32 _ele_size = 16;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<double>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(double *pvalue) {
    glUniformMatrix4dv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_DOUBLE_MAT4);
template<>
class shader_variable_type_en<GL_DOUBLE_MAT2x3> : public shader_variable_type {
  u32 _ele_size = 6;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<double>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(double *pvalue) {
    glUniformMatrix2x3dv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_DOUBLE_MAT2x3);
template<>
class shader_variable_type_en<GL_DOUBLE_MAT3x2> : public shader_variable_type {
  u32 _ele_size = 6;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<double>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(double *pvalue) {
    glUniformMatrix3x2dv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_DOUBLE_MAT3x2);
template<>
class shader_variable_type_en<GL_DOUBLE_MAT2x4> : public shader_variable_type {
  u32 _ele_size = 8;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<double>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(double *pvalue) {
    glUniformMatrix2x4dv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_DOUBLE_MAT2x4);
template<>
class shader_variable_type_en<GL_DOUBLE_MAT4x2> : public shader_variable_type {
  u32 _ele_size = 8;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<double>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(double *pvalue) {
    glUniformMatrix4x2dv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_DOUBLE_MAT4x2);
template<>
class shader_variable_type_en<GL_DOUBLE_MAT3x4> : public shader_variable_type {
  u32 _ele_size = 12;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<double>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(double *pvalue) {
    glUniformMatrix3x4dv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_DOUBLE_MAT3x4);
template<>
class shader_variable_type_en<GL_DOUBLE_MAT4x3> : public shader_variable_type {
  u32 _ele_size = 12;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<double>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(double *pvalue) {
    glUniformMatrix4x3dv(_location, _ele_count, false, pvalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_DOUBLE_MAT4x3);

#endif
template<>
class shader_variable_type_en<GL_SAMPLER_2D> : public shader_variable_type {
  u32 _ele_size = 1;

public:
  sd_shader_variable spawn_variable() {
    return std::make_shared<shader_variable_T<sampler_dumy>>(
        sd_shader_variable_type(this), _ele_size * _ele_count);
  }
  bool set_location(s32 ivalue) {
    glUniform1i(_location, ivalue);
    return true;
  }
};
REG_SHADER_VARIABLE_TYPE(GL_SAMPLER_2D);

// FIXME:how to use these?
/**
#define GL_SAMPLER_1D 0x8B5D
#define GL_SAMPLER_2D 0x8B5E
#define GL_SAMPLER_3D 0x8B5F
#define GL_SAMPLER_CUBE 0x8B60
#define GL_SAMPLER_1D_SHADOW 0x8B61
#define GL_SAMPLER_2D_SHADOW 0x8B62

#define GL_SAMPLER_1D_ARRAY 0x8DC0
#define GL_SAMPLER_2D_ARRAY 0x8DC1
#define GL_SAMPLER_1D_ARRAY_SHADOW 0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW 0x8DC4
#define GL_SAMPLER_CUBE_SHADOW 0x8DC5

#define GL_INT_SAMPLER_1D 0x8DC9
#define GL_INT_SAMPLER_2D 0x8DCA
#define GL_INT_SAMPLER_3D 0x8DCB
#define GL_INT_SAMPLER_CUBE 0x8DCC
#define GL_INT_SAMPLER_1D_ARRAY 0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY 0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_1D 0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D 0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D 0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE 0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY 0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY 0x8DD7

#define GL_SAMPLER_2D_RECT 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW 0x8B64
#define GL_SAMPLER_BUFFER 0x8DC2
#define GL_INT_SAMPLER_2D_RECT 0x8DCD
#define GL_INT_SAMPLER_BUFFER 0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT 0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_BUFFER 0x8DD8

#define GL_SAMPLER_2D_MULTISAMPLE 0x9108
#define GL_INT_SAMPLER_2D_MULTISAMPLE 0x9109
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910B
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910C
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D
 */

struct attibute_variable {
  std::string _name;
  // DOUBT: _type maybe unuseful
  // GLenum _type;
  u32 _location;
  u32 _size;
};
using sd_attibute_variable = std::shared_ptr<attibute_variable>;

using v_sd_attibute_variable = std::vector<sd_attibute_variable>;

class shader_base {
protected:
  v_sd_attibute_variable _att_list;
  mp_shader_variable_type _unf_list;
  u32 _program_id{0}, _vertex_shader{0}, _fragment_shader{0};
  void refresh_viarable_list();
  bool _valid{false}, _valid_vs{false}, _valid_fs{false};
public:
  bool build_vs_code(const char *vertex_code);
  bool build_fs_code(const char *framgent_code);
  bool valid(){return _valid;}
  bool vs_valid(){return _valid_vs;}
  bool fs_valid(){return _valid_fs;}
  bool link();
  void use();
  bool match_format(std::vector<u8> &primitive_fmt);
  template <typename T> bool uniform(std::string unf_name, T tValue) {
    auto iunf = _unf_list.find(unf_name);
    if (iunf == _unf_list.end()) {
      return false;
    }
    auto p_shd_type = iunf->second;
    return p_shd_type->set_location(tValue);
  }
  void spawn_mp_sd_shader_variable(mp_sd_shader_variable &target);
};
} // namespace shader
} // namespace vg