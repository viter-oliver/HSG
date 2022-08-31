include_directories(components/include controls/include ../soil)

file(GLOB CP_HD components/include/*.h )
file(GLOB CT_HD controls/include/*.h)

aux_source_directory(components/source CP_SRC)
aux_source_directory(controls/source CT_SRC)

if(HSG_IDE)

  include_directories(ide ide/components ide/controls)
  aux_source_directory(ide MG_SRC)
  
  file(GLOB MG_HD ide/*.h)
  file(GLOB MG_CP_HD ide/components/*.h)
  file(GLOB MG_CP_SRC ide/components/*.cpp)
  file(GLOB MG_CT_HD ide/controls/*.h)
  file(GLOB MG_CT_SRC ide/controls/*.cpp)

  set(ALL_FILES ${CP_SRC} ${CT_SRC} ${CP_HD} ${CT_HD} ${MG_SRC} ${MG_HD} 
  ${MG_CP_SRC} ${MG_CP_HD} ${MG_CT_SRC} ${MG_CT_HD})

  source_group("components" FILES ${CP_SRC} ${CP_HD})
  source_group("controls"  FILES ${CT_SRC} ${CT_HD})
  source_group("ide" FILES ${MG_SRC} ${MG_HD})
  source_group("ide/components" FILES ${MG_CP_SRC} ${MG_CP_HD})
  source_group("ide/controls" FILES ${MG_CT_SRC} ${MG_CT_HD})
  add_executable(HSG ${ALL_FILES})
  target_link_libraries(HSG freetype glfw imgui lua soil assimp
  shlwapi opengl32 HtmlHelp)
else()
  add_library(VG SHARED ${CP_SRC} ${CT_SRC})
  if(ENGINE_WIN32)
    target_link_libraries(VG freetype glfw imgui lua soil opengl32)
  elseif(ENGINE_WAYLAND)
    target_link_libraries(VG wayland-client wayland-server wayland-cursor
     EGL GLESv2 m pthread dl freetype glfw imgui lua soil)
  elseif(ENGINE_SCREEN)
    target_link_libraries(VG EGL GLESv2 m screen pthread dl freetype glfw imgui lua soil)
  endif()
endif()