#pragma once
#include "vg_type.h"
#include "VarEditorRegistry.h"
//#include "TypeEditor.h"
#include "bind_var_def.h"
namespace vg
{

    template <class T>
    struct var_base_t :public var_base
    {
        T _value;
        void get_memory_info(void*& address, int& len) {
            address = &_value;
            len = sizeof(T);
        }
        sd_bind_var get_bind(const std::string& name); 
    };
  template<class T>
  struct bind_var_t :public bind_var
  {
    std::string _var_name;
    T& _value;
    bind_var_t( std::string name, T& value ) :_var_name( name ), _value( value )
    {
      //(void)TypeEditor<T>::_auto_reg;
    }
    std::string var_name()
    {
      return _var_name;
    }
    
    sd_var_base spawn()
    {
      auto sd_v = std::make_shared<var_base_t<T>>();
      sd_v->_value = _value;
      return sd_v;
    }
    sd_bind_var clone()
    {
      auto sd_bind_var_t = std::make_shared<bind_var_t>( _var_name, _value );
      return sd_bind_var_t;
    }
    bind_var_t<T>& operator = ( var_base_t<T>& var )
    {
      _value = var._value;
      return *this;
    }

    bool assign( sd_var_base& sbase )
    {
      if( typeid( *sbase ) == typeid( var_base_t<T> ) )
      {
        auto sd_var_base_t = std::static_pointer_cast<var_base_t<T>>( sbase );
        _value = sd_var_base_t->_value;
        return true;
      }
      return false;
    }
    

    template<typename U = T>
    std::enable_if_t<
        is_instance_of_vg_array<U>::is_vg_array,bool>
        set_location(typename is_instance_of_vg_array<U>::value_type* pvalue)
    {
        auto msz = _value.size * sizeof(U::value_type);
        memcpy_s(_value.value, msz, pvalue, msz);
        return true;
    }
/**    template<typename U = T>
    std::enable_if_t<
        !is_instance_of_vg_array<U>::is_vg_array, bool>
        set_location(U* pvalue)
    {
        _value = *pvalue;
        return true;
    }
    
template<typename U = T>
    std::enable_if_t < std::is_class_v<U>&&
        requires { typename U::value_type; typename std::remove_reference_t<U>::size; }, bool >
        set_location(typename U::value_type* pvalue)
    {
        auto msz = _value.size * sizeof(U::value_type);
        memcpy_s(_value.value, msz, pvalue, msz);
        return true;
    }*/
    bool pass_value(void* pvalue)
    {
      if constexpr (is_instance_of_vg_array<T>::value) {
          auto msz = _value.size * sizeof(T);
         memcpy_s(_value.value, msz, pvalue, msz);
      } else {
        _value = *((T*) pvalue);
      }
      return true;
    }
    void get_void_ptr(void*& pvalue) {
      if constexpr (is_instance_of_vg_array<T>::value) {
        pvalue=_value.value;
      } else {
        pvalue=&_value;
      }
    }
    void value_info(int& type_value,int& count){
      count=1;
      type_value = type_value_number;
      if constexpr (std::is_same_v<T, u8>) {
        type_value= u8_value;
      }
      if constexpr (std::is_same_v<T, u16>) {
        type_value= u16_value;
      }
      if constexpr (std::is_same_v<T, u32>) {
        type_value= u32_value;
      }
      if constexpr (std::is_same_v<T, s8>) {
        type_value= s8_value;
      }
      if constexpr (std::is_same_v<T, s16>) {
        type_value= s16_value;
      }
      if constexpr (std::is_same_v<T, s32>) {
        type_value= s32_value;
      }
      if constexpr (std::is_same_v<T, float>) {
        type_value= f32_value;
      }
      if constexpr (std::is_same_v<T, double>) {
        type_value= f64_value;
      }
      if constexpr (std::is_same_v<T, bool>) {
          type_value = bool_value;
      }
      if constexpr (is_instance_of_vg_array<T>::value) {
        type_value = _value.type_value();
        count = _value.dimention();
      }
    }
    bool scale( sd_var_base from, sd_var_base to, double s )
      /*requires IsNotChar<T> */
    {
      if constexpr( std::is_base_of<char_50, T>::value )
      {
        return false;
      }
      else
      {
        if( typeid( *from ) != typeid( var_base_t<T> )
            || typeid( *to ) != typeid( var_base_t<T> ) )
             return false;
          if constexpr (is_instance_of_vg_array<T>::value) {
             auto sd_from = std::static_pointer_cast<var_base_t<T>>(from);
             auto sd_to = std::static_pointer_cast<var_base_t<T>>(to);

             auto  valueSp= sd_to->_value - sd_from->_value;
             _value = sd_from->_value +valueSp *s;
          } else {
            auto sd_from = std::static_pointer_cast<var_base_t<T>>( from );
            auto sd_to = std::static_pointer_cast<var_base_t<T>>( to );
            auto valueSp = sd_to->_value - sd_from->_value;
            _value = sd_from->_value + valueSp * s;
          }
          return true;
      }  
    }
#
    void edit(IVarEditor* editor) override {
        if (editor){
          int type_value, type_count;
          value_info(type_value, type_count);
          if constexpr (is_instance_of_vg_array<T>::value) {
            editor->edit_var(_var_name, _value.value, type_value, type_count);
          } else {
            editor->edit_var(_var_name, &_value, type_value, type_count);
          }
        }      
    }
    void edit()
    {
      VarEditorRegistry::instance().edit(_var_name, _value);
    }
  };
  template<class T>
  sd_bind_var var_base_t<T>::get_bind(const std::string& name)
  {
      return std::make_shared<bind_var_t<T>>(name, _value);
  }
}