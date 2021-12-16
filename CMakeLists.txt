cmake_minimum_required(VERSION 3.1)

project(minesweeper)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Choose the type of build, options are: Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

if ((MINGW) OR (CYGWIN))
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT (WIN32)) OR (CYGWIN) OR (MINGW)))
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s -Wl,--gc-sections")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND (WIN32) AND (NOT (MSVC)))
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl")
endif()

if (NOT (WIN32))
    link_libraries(pthread)
endif()

include_directories(include)

add_executable(minesweeper src/main.cpp src/colors.cpp src/common.cpp src/field.cpp src/game.cpp src/input.cpp src/print.cpp src/solver.cpp src/symbols.cpp)

install(TARGETS minesweeper RUNTIME DESTINATION bin)
