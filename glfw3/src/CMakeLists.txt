   
set(common_SOURCES context.c init.c input.c monitor.c  window.c)
if (ENGINE_WAYLAND)
    add_definitions(-D_GLFW_WAYLAND)
    set(glfw_HEADERS wl_platform.h linux_joystick.h choose_config.h
                    posix_time.h posix_thread.h xkb_unicode.h egl_context.h)
    set(glfw_SOURCES ${common_SOURCES} wl_init.c wl_monitor.c wl_window.c
                     linux_joystick.c posix_time.c posix_thread.c xkb_unicode.c
                     egl_context.c choose_config.c)
elseif(ENGINE_SCREEN)
    message(STATUS,"it is qnx screen")
    add_definitions(-D_GLFW_SCREEN)
    set(glfw_HEADERS screen_platform.h screen_joystick.h choose_config.h
                    posix_time.h posix_thread.h xkb_unicode.h egl_context.h)
    set(glfw_SOURCES ${common_SOURCES} screen_init.c screen_monitor.c screen_window.c
                     screen_joystick.c posix_time.c posix_thread.c xkb_unicode.c
                     egl_context.c  choose_config.c)
else()
   message(FATAL_ERROR "you must select a platform!")
endif()


add_library(glfw ${glfw_SOURCES} ${glfw_HEADERS})
set_target_properties(glfw PROPERTIES
                      OUTPUT_NAME ${GLFW_LIB_NAME}
                      VERSION ${GLFW_VERSION}
                      SOVERSION ${GLFW_VERSION_MAJOR}
                      POSITION_INDEPENDENT_CODE ON)
IF("$ENV{ARCH}" STREQUAL "arm64")
   MESSAGE(STATUS "this is imx8 platform")
   add_definitions(-DIMX8_PLATFORM)
   set(LIBRARY_OUTPUT_PATH ${GLFW_SOURCE_DIR}/../RV_cluster/afg_sdk/lib)
   set(LIB_OUTPUT_PATH ${GLFW_SOURCE_DIR}/../../arhud/ar_hud/RV/afg_sdk/lib)
   #set(LIBRARY_OUTPUT_PATH ${LIB_OUTPUT_PATH})
   #ADD_CUSTOM_COMMAND(TARGET glfw POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${LIB_OUTPUT_PATH}/libglfw.a ${GLFW_SOURCE_DIR}/../RV_cluster/afg_sdk/lib)
ELSE()
   MESSAGE(STATUS "this is imx6 platform")
   add_definitions(-DIMX6_PLATFORM)
   #set(LIBRARY_OUTPUT_PATH ${GLFW_SOURCE_DIR}/../../arhud/ar_hud/e115/afg_sdk/lib)
   set(LIBRARY_OUTPUT_PATH ${GLFW_SOURCE_DIR}/../../arhud/ar_hud/e115_test_new/afg_sdk/lib)
ENDIF()