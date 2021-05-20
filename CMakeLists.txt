cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

# don't let cmake guess file extensions
if(POLICY CMP0115)
    cmake_policy(SET CMP0115 NEW)
endif()

project(cppTango)

# omniORB>=4.2.4 is required for compilation in C++17 (or better) mode.
if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(BUILD_SHARED_LIBS "Build a shared library instead of static" ON)
option(TANGO_USE_LIBCPP "Build against libc++" OFF)

option(USE_PCH "Use precompiled header for server/tango.h" ON)
if (USE_PCH AND NOT (CMAKE_CXX_COMPILER_ID MATCHES "GNU|MSVC|Clang"))
    message(WARNING "Precompiled headers are not supported for selected compiler.")
    set(USE_PCH OFF)
endif()

option(TANGO_ENABLE_COVERAGE "Instrument code for coverage analysis" OFF)
if (TANGO_ENABLE_COVERAGE AND NOT (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"))
    message(WARNING "Code coverage is not supported for selected compiler.")
    set(TANGO_ENABLE_COVERAGE OFF)
endif()

set(TANGO_ENABLE_SANITIZER "" CACHE STRING "Enable sanitizer")
set(TANGO_SUPPORTED_SANITIZERS ASAN TSAN UBSAN MSAN)
set_property(CACHE TANGO_ENABLE_SANITIZER PROPERTY STRINGS ${TANGO_SUPPORTED_SANITIZERS})

if(TANGO_ENABLE_SANITIZER AND NOT (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"))
    message(FATAL_ERROR "Sanitizers are not supported for selected compiler.")
endif()
if(TANGO_ENABLE_SANITIZER AND NOT (TANGO_ENABLE_SANITIZER IN_LIST TANGO_SUPPORTED_SANITIZERS))
    message(FATAL_ERROR "TANGO_ENABLE_SANITIZER must be one of: ${TANGO_SUPPORTED_SANITIZERS}.")
endif()
if(TANGO_ENABLE_SANITIZER STREQUAL "MSAN" AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "MSAN is not supported for selected compiler.")
endif()
if(TANGO_ENABLE_SANITIZER)
    find_program(ADDR2LINE_PROGRAM "addr2line")
    find_program(LLVM_SYMBOLIZER_PROGRAM "llvm-symbolizer")
    if(NOT ADDR2LINE_PROGRAM)
        message(FATAL_ERROR "Could not find addr2line (required for sanitizers).")
    endif()
    if(NOT LLVM_SYMBOLIZER_PROGRAM AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(FATAL_ERROR "Could not find llvm-symbolizer (required for sanitizers).")
    endif()
endif()

#need to define the version of the library
set(MAJOR_VERSION "9")
set(MINOR_VERSION "4")
set(PATCH_VERSION "0")

if(WIN32)
    include(configure/cmake_win_defs.cmake)
    include(configure/cmake_pch_win.cmake)
else()
    include(configure/cmake_pch_linux.cmake)
endif()

#convenient versions
set(LIBRARY_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
set(SO_VERSION "${MAJOR_VERSION}${MINOR_VERSION}")

find_package(Git QUIET)
if(EXISTS "${CMAKE_SOURCE_DIR}/.git" AND Git_FOUND)
  execute_process(
      COMMAND "${GIT_EXECUTABLE}" describe --tags --exclude=*test* --always HEAD
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      OUTPUT_VARIABLE GIT_DESCRIBE_STDOUT
      ERROR_VARIABLE GIT_DESCRIBE_STDERR
      RESULT_VARIABLE GIT_DESCRIBE_RESULT
      OUTPUT_STRIP_TRAILING_WHITESPACE)

  if(GIT_DESCRIBE_RESULT)
    # older git versions (< 2.13) don't know about --exclude
    # in case we errored out, retry without it
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" describe --tags --always HEAD
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_DESCRIBE_STDOUT
        ERROR_VARIABLE GIT_DESCRIBE_STDERR
        RESULT_VARIABLE GIT_DESCRIBE_RESULT
        OUTPUT_STRIP_TRAILING_WHITESPACE)
  endif()

  if(GIT_DESCRIBE_RESULT)
    message(WARNING "Could not fetch the git revision (error ${GIT_DESCRIBE_RESULT}): ${GIT_DESCRIBE_STDERR}")
    set(GIT_DESCRIBE_STDOUT "unknown")
  endif()
else()
    set(GIT_DESCRIBE_STDOUT "unknown")
endif()

set(TANGO_GIT_REVISION "${GIT_DESCRIBE_STDOUT}")

set(TANGO_HOST $ENV{TANGO_HOST})
include(configure/CMakeLists.txt)

include_directories(cppapi/client)
include_directories(cppapi/client/helpers)
include_directories(cppapi/server)
include_directories(cppapi/server/idl)
include_directories(log4tango/include)
#required for generated config.h
include_directories(${CMAKE_CURRENT_BINARY_DIR}/cppapi/server)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/log4tango/include)

#source code
add_subdirectory("log4tango")
add_subdirectory("cppapi")

if(BUILD_TESTING)

  find_program(DOCKER_BINARY docker)
  if(NOT DOCKER_BINARY)
      message(WARNING "The tests can not be run as docker is missing.")
  endif()

  if(BUILD_SHARED_LIBS)
    add_subdirectory("cpp_test_suite")
  else()
    message(WARNING "Not building the tests, because that is currently supported only when BUILD_SHARED_LIBS is ON")
    set(BUILD_TESTING OFF)
  endif()
endif()

if(WIN32)
    include(configure/cmake_win.cmake)
    include(configure/cpack_win.cmake)
else()
    include(configure/cmake_linux.cmake)
endif()

include(CPack)
