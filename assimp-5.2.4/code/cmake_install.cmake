# Install script for directory: D:/workspace/hsg/assimp-5.2.4/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp5.2.0-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/workspace/hsg/assimp-5.2.4/lib/Debug/assimp-vc142-mtd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/workspace/hsg/assimp-5.2.4/lib/Release/assimp-vc142-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/workspace/hsg/assimp-5.2.4/lib/MinSizeRel/assimp-vc142-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/workspace/hsg/assimp-5.2.4/lib/RelWithDebInfo/assimp-vc142-mt.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp5.2.0x" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/workspace/hsg/assimp-5.2.4/bin/Debug/assimp-vc142-mtd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/workspace/hsg/assimp-5.2.4/bin/Release/assimp-vc142-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/workspace/hsg/assimp-5.2.4/bin/MinSizeRel/assimp-vc142-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/workspace/hsg/assimp-5.2.4/bin/RelWithDebInfo/assimp-vc142-mt.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/anim.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/aabb.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/ai_assert.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/camera.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/color4.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/color4.inl"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/config.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/ColladaMetaData.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/commonMetaData.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/defs.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/cfileio.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/light.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/material.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/material.inl"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/matrix3x3.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/matrix3x3.inl"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/matrix4x4.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/matrix4x4.inl"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/mesh.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/ObjMaterial.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/pbrmaterial.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/GltfMaterial.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/postprocess.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/quaternion.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/quaternion.inl"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/scene.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/metadata.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/texture.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/types.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/vector2.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/vector2.inl"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/vector3.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/vector3.inl"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/version.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/cimport.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/importerdesc.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/Importer.hpp"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/DefaultLogger.hpp"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/ProgressHandler.hpp"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/IOStream.hpp"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/IOSystem.hpp"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/Logger.hpp"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/LogStream.hpp"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/NullLogger.hpp"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/cexport.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/Exporter.hpp"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/DefaultIOStream.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/DefaultIOSystem.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/ZipArchiveIOSystem.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/SceneCombiner.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/fast_atof.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/qnan.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/BaseImporter.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/Hash.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/MemoryIOWrapper.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/ParsingUtils.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/StreamReader.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/StreamWriter.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/StringComparison.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/StringUtils.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/SGSpatialSort.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/GenericProperty.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/SpatialSort.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/SkeletonMeshBuilder.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/SmallVector.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/SmoothingGroups.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/SmoothingGroups.inl"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/StandardShapes.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/RemoveComments.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/Subdivision.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/Vertex.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/LineSplitter.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/TinyFormatter.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/Profiler.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/LogAux.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/Bitmap.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/XMLTools.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/IOStreamBuffer.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/CreateAnimMesh.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/XmlParser.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/BlobIOSystem.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/MathFunctions.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/Exceptional.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/ByteSwapper.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/Base64.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/Compiler/pushpack1.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/Compiler/poppack1.h"
    "D:/workspace/hsg/assimp-5.2.4/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/workspace/hsg/assimp-5.2.4/code/Debug/assimp-vc142-mtd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/workspace/hsg/assimp-5.2.4/code/RelWithDebInfo/assimp-vc142-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

