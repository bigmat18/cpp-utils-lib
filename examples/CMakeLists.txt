cmake_minimum_required(VERSION 3.24)
project(cpp-utils-lib-examples)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O -Wcpp -Wall -Wextra -Wpedantic" CACHE STRING "Flags for Debug" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native" CACHE STRING "Flags for Release" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -ggdb" CACHE STRING "Flags for RelWithDebInfo" FORCE)
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto" CACHE STRING "Flags for MinSizeRel" FORCE)

message(STATUS "Building examples")
set(EXAMPLES 
    01-assert
    02-logging
    03-profiling
    04-debug
)

foreach (subdir ${EXAMPLES})
    set(SUBDIR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
    set(MAIN_FILE ${SUBDIR_PATH}/main.cpp)

    if (IS_DIRECTORY ${SUBDIR_PATH} AND EXISTS ${MAIN_FILE})

        add_executable(${subdir} ${MAIN_FILE})
        target_link_libraries(${subdir} PRIVATE cpp-utils-lib)

        if(USE_OPENMP AND TARGET OpenMP::OpenMP_CXX)
            target_link_libraries(${subdir} PRIVATE OpenMP::OpenMP_CXX)
        endif()

    else()
        message(STATUS "-- main.cpp don't exists in ${subdir}, skipped")
    endif()
endforeach()
