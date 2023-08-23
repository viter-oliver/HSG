
#include  "bind_var.h"

#undef DEFINE_MEMBER_CONTAINER
#define DEFINE_MEMBER_CONTAINER v_sd_bind_var _v_bind_var;

#undef MEMBER_CONTAINER
#define MEMBER_CONTAINER _v_bind_var

#undef DEFINE_GET_MEMBER
#define DEFINE_GET_MEMBER sd_bind_var get_member(int id){\
return _v_bind_var[id];\
}

enum {en_last_item};

#define INIT_LAST =en_last_item
#define DEF_EUM_I(i,x) x BOOST_PP_EXPR_IF(BOOST_PP_NOT(i),INIT_LAST),
#define GEN_ENUM_I(r,data,i,elem) DEF_EUM_I(i,BOOST_PP_TUPLE_ELEM(3,1,elem))
#undef DEFINE_MEMBER_ENUM
#define DEFINE_MEMBER_ENUM(...) enum{\
BOOST_PP_SEQ_FOR_EACH_I(GEN_ENUM_I,_,BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))\
};
#define INSERT_BIND(T,x) vbind_var.emplace_back(std::make_shared<bind_var_t<T>>(PIKSTR(x),x))
#define GEN_BIND3(r,data,elem)\
 INSERT_BIND(BOOST_PP_TUPLE_ELEM(3,0,elem), BOOST_PP_TUPLE_ELEM(3,1,elem));
#undef DEFINE_PICK_MEMBER _v_bind_var
#define DEFINE_PICK_MEMBER(...)\
class_page(v_sd_bind_var& vbind_var){\
  BOOST_PP_SEQ_FOR_EACH(GEN_BIND3,_,BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))\
}

