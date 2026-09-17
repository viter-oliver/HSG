cmake_minimum_required(VERSION 3.14)
project(HSG C CXX)
include(ExternalProject) 
include(FetchContent)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
option(HSG_IDE "Create HSG IDE" ON)
option(GLFW_WINDOWS "Use glfw as windows system" ON)
option(ENGINE_WAYLAND "Use Wayland as platform" OFF)
option(ENGINE_SCREEN "Use Screen as platform" OFF)
option(ENGINE_WIN32 "Use windows as platform " ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#set(ENV{http_proxy} "http://127.0.0.1:7890")  
#set(ENV{https_proxy} "https://127.0.0.1:7890")  
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/running)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
  #GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(nlohmann_json)
message("src:${nlohmann_json_SOURCE_DIR} bin:${nlohmann_json_BINARY_DIR}")
include_directories(${nlohmann_json_SOURCE_DIR}/single_include)
FetchContent_Declare(
  soil2
  GIT_REPOSITORY https://github.com/SpartanJ/SOIL2.git
  GIT_TAG        1.31 # 或者使用具体的 commit hash，例如 "ae035b8"
  GIT_SHALLOW    TRUE         # 只下载最新层，加快速度并减少错误
  GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(soil2)
include_directories(${soil2_SOURCE_DIR}/src/SOIL2)
#[[

FetchContent_Declare(
  boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.86.0.beta1
)
FetchContent_MakeAvailable(boost)
FetchContent_Declare(
  msgpack-c
  GIT_REPOSITORY https://github.com/msgpack/msgpack-c.git
  GIT_TAG cpp-6.1.1
)
FetchContent_MakeAvailable(msgpack-c)
include_directories(${msgpack-c_SOURCE_DIR}/include)
]]#

ExternalProject_Add(  
    glm  
    GIT_REPOSITORY https://github.com/icaven/glm.git 
    GIT_TAG master
    PREFIX "${CMAKE_BINARY_DIR}/external/glm"  
    UPDATE_COMMAND ""  
    CONFIGURE_COMMAND ""  
    BUILD_COMMAND ""  
    INSTALL_COMMAND ""  
)  
ExternalProject_Get_Property(glm SOURCE_DIR)  
include_directories("${SOURCE_DIR}/glm")
if(GLFW_WINDOWS)
  if(ENGINE_WIN32)
    add_compile_definitions(_GLFW_WIN32)

    include_directories(
      ${HSG_SOURCE_DIR}/gl3w
      ${HSG_SOURCE_DIR}/glfw3/win32/include
      ${HSG_SOURCE_DIR}/imgui/examples/example_glfw_opengl3
      )
    add_subdirectory(gl3w)
  else()
    include_directories(${HSG_SOURCE_DIR}/glfw3/include
    ${HSG_SOURCE_DIR}/imgui/examples/example_glfw_opengl_es
    )
  endif()
  add_subdirectory(glfw3)
endif()
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.91.0-docking
)
set(IMGUI_BUILD OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(imgui)
FetchContent_Declare(
  imgui_node_editor
  GIT_REPOSITORY https://github.com/thedmd/imgui-node-editor.git
  GIT_TAG master
)
FetchContent_MakeAvailable(imgui_node_editor)
set(NODE_EDITOR_DIR ${imgui_node_editor_SOURCE_DIR})

add_library(imgui_node_editor STATIC
    ${NODE_EDITOR_DIR}/crude_json.cpp
    ${NODE_EDITOR_DIR}/imgui_canvas.cpp
    ${NODE_EDITOR_DIR}/imgui_node_editor.cpp
    ${NODE_EDITOR_DIR}/imgui_node_editor_api.cpp)

target_include_directories(imgui_node_editor PUBLIC
    ${NODE_EDITOR_DIR}
)

target_link_libraries(imgui_node_editor PUBLIC imgui)

set(Boost_USE_STATIC_LIBS        OFF)  # ON 表示使用静态库，OFF 表示动态库
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
set(BOOST_ROOT "${HSG_SOURCE_DIR}/../boost_1_79_0")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
find_package(Boost 1.79.0 REQUIRED COMPONENTS context)

include_directories(${HSG_SOURCE_DIR} 
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/misc/freetype
  ${imgui_SOURCE_DIR}/backends
  ${imgui_SOURCE_DIR}/examples/example_glfw_opengl3
  #${HSG_SOURCE_DIR}/boost_1_79_0
  #${HSG_SOURCE_DIR}/json-3.10.5/single_include
  #${HSG_SOURCE_DIR}/imgui
  ${HSG_SOURCE_DIR}/msgpack-c
  ${HSG_SOURCE_DIR}/lua
  ${HSG_SOURCE_DIR}/non_rolling_log
)
if(Boost_FOUND)
    message(STATUS "Boost include: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost libs: ${Boost_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARIES})
endif()

file(GLOB IMGUI ${imgui_SOURCE_DIR}/*.cpp ${imgui_SOURCE_DIR}/*.h )
set(IMGUI_IMPL 
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h)
add_library(imgui STATIC ${IMGUI} ${IMGUI_IMPL})
target_link_libraries(imgui glfw opengl32 ${Boost_LIBRARIES})
if(ENGINE_WIN32)
  add_executable(imgui_test ${IMGUI} ${IMGUI_IMPL} 
  ${imgui_SOURCE_DIR}/examples/example_glfw_opengl3/main.cpp
  )
  target_link_libraries(imgui_test glfw opengl32 ${Boost_LIBRARIES})
endif()

# 让 example 能找到 imgui 和 node editor
#set(imgui_DIR ${imgui_SOURCE_DIR})


if(HSG_IDE)
  add_compile_definitions(WITHIN_IDE)
  add_definitions(-DRUNNING_PATH="${CMAKE_CURRENT_SOURCE_DIR}/running/")
  set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "off assimp test" FORCE) 
  set(ASSIMP_INSTALL  OFF CACHE BOOL "off assimp install" FORCE)
  set(ASSIMP_BUILD_ASSIMP_TOOLS  OFF CACHE BOOL "choose off to diable" FORCE)
  set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)   # ✅ 改为动态库
  set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
  set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT ON CACHE BOOL "" FORCE)

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


#add_subdirectory(soil)
add_subdirectory(lua)

#add_subdirectory(imgui)
add_subdirectory(vg)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib 
${CMAKE_CURRENT_SOURCE_DIR}/running)