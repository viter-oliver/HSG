
if(ENGINE_WIN32)
  
  file(GLOB glfw_source win32/*.c)
  add_library(glfw  SHARED ${glfw_source})
  target_link_libraries(glfw gl3w)
else()#ENGINE_WIN32
  add_subdirectory(src)
endif()