#
# Copyright 2014-2018 Neueda Ltd.
#
cmake_minimum_required(VERSION 2.8.0)
project(utils CXX)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(SUBMODULE_FLAGS -DTESTS=off)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "default install path" FORCE )
endif()

if(APPLE)
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
  set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
  set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-undefined,dynamic_lookup")
  set(CMAKE_MACOSX_RPATH 1)
endif()

option(DEBUG "Enable debug build" OFF)
option(COVERAGE "Enable gcov coverage" OFF)

# set version info
set(UTILS_MAJOR_VERSION 0)
set(UTILS_MINOR_VERSION 1)
set(UTILS_PATCH_VERSION 0)
set(UTILS_VERSION ${UTILS_MAJOR_VERSION}.${UTILS_MINOR_VERSION}.${UTILS_PATCH_VERSION})

if (UNIX)
  macro (add_library _name)
    _add_library(${ARGV})
    if (TARGET ${_name})
      SET_TARGET_PROPERTIES(${_name}
        PROPERTIES
            VERSION ${UTILS_VERSION}
            # SOVERSION 0.0.0
        )
    endif()
  endmacro()
endif (UNIX)

# compile options
add_compile_options(-Wall)
if (DEBUG)
  message(STATUS "utils DEBUG: ON")
  add_compile_options(-g3)
  set(SUBMODULE_FLAGS ${SUBMODULE_FLAGS} -DDEBUG=on)
else ()
  message(STATUS "utils DEBUG: OFF")
  add_compile_options(-g -O2)
endif (DEBUG)
if (COVERAGE)
    set(COVERAGE_COMPILE_FLAGS "-g -O0 -coverage -fprofile-arcs -ftest-coverage")
    set(COVERAGE_LINK_FLAGS    "-coverage -lgcov")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILE_FLAGS}" )
    set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}" )
endif(COVERAGE)

# allow external projects
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

# add source
add_subdirectory(src)

# deps
find_library(EVENT_LIB
  NAMES event
  HINTS "${CMAKE_INSTALL_PREFIX}/lib")
find_library(EVENT_PTHREAD_LIB
  NAMES event_pthreads
  HINTS "${CMAKE_INSTALL_PREFIX}/lib")
if (EVENT_LIB AND EVENT_PTHREAD_LIB)
  message(STATUS "event-found: " ${EVENT_LIB})
  message(STATUS "event-pthread-found: " ${EVENT_PTHREAD_LIB})
elseif (NOT event_FOUND)
  set(LIBEVENT_CMAKE ${PROJECT_SOURCE_DIR}/ext/libevent CACHE PATH "Location of libevent-cmake" FORCE)
  message(STATUS "libevent-location: " ${LIBEVENT_CMAKE})
  ExternalProject_Add(
    libevent
    SOURCE_DIR ${LIBEVENT_CMAKE}
    CMAKE_ARGS -DEVENT__DISABLE_TESTS=ON -DEVENT__DISABLE_REGRESS=ON -DEVENT__DISABLE_OPENSSL=ON -DEVENT__DISABLE_SAMPLES=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    )

  # add include and lib dir
  link_directories(${CMAKE_INSTALL_PREFIX}/lib)
endif()

# sbf
find_package(sbf QUIET)
if(NOT ${sbf_FOUND})
  set(SBF_CMAKE ${PROJECT_SOURCE_DIR}/ext/sbf CACHE PATH "Location of sbf-cmake" FORCE)
  message(STATUS "sbf-location: " ${SBF_CMAKE})
  ExternalProject_Add(
    SBF_PROJECT
    SOURCE_DIR ${SBF_CMAKE}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} ${SUBMODULE_FLAGS}
    )
  ExternalProject_Get_Property(SBF_PROJECT binary_dir)
  set(SBF_PROJECT_DIR ${binary_dir}/lib/cmake/sbf CACHE PATH "location of sbf cmake files" FORCE)
endif()

# unit-tests
option(TESTS "Enable unit-tests" OFF)
if(TESTS)
  ExternalProject_Add(
    googletest
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/ext/googletest
    CMAKE_ARGS -DBUILD_GMOCK=ON -DBUILD_GTEST=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/googletest -DCMAKE_INSTALL_LIBDIR=${CMAKE_BINARY_DIR}/googletest/lib
    )
  
  include(CTest)
  enable_testing()
  
  # add test dir
  add_subdirectory(test)
endif()

INCLUDE(CPack)
SET(CPACK_GENERATOR "TGZ")
