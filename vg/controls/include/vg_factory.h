#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#ifdef WITHIN_IDE
#include "control_def_ide.h"
using control_base=vg::control_def_ide;
#else
#include "control_def.h"
using control_base=vg::control_def;
#endif

namespace vg{
template<type baseType>
struct factory {
  template <typename T> struct register_t {
    register_t(const std::string &key) {
#if defined(__QNXNTO__)
      // cout<< "register control:"<<key<<endl;
      auto &fmap = factory::get().map_;
      auto itc = fmap.find(key);
      if (itc == fmap.end()) {
        // cout<<"begin reg:"<<key<<endl;
        factory::get().map_[key] = [] { return new T(); };
        // cout<<"end reg:"<<key<<endl;
      } else {
        // cout<< " fail to register control:"<<key<<endl;
      }
#else
      factory<baseType>::get().map_.emplace(key, [] { return new T(); });
#endif
    }
#if 1
    template <typename... Args>
    register_t(const std::string &key, Args... args) {
      factory<baseType>::get().map_.emplace(key, [&] { return new T(args...); });
    }
#endif
  };

  inline baseType *produce(const std::string &key) {
		auto ifun_construcor=map_.find(key);
    if (ifun_construcor== map_.end())
      throw std::invalid_argument("the message key is not exist!");
		auto pcontrol_obj=ifun_construcor->second();
		
    return pcontrol_obj;
  }
#if 0
	template<typename... Args>
	baseType* produce(const std::string& key, Args&&... args)
	{
		if (map_.find(key) == map_.end())
			throw std::invalid_argument("the message key is not exist!");

		return map_[key](std::forward<Args>(args)...);
	}
#endif
  std::unique_ptr<baseType> produce_unique(const std::string &key) {
    return std::unique_ptr<baseType>(produce(key));
  }

  std::shared_ptr<baseType> produce_shared(const std::string &key) {
    return std::shared_ptr<baseType>(produce(key));
  }
  void register_res_constrained(std::string cname,
                                std::function<bool()> res_constrained) {
    auto &ac_map = factory<baseType>::get().map_ac_;
    auto itc = ac_map.find(cname);
    if (itc != ac_map.end()) {
      printf("%s has existed!\n", cname.c_str());
    } else {
      ac_map[cname] = res_constrained;
    }
  }
	void register_init_fun(std::string& className, )
  void
  iterate_types(std::function<void(std::string, std::function<baseType *()>)> imp) {
    for (auto it : map_) {
      bool be_valid = true;
      auto iac = map_ac_.find(it.first);
      if (iac != map_ac_.end()) {
        be_valid = iac->second();
      }
      imp((it.first), be_valid ? it.second : nullptr);
    }
  }

private:
  factory(){};
  factory(const factory &) = delete;
  factory(factory &&) = delete;

  std::map<std::string, std::function<baseType *()>> map_;
  std::map<std::string, std::function<bool()>> map_ac_;
public:
  inline static factory<baseType> &get() {
    static factory<baseType> instance;
    return instance;
  }
};
// std::map<std::string, std::function<baseType*()>> factory::map_;

#define REGISTER_CONTROL_VNAME(T) reg_control_##T##_
#define REGISTER_CONTROL_BYKEY(T, key, ...)                                    \
  static factory<control_base>::register_t<T> REGISTER_CONTROL_VNAME(T)(key, ##__VA_ARGS__);

#define REGISTER_CONTROL(T, ...) REGISTER_CONTROL_BYKEY(T, #T, ##__VA_ARGS__)

}