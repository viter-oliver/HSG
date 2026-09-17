#ifndef NON_ROLLING_H
#define NON_ROLLING_H

#if defined (__cplusplus)
extern "C" {
#endif

#include <stdio.h>

void print_message(const char* fun_name, int line, const char* format, ...);

#if defined (__cplusplus)
#define FUNC_NAME __FUNCSIG__
#else
#define FUNC_NAME __func__
#endif

#define cc_print(...) print_message(FUNC_NAME,__LINE__,__VA_ARGS__)

#if defined (__cplusplus)
}
#endif

#endif

