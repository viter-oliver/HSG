
#message("cppsrc=${cppsrc}")
#list(REMOVE_ITEM cppsrc E:/non-rolling-log-application/main.cpp)
#message("cppsrc2=${cppsrc}")

add_library(Non_Rolling_Log non_rolling_log.cpp non_rolling_log.h )
add_executable(Non_Rolling_Log_TEST main.cpp)
target_link_libraries(Non_Rolling_Log_TEST Non_Rolling_Log)