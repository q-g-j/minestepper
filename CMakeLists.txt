cmake_minimum_required(VERSION 3.0)

project(minesweeper)

SET(BUILD_SHARED_LIBS OFF)
SET(CMAKE_EXE_LINKER_FLAGS "-static")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s -Wl,--gc-sections")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -Wl,--gc-sections")
endif()

#IF (NOT WIN32)
#    set(CMAKE_CXX_FLAGS "-lncurses -ltinfo")
#    find_package(Curses REQUIRED)
#    include_directories(${CURSES_INCLUDE_DIR})
#ENDIF()

include_directories(include)
add_executable(minesweeper src/main.cpp src/colors.cpp src/field.cpp src/common.cpp src/input.cpp src/print.cpp )

#IF (NOT WIN32)
#    target_link_libraries(minesweeper ${CURSES_LIBRARIES})
#ENDIF()

install(TARGETS minesweeper RUNTIME DESTINATION bin)
