cmake_minimum_required(VERSION 3.1)
project(MsdnCpuId CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

file(GLOB SRCS *.c *.cpp *.cxx *.cc)
add_executable(
  MsdnCpuId
  ${SRCS})

include(../../cmake/flags.cmake)

if(DEFLIST)
  foreach(DEF "${DEFLIST}")
    add_definitions(${DEF})
  endforeach(DEF)
endif()

foreach(TARGET_FLAG
    C_FLAGS
    C_FLAGS_DEBUG
    C_FLAGS_RELEASE C_FLAGS_RELWITHDEBINFO
    C_FLAGS_MINSIZEREL
    CXX_FLAGS
    CXX_FLAGS_DEBUG
    CXX_FLAGS_RELEASE
    CXX_FLAGS_RELWITHDEBINFO
    CXX_FLAGS_MINSIZEREL
    EXE_LINKER_FLAGS
    EXE_LINKER_FLAGS_DEBUG
    EXE_LINKER_FLAGS_RELEASE
    EXE_LINKER_FLAGS_RELWITHDEBINFO
    EXE_LINKER_FLAGS_MINSIZEREL)
  set("CMAKE_${TARGET_FLAG}" "${${TARGET_FLAG}}")
endforeach(TARGET_FLAG)
