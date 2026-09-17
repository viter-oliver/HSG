#include "shader.h"
namespace vg {
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
    return false;
  }
  if (_program_id == 0) {
    _program_id = glCreateProgram();
  }

  glAttachShader(_program_id, _vertex_shader);
  glAttachShader(_program_id, _fragment_shader);
  glLinkProgram(_program_id);
  GLint status;
  glGetProgramiv(_program_id, GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    _valid_pm = false;
    glGetProgramInfoLog(_program_id, compile_buff_len, NULL, compile_buff);
    printf("shader linker error:%s\n", compile_buff);
    return false;
  }
  _valid_pm = true;
  refresh_viarable_list();
  return true;
}
void shader_base::use() { glUseProgram(_program_id); }
bool shader_base::match_format(std::vector<u8> &vertec_fmt) {
  if (!valid() || vertec_fmt.size() != _att_list.size()) {
    return false;
  }
  for (u8 ix = 0; ix < vertec_fmt.size(); ++ix) {
    const auto &fmt_stride = vertec_fmt[ix];
    const auto &attr_unit = *_att_list[ix];
    if (fmt_stride != attr_unit._size) {
      return false;
    }
  }
  return true;
}

GLuint shader_base::type2target(ShaderTargetType target_type)
{
    switch (target_type)
    {
        case ShaderTargetType::VERTEX_SHADER: { return  _vertex_shader; break; }
        case ShaderTargetType::FRAGMENT_SHADER: { return   _fragment_shader; break; }
        case ShaderTargetType::PROGRAM: { return  _program_id; break; }
        default:
            return 0;
    }
}
bool shader_base::compile(ShaderTargetType target_type, std::string& error_info)
{
    GLuint target = type2target(target_type);
    if (target == 0)return 0;
    glCompileShader(target);

    bool ret = check_shader_error(target_type, ShaderCheckType::COMPILE, error_info);
    bool cur_valid = true;
    if (ret == false)
    {
        cur_valid = false;
        printf("shader error:%s\n", error_info.c_str());
    }
    if (target_type == ShaderTargetType::VERTEX_SHADER)
        _valid_vs = cur_valid;
    else
        _valid_fs = cur_valid;
    return cur_valid;
}
bool shader_base::link(std::string& error_info) {
    if (!_valid_vs || !_valid_fs) {
        return false;
    }
    if (_program_id == 0) {
        _program_id = glCreateProgram();
    }

    glAttachShader(_program_id, _vertex_shader);
    glAttachShader(_program_id, _fragment_shader);
    glLinkProgram(_program_id);
    bool ret = check_shader_error(ShaderTargetType::PROGRAM, ShaderCheckType::LINK, error_info);
    if (ret == GL_FALSE) {
        _valid_pm = false;
        return false;
    }
    _valid_pm = true;
    refresh_viarable_list();
    return true;
}
void shader_base::set_source(ShaderTargetType target_type, const std::string& source)
{
    const char* cur_vs_code = source.c_str();
    if (target_type == ShaderTargetType::VERTEX_SHADER)
    {
        if (_vertex_shader == 0)
            _vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(_vertex_shader, 1, &cur_vs_code, 0);
    }
    else if (target_type == ShaderTargetType::FRAGMENT_SHADER)
    {
        if (_fragment_shader == 0)
            _fragment_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(_fragment_shader, 1, &cur_vs_code, 0);
    }

}
bool shader_base::get_source(ShaderTargetType target_type, std::string& source)
{
    GLint cur_source_length = 0;
    u32 target = type2target(target_type);

    glGetShaderiv(target, GL_SHADER_SOURCE_LENGTH, &cur_source_length);
    if (cur_source_length > 0) {
        source.resize(cur_source_length);
        glGetShaderSource(target, cur_source_length, nullptr, &source[0]);
    }
    return cur_source_length;
}
bool shader_base::check_shader_error(ShaderTargetType target_type, ShaderCheckType type, std::string& error_info)
{
    GLint success = 0;
    constexpr int LOG_SIZE = 1024;
    char info_log[LOG_SIZE]{};
    GLuint target = type2target(target_type);

    if (type == COMPILE)
    {
        glGetShaderiv(target, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(target, LOG_SIZE, NULL, info_log);
            error_info = std::string(info_log);
            return 0;
        }
    }
    else if (type == LINK)
    {
        glGetProgramiv(target, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(target, 1024, NULL, info_log);
            error_info = std::string(info_log);
            return 0;
        }
    }
    return 1;
}



} // namespace shader
} // namespace vg