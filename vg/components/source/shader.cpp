#include "shader.h"
namespace vg {
GLuint draw_model[en_gl_count] = {
    GL_POINTS,    GL_LINES,          GL_LINE_LOOP,    GL_LINE_STRIP,
    GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN,
};
namespace shader {
using namespace std;
struct vtype_size {
  int _utsize, _cnt;
  vtype_size(int us, int ct) : _utsize(us), _cnt(ct) {}
  vtype_size() : _utsize(0), _cnt(0) {}
};
map<GLenum, vtype_size> shader_variable_type_size = {
    {GL_FLOAT, {sizeof(float), 1}},
    {GL_FLOAT_VEC2, {sizeof(float), 2}},
    {GL_FLOAT_VEC3, {sizeof(float), 3}},
    {GL_FLOAT_VEC4, {sizeof(float), 4}},

    {GL_INT, {sizeof(int), 1}},
    {GL_INT_VEC2, {sizeof(int), 2}},
    {GL_INT_VEC3, {sizeof(int), 3}},
    {GL_INT_VEC3, {sizeof(int), 3}},
    {GL_INT_VEC4, {sizeof(int), 4}},
    {GL_UNSIGNED_INT, {sizeof(unsigned int), 1}},
    {GL_UNSIGNED_INT_VEC2, {sizeof(unsigned int), 2}},
    {GL_UNSIGNED_INT_VEC3, {sizeof(unsigned int), 3}},
    {GL_UNSIGNED_INT_VEC4, {sizeof(unsigned int), 4}},
    {GL_BOOL, {sizeof(bool), 1}},
    {GL_BOOL_VEC2, {sizeof(bool), 2}},
    {GL_BOOL_VEC3, {sizeof(bool), 3}},
    {GL_BOOL_VEC4, {sizeof(bool), 4}},
    {GL_FLOAT_MAT2, {sizeof(float), 2 * 2}},
    {GL_FLOAT_MAT3, {sizeof(float), 3 * 3}},
    {GL_FLOAT_MAT4, {sizeof(float), 4 * 4}},
    {GL_FLOAT_MAT2x3, {sizeof(float), 2 * 3}},
    {GL_FLOAT_MAT2x4, {sizeof(float), 2 * 4}},
    {GL_FLOAT_MAT3x2, {sizeof(float), 3 * 2}},
    {GL_FLOAT_MAT3x4, {sizeof(float), 3 * 4}},
    {GL_FLOAT_MAT4x2, {sizeof(float), 4 * 2}},
    {GL_FLOAT_MAT4x3, {sizeof(float), 4 * 3}},
#ifdef GL_DOUBLE_VEC2
    {GL_DOUBLE, {sizeof(double), 1}},
    {GL_DOUBLE_VEC2, {sizeof(double), 2}},
    {GL_DOUBLE_VEC3, {sizeof(double), 3}},
    {GL_DOUBLE_VEC4, {sizeof(double), 4}},
    {GL_DOUBLE_MAT2, {sizeof(double), 2 * 2}},
    {GL_DOUBLE_MAT3, {sizeof(double), 3 * 3}},
    {GL_DOUBLE_MAT4, {sizeof(double), 4 * 4}},
    {GL_DOUBLE_MAT2x3, {sizeof(double), 2 * 3}},
    {GL_DOUBLE_MAT2x4, {sizeof(double), 2 * 4}},
    {GL_DOUBLE_MAT3x2, {sizeof(double), 3 * 2}},
    {GL_DOUBLE_MAT3x4, {sizeof(double), 3 * 4}},
    {GL_DOUBLE_MAT4x2, {sizeof(double), 4 * 2}},
    {GL_DOUBLE_MAT4x3, {sizeof(double), 4 * 3}},
#endif
};
const int compile_buff_len = 0x200;
static char compile_buff[compile_buff_len];

void shader_base::refresh_viarable_list() {
  GLint idx;
  GLint count;
  GLint size;  // size of the variable
  GLenum type; // type of the variable (float, vec3 or mat4, etc)

  const GLsizei bufSize = 256; // maximum name length
  GLchar name[bufSize];        // variable name in GLSL
  GLsizei length;              // name length
  glGetProgramiv(_program_id, GL_ACTIVE_ATTRIBUTES, &count);
  printf("Active Attributes: %d\n", count);
  _att_list.resize(count);
  for (idx = 0; idx < count; idx++) {
    // DOUBT:
    /** GL_FLOAT, GL_FLOAT_VEC2, GL_FLOAT_VEC3, GL_FLOAT_VEC4, GL_FLOAT_MAT2,
     GL_FLOAT_MAT3, GL_FLOAT_MAT4, GL_FLOAT_MAT2x3, GL_FLOAT_MAT2x4,
     GL_FLOAT_MAT3x2, GL_FLOAT_MAT3x4, GL_FLOAT_MAT4x2, GL_FLOAT_MAT4x3, GL_INT,
     GL_INT_VEC2, GL_INT_VEC3, GL_INT_VEC4, GL_UNSIGNED_INT,
     GL_UNSIGNED_INT_VEC2, GL_UNSIGNED_INT_VEC3, GL_UNSIGNED_INT_VEC4,
     GL_DOUBLE, GL_DOUBLE_VEC2, GL_DOUBLE_VEC3, GL_DOUBLE_VEC4, GL_DOUBLE_MAT2,
     GL_DOUBLE_MAT3, GL_DOUBLE_MAT4, GL_DOUBLE_MAT2x3, GL_DOUBLE_MAT2x4,
     GL_DOUBLE_MAT3x2, GL_DOUBLE_MAT3x4, GL_DOUBLE_MAT4x2, or GL_DOUBLE_MAT4x3
     maybe returned to type,but only GL_FLOAT,GL_FLOAT_VEC2,GL_FLOAT_VEC3,
     GL_FLOAT_VEC4 are actually used in the shader, so type is temporarily
     ignored here
     */
    glGetActiveAttrib(_program_id, (GLuint)idx, bufSize, &length, &size, &type,
                      name);
    printf("Attribute #%d Type: 0x%x Name: %s\n", idx, type, name);
    GLuint location = glGetAttribLocation(_program_id, name);
    //_att_list[name] = shader_variable(type, location, size);
    sd_attibute_variable &_sd_attibute_variable = _att_list[location];
    if (_sd_attibute_variable == nullptr) {
      _sd_attibute_variable = make_shared<attibute_variable>();
    }
    auto &attr_value = *_sd_attibute_variable;
    auto type_size = size * shader_variable_type_size[type]._cnt;
    attr_value = {name, location, (u32)type_size};
  }

  glGetProgramiv(_program_id, GL_ACTIVE_UNIFORMS, &count);
  printf("Active Uniforms: %d\n", count);
  _unf_list.clear();
  for (idx = 0; idx < count; idx++) {
    glGetActiveUniform(_program_id, (GLuint)idx, bufSize, &length, &size, &type,
                       name);
    printf("Uniform #%d Type: 0x%x Name: %s\n", idx, type, name);
    GLint location = glGetUniformLocation(_program_id, name);
    _unf_list[name] =
        fac_shadre_variable_type::get().Create(type, location, size);
  }
}
bool shader_base::build_vs_code(const char *vertex_code) {
  if (_vertex_shader == 0) {
    _vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  }
  glShaderSource(_vertex_shader, 1, &vertex_code, 0);
  glCompileShader(_vertex_shader);
  GLint status;
  glGetShaderiv(_vertex_shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    _valid_vs = false;
    glGetShaderInfoLog(_vertex_shader, compile_buff_len, NULL, compile_buff);
    printf("vertex shader error:%s\n", compile_buff);
    return false;
  }
  _valid_vs = true;
  return true;
}

bool shader_base::build_fs_code(const char *fragment_code) {
  if (_fragment_shader == 0) {
    _fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  }
  glShaderSource(_fragment_shader, 1, &fragment_code, 0);
  glCompileShader(_fragment_shader);
  GLint status;
  glGetShaderiv(_fragment_shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    _valid_fs = false;
    glGetShaderInfoLog(_fragment_shader, compile_buff_len, NULL, compile_buff);
    printf("fragment shader error:%s\n", compile_buff);
    return false;
  }
  _valid_fs = true;
  return true;
}
bool shader_base::link() {
  if (!_valid_vs || !_valid_fs) {
    _valid = false;
    return false;
  }
  if (_program_id == 0) {
    _program_id = glCreateProgram();
  }

  glAttachShader(_program_id, _vertex_shader);
  glAttachShader(_program_id, _fragment_shader);
  glLinkProgram(_program_id);
  GLint status;
  glGetShaderiv(_program_id, GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    _valid = false;
    return false;
  }
  _valid = true;
  refresh_viarable_list();
  return true;
}
void shader_base::use() { glUseProgram(_program_id); }
bool shader_base::match_format(std::vector<u8> &primitive_fmt) {
  if (!_valid || primitive_fmt.size() != _att_list.size()) {
    return false;
  }
  for (u8 ix = 0; ix < primitive_fmt.size(); ++ix) {
    const auto &fmt_stride = primitive_fmt[ix];
    const auto &attr_unit = *_att_list[ix];
    if (fmt_stride != attr_unit._size) {
      return false;
    }
  }
  return true;
}
void shader_base::spawn_mp_sd_shader_variable(mp_sd_shader_variable &target) {
  for (const auto &unf_u : _unf_list) {
    target[unf_u.first] = unf_u.second->spawn_variable();
  }
}
mp_sd_shder_base _mp_sd_shder_bases;
} // namespace shader
} // namespace vg