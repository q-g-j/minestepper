cmake_minimum_required(VERSION 3.0)

project(minesweeper)

if (((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") AND (WIN32)) OR (CYGWIN))
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND (!WIN32))) OR (CYGWIN))
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s -Wl,--gc-sections")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND (WIN32) AND (!MSVC))
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl")
endif()

add_executable(minesweeper src/main.cpp src/colors.cpp src/field.cpp src/common.cpp src/input.cpp src/print.cpp src/solver.cpp)

install(TARGETS minesweeper RUNTIME DESTINATION bin)
