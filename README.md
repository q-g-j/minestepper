# Minesweeper

A console based version of Minesweeper to teach myself C++.

![](https://github.com/q-g-j/minesweeper/blob/master/images/game_small.jpg?raw=true)

## Controls:
- Arrow keys:    navigate
- ENTER:         uncover
- SPACE:         place or remove a flag
- 'f' or 'F':    let the computer place the flags for you
- 'r' or 'R':    trigger auto revealing (after placing flags)
- 's' or 'S':    automatically place flags, auto-reveal and repeat recursively
- 'c' or 'C':    toggle cursor jump to opposite edge on or off
- 'q' or 'Q':    quit

## Features:
- small, medium, large and custom size
- colored numbers, flags and mines.
- auto-revealing of connected safe cells when a "zero" (no neighbor mines) is hit
- press ENTER on a number to auto-reveal connected safe cells similar to the Windows-version (if flags are placed right)
- show a timer while playing (using threads)
- Windows version only: automatically resize and center the console window

It compiles and runs in Linux and Windows but might look a bit different on both, depending on the terminal(-settings) used.

## Compilation:

To build from source you need cmake and a C++ compiler (like clang or g++) that supports at least C++-14.

***Example for cmake and ninja in Linux:***

```
git clone https://github.com/q-g-j/minesweeper.git
mkdir -p minesweeper/build
cd minesweeper/build
cmake -G "Ninja" ..
cmake --build .
```
