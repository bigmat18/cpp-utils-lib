cmake_minimum_required(VERSION 3.24)
project(cpp-utils-lib CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)
set(UTILS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils)

option(BUILD_EXAMPLES "Build examples" ON)
option(USE_OPENMP "Active openmp in utils" OFF)

option(ENABLE_ASSERT "Example asserts utils" ON)
option(ENABLE_DEBUG "Example debug utils" ON)
option(ENABLE_PROFILING "Example profiling utils" ON)
option(ENABLE_LOGGING "Example asserts utils" ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O -Wcpp -Wall -Wextra -Wpedantic" CACHE STRING "Flags for Debug" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native" CACHE STRING "Flags for Release" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -ggdb" CACHE STRING "Flags for RelWithDebInfo" FORCE)
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto" CACHE STRING "Flags for MinSizeRel" FORCE)

add_library(cpp-utils-lib INTERFACE)
target_include_directories(cpp-utils-lib INTERFACE $<BUILD_INTERFACE:${UTILS_DIR}>)
target_link_libraries(cpp-utils-lib INTERFACE stdc++exp)

if(USE_OPENMP AND OpenMP_CXX_FOUND)
    target_link_libraries(cpp-utils-lib INTERFACE OpenMP::OpenMP_CXX)
    target_compile_definitions(cpp-utils-lib INTERFACE USE_OPENMP)
endif()

if(ENABLE_ASSERT)
    target_compile_definitions(cpp-utils-lib INTERFACE ENABLE_ASSERT)
endif()

if(ENABLE_DEBUG)
    target_compile_definitions(cpp-utils-lib INTERFACE ENABLE_DEBUG)
endif()

if(ENABLE_PROFILING)
    target_compile_definitions(cpp-utils-lib INTERFACE ENABLE_PROFILING)
endif()

if(ENABLE_LOGGING)
    target_compile_definitions(cpp-utils-lib INTERFACE ENABLE_LOGGING)
endif()


if(BUILD_EXAMPLES)
    add_subdirectory(${EXAMPLES_DIR})
endif()
