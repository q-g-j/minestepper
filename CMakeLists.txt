cmake_minimum_required(VERSION 3.0)

project(minestepper)

SET(BUILD_SHARED_LIBS OFF)
SET(CMAKE_EXE_LINKER_FLAGS "-static")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s -Wl,--gc-sections")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -Wl,--gc-sections")
endif()

add_executable(minestepper src/main.cpp src/field.cpp src/common.cpp src/input.cpp)

install(TARGETS minestepper RUNTIME DESTINATION bin)
