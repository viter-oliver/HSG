cmake_minimum_required(VERSION 2.8)
project(HSG C CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
option(HSG_IDE "Create HSG IDE" ON)
option(GLFW_WINDOWS "Use glfw as windows system" ON)
option(ENGINE_WAYLAND "Use Wayland as platform" OFF)
option(ENGINE_SCREEN "Use Screen as platform" OFF)
option(ENGINE_WIN32 "Use windows as platform " ON)
include_directories(${HSG_SOURCE_DIR} 
  ${HSG_SOURCE_DIR}/boost_1_79_0
  ${HSG_SOURCE_DIR}/json-3.10.5/single_include
  ${HSG_SOURCE_DIR}/imgui
  ${HSG_SOURCE_DIR}/msgpack-c
)
if(HSG_IDE)
  add_compile_definitions(WITHIN_IDE)
  set(ASSIMP_BUILD_TESTS CACHE OFF FORCE) 
  set(ASSIMP_INSTALL CACHE OFF FORCE)
  include_directories(
    ${HSG_SOURCE_DIR}/assimp-5.2.4/include
    ${HSG_SOURCE_DIR}/freetype-2.6.5/include
  )
  add_subdirectory(assimp-5.2.4)
  set(BUILD_SHARED_LIBS OFF)
  add_subdirectory(freetype-2.6.5)
  #target_compile_features(freetype PRIVATE BUILD_SHARED_LIBS=OFF)
  set(ENGINE_SCREEN  CACHE OFF FORCE)
  set(ENGINE_WAYLAND  CACHE OFF FORCE)
  set(ENGINE_WIN32  CACHE OFF FORCE)
else()

endif()
if(GLFW_WINDOWS)
  if(ENGINE_WIN32)
    add_compile_definitions(_GLFW_WIN32)

    include_directories(
      ${HSG_SOURCE_DIR}/gl3w
      ${HSG_SOURCE_DIR}/glfw3/win32/include
      )
    add_subdirectory(gl3w)
  else()
    include_directories(${HSG_SOURCE_DIR}/glfw3/include)
  endif()
  add_subdirectory(glfw3)
endif()

add_subdirectory(soil)
add_subdirectory(lua)

add_subdirectory(imgui)
add_subdirectory(vg)