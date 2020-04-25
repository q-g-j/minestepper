cmake_minimum_required(VERSION 3.0)

project(minesweeper)

add_executable(minesweeper main.cpp)

install(TARGETS minesweeper RUNTIME DESTINATION bin)
