cmake_minimum_required(VERSION 3.2)
project(cpp-utils-lib CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)
set(UTILS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils)

option(CPPUTILS_BUILD_EXAMPLES "Build examples" ON)
option(CPPUTILS_USE_OPENMP "Enable OpenMP" OFF)
option(CPPUTILS_ENABLE_WARNINGS "Enable recommended warnings" OFF)
option(CPPUTILS_ENABLE_PROFILING "Profiling utils" ON)
option(CPPUTILS_ENABLE_ASSERT "Enable assertions" ON)
option(CPPUTILS_ENABLE_DEBUG "Enable debug utilities" ON)
option(CPPUTILS_ENABLE_LOGGING "Enable logging" ON)


add_library(cpp-utils-lib INTERFACE)
target_include_directories(cpp-utils-lib INTERFACE ${UTILS_DIR})

if(CPPUTILS_USE_OPENMP)
    find_package(OpenMP QUIET)
    if(OpenMP_CXX_FOUND)
        target_link_libraries(cpp-utils-lib INTERFACE OpenMP::OpenMP_CXX)
        target_compile_definitions(cpp-utils-lib INTERFACE USE_OPENMP)
        target_link_libraries(cpp-utils-lib INTERFACE 
            $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Linux>:stdc++exp>
        )
    endif()
endif()

if(CPPUTILS_ENABLE_WARNINGS)
    target_compile_options(cpp-utils-lib INTERFACE 
        -Wall 
        -Wextra 
        -Wpedantic 
        -Wcpp 
        $<$<CXX_COMPILER_ID:Clang>:-Weverything -Wno-c++98-compat>
        $<$<CXX_COMPILER_ID:GNU>:-Wshadow=local -Wformat=2>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /wd4996>
    )
endif()

if(CPPUTILS_ENABLE_ASSERT)
    target_compile_definitions(cpp-utils-lib INTERFACE ENABLE_ASSERT)
endif()

if(CPPUTILS_ENABLE_DEBUG)
    target_compile_definitions(cpp-utils-lib INTERFACE ENABLE_DEBUG)
endif()

if(CPPUTILS_ENABLE_LOGGING)
    target_compile_definitions(cpp-utils-lib INTERFACE ENABLE_LOGGING)
endif()

if(CPPUTILS_ENABLE_PROFILING)
    target_compile_definitions(cpp-utils-lib INTERFACE ENABLE_PROFILING)
endif()

if(CPPUTILS_BUILD_EXAMPLES)
    add_subdirectory(${EXAMPLES_DIR})
endif()
