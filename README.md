# Minesweeper

A console based version of Minesweeper to teach myself C++.

![](https://github.com/q-g-j/minesweeper/blob/master/images/game_small.jpg?raw=true)

## Features:
- small, medium, large and custom size
- cursor navigation with the arrow keys
- colored numbers, flags and mines.
- auto-revealing of safe cells with no neighbor
 mines
- press ENTER on a number to auto-reveal safe cells similar to the Windows-version (if flags are placed right)
- automatically resize the console window
- Windows version only: automatically center the console window

It compiles and runs in Linux and Windows but might look a bit different on both, depending on the terminal(-settings) used.<br/>
Not every (graphical) terminal program in Linux seems to support automatically resizing. <br/>
Your terminal program needs to support control sequences. To enable them for xterm and compatible terminals edit <br/>
```~/.Xdefaults```<br/>
```
*VT100.allowWindowOps: true
*VT100.allowTitleOps: true
*VT100.allowFontOps: true
```
Then log out and log back in.

## Compilation:

To compile you need a c++ compiler (like gcc with g++) and optionally cmake. No extra libraries needed.


***Example for cmake in Linux:***

```
git clone https://github.com/q-g-j/minesweeper.git
mkdir -p minesweeper/build
cd minesweeper/build
cmake ..
cmake --build .
```
