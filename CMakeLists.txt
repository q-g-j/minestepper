cmake_minimum_required(VERSION 3.0)

project(minesweeper)

add_executable(minesweeper src/main.cpp src/common.cpp src/drawboard.cpp src/input.cpp)

install(TARGETS minesweeper RUNTIME DESTINATION bin)
