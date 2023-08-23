
#include "bind_base.h"
#include "lua.hpp"
#include "vg_type.h"
#include <boost/lexical_cast.hpp>
#include <cassert>
using namespace std;
namespace vg {
static lua_State *L = nullptr;
static class manager_lua {
public:
  manager_lua() {
    L = luaL_newstate();
    assert(L != nullptr && "fail to create lua state!");
    luaL_openlibs(L);
  }
  ~manager_lua() { lua_close(L); }
} s_manager_lua;
bool calculate_bind_pair(bind_pair &bp) {
  luaL_dostring(L, bp._expression.c_str());
  lua_getglobal(L, "calculate_property");
  for (const auto &sd_v : bp._var_list) {
      auto& host = sd_v->_host;
      auto& bind_obj = host->get_member(sd_v->ix);
    if (typeid(*bind_obj) == typeid(bind_var_t<int>)) {
      auto psd = static_pointer_cast<bind_var_t<int>>(bind_obj);
      lua_pushinteger(L, (*psd)._value);
    } else if (typeid(*bind_obj) == typeid(bind_var_t<float>)) {
      auto psd = static_pointer_cast<bind_var_t<float>>(bind_obj);
      lua_pushnumber(L, (*psd)._value);
    } else if (typeid(*bind_obj) == typeid(bind_var_t<double>)) {
      auto psd = static_pointer_cast<bind_var_t<double>>(bind_obj);
      lua_pushnumber(L, (*psd)._value);
    } else if (typeid(*bind_obj) == typeid(bind_var_t<bool>)) {
      auto psd = static_pointer_cast<bind_var_t<bool>>(bind_obj);
      lua_pushboolean(L, (*psd)._value);
    } else if (typeid(*bind_obj) == typeid(bind_var_t<char_50>)) {
      auto psd = static_pointer_cast<bind_var_t<char_50>>(bind_obj);
      lua_pushstring(L, (*psd)._value.value);
    } else if (typeid(*bind_obj) == typeid(bind_var_t<vi2>)) {
      auto psd = static_pointer_cast<bind_var_t<vi2>>(bind_obj);
      lua_newtable(L);
      lua_pushstring(L, "x");
      lua_pushinteger(L, (*psd)._value.x);
      lua_settable(L, -3);
      lua_pushstring(L, "y");
      lua_pushinteger(L, (*psd)._value.y);
      lua_settable(L, -3);
    } else if (typeid(*bind_obj) == typeid(bind_var_t<vi3>)) {
      auto psd = static_pointer_cast<bind_var_t<vi3>>(bind_obj);
      lua_newtable(L);
      lua_pushstring(L, "x");
      lua_pushinteger(L, (*psd)._value.x);
      lua_settable(L, -3);
      lua_pushstring(L, "y");
      lua_pushinteger(L, (*psd)._value.y);
      lua_settable(L, -3);
      lua_pushstring(L, "z");
      lua_pushinteger(L, (*psd)._value.z);
      lua_settable(L, -3);
    } else if (typeid(*bind_obj) == typeid(bind_var_t<vi4>)) {
      auto psd = static_pointer_cast<bind_var_t<vi4>>(bind_obj);
      lua_newtable(L);
      lua_pushstring(L, "x");
      lua_pushinteger(L, (*psd)._value.x);
      lua_settable(L, -3);
      lua_pushstring(L, "y");
      lua_pushinteger(L, (*psd)._value.y);
      lua_settable(L, -3);
      lua_pushstring(L, "z");
      lua_pushinteger(L, (*psd)._value.z);
      lua_settable(L, -3);
      lua_pushstring(L, "w");
      lua_pushinteger(L, (*psd)._value.w);
      lua_settable(L, -3);
    } else if (typeid(*bind_obj) == typeid(bind_var_t<vec2>)) {
      auto psd = static_pointer_cast<bind_var_t<vec2>>(bind_obj);
      lua_newtable(L);
      lua_pushstring(L, "x");
      lua_pushnumber(L, (*psd)._value.x);
      lua_settable(L, -3);
      lua_pushstring(L, "y");
      lua_pushnumber(L, (*psd)._value.y);
      lua_settable(L, -3);
    } else if (typeid(*bind_obj) == typeid(bind_var_t<vec3>)) {
      auto psd = static_pointer_cast<bind_var_t<vec3>>(bind_obj);
      lua_newtable(L);
      lua_pushstring(L, "x");
      lua_pushnumber(L, (*psd)._value.x);
      lua_settable(L, -3);
      lua_pushstring(L, "y");
      lua_pushnumber(L, (*psd)._value.y);
      lua_settable(L, -3);
      lua_pushstring(L, "z");
      lua_pushnumber(L, (*psd)._value.z);
      lua_settable(L, -3);
    } else if (typeid(*bind_obj) == typeid(bind_var_t<vec4>)) {
      auto psd = static_pointer_cast<bind_var_t<vec4>>(bind_obj);
      lua_newtable(L);
      lua_pushstring(L, "x");
      lua_pushnumber(L, (*psd)._value.x);
      lua_settable(L, -3);
      lua_pushstring(L, "y");
      lua_pushnumber(L, (*psd)._value.y);
      lua_settable(L, -3);
      lua_pushstring(L, "z");
      lua_pushnumber(L, (*psd)._value.z);
      lua_settable(L, -3);
      lua_pushstring(L, "w");
      lua_pushnumber(L, (*psd)._value.w);
      lua_settable(L, -3);
    }
  }
  int rt = lua_pcall(L, bp._var_list.size(), 1, 0);
  if (rt) {
    printf("failing to call lua function because %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
    return false;
  }
  auto &rtn = bp._calcu_unit;
  if (typeid(*rtn) == typeid(bind_var_t<int>)) {
    auto psd = static_pointer_cast<bind_var_t<int>>(rtn);
    (*psd)._value = lua_tointeger(L, -1);
  } else if (typeid(*rtn) == typeid(bind_var_t<float>)) {
    auto psd = static_pointer_cast<bind_var_t<float>>(rtn);
    (*psd)._value = lua_tonumber(L, -1);
  } else if (typeid(*rtn) == typeid(bind_var_t<double>)) {
    auto psd = static_pointer_cast<bind_var_t<double>>(rtn);
    (*psd)._value = lua_tonumber(L, -1);
  } else if (typeid(*rtn) == typeid(bind_var_t<bool>)) {
    auto psd = static_pointer_cast<bind_var_t<bool>>(rtn);
    (*psd)._value = lua_toboolean(L, -1);
  } else if (typeid(*rtn) == typeid(bind_var_t<char_50>)) {
    auto psd = static_pointer_cast<bind_var_t<char_50>>(rtn);
    const char *pc = lua_tostring(L, -1);
    strcpy((*psd)._value.value, pc);
  } else if (typeid(*rtn) == typeid(bind_var_t<vi2>)) {
    auto psd = static_pointer_cast<bind_var_t<vi2>>(rtn);
    lua_pushstring(L, "x");
    lua_gettable(L, -2);
    (*psd)._value.x = lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_pushstring(L, "y");
    lua_gettable(L, -2);
    (*psd)._value.y = lua_tointeger(L, -1);
  } else if (typeid(*rtn) == typeid(bind_var_t<vi3>)) {
    auto psd = static_pointer_cast<bind_var_t<vi3>>(rtn);
    lua_pushstring(L, "x");
    lua_gettable(L, -2);
    (*psd)._value.x = lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_pushstring(L, "y");
    lua_gettable(L, -2);
    (*psd)._value.y = lua_tointeger(L, -1);
    lua_pushstring(L, "z");
    lua_gettable(L, -2);
    (*psd)._value.z = lua_tointeger(L, -1);
  } else if (typeid(*rtn) == typeid(bind_var_t<vi4>)) {
    auto psd = static_pointer_cast<bind_var_t<vi4>>(rtn);
    lua_pushstring(L, "x");
    lua_gettable(L, -2);
    (*psd)._value.x = lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_pushstring(L, "y");
    lua_gettable(L, -2);
    (*psd)._value.y = lua_tointeger(L, -1);
    lua_pushstring(L, "z");
    lua_gettable(L, -2);
    (*psd)._value.z = lua_tointeger(L, -1);
    lua_pushstring(L, "w");
    lua_gettable(L, -2);
    (*psd)._value.w = lua_tointeger(L, -1);
  } else if (typeid(*rtn) == typeid(bind_var_t<vec2>)) {
    auto psd = static_pointer_cast<bind_var_t<vec2>>(rtn);
    lua_pushstring(L, "x");
    lua_gettable(L, -2);
    (*psd)._value.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_pushstring(L, "y");
    lua_gettable(L, -2);
    (*psd)._value.y = lua_tonumber(L, -1);
  } else if (typeid(*rtn) == typeid(bind_var_t<vec3>)) {
    auto psd = static_pointer_cast<bind_var_t<vec3>>(rtn);
    lua_pushstring(L, "x");
    lua_gettable(L, -2);
    (*psd)._value.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_pushstring(L, "y");
    lua_gettable(L, -2);
    (*psd)._value.y = lua_tonumber(L, -1);
    lua_pushstring(L, "z");
    lua_gettable(L, -2);
    (*psd)._value.z = lua_tonumber(L, -1);
  } else if (typeid(*rtn) == typeid(bind_var_t<vec4>)) {
    auto psd = static_pointer_cast<bind_var_t<vec4>>(rtn);
    lua_pushstring(L, "x");
    lua_gettable(L, -2);
    (*psd)._value.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_pushstring(L, "y");
    lua_gettable(L, -2);
    (*psd)._value.y = lua_tonumber(L, -1);
    lua_pushstring(L, "z");
    lua_gettable(L, -2);
    (*psd)._value.z = lua_tonumber(L, -1);
    lua_pushstring(L, "w");
    lua_gettable(L, -2);
    (*psd)._value.w = lua_tonumber(L, -1);
  }
  lua_settop(L, 0);
  return true;
}
v_sd_bind_pair _vbinds;
void execute_binds() {
    for (auto& bu : _vbinds) {
        calculate_bind_pair(*bu);
    }
}
} // namespace vg