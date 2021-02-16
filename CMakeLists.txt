cmake_minimum_required(VERSION 3.0)

project(minesweeper)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Choose the type of build, options are: Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

if ((MINGW) OR (CYGWIN))
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND (!WIN32))) OR (CYGWIN) OR (MINGW))
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s -Wl,--gc-sections")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND (WIN32) AND (!MSVC))
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl")
endif()

include_directories(include)

add_executable(minesweeper src/main.cpp src/colors.cpp src/common.cpp src/field.cpp src/game.cpp src/input.cpp src/print.cpp src/solver.cpp)

install(TARGETS minesweeper RUNTIME DESTINATION bin)
