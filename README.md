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
- for the lazy: press "f" or "F" during a game to have the computer place all the flags for you (could win the game instantly)
- press "b" or "B" during a game to toggle cursor jump on or off (when cursor reaches one edge, jump to opposite edge - this can be useful on large game fields)
- TODO: add a demo mode (kind of "A.I.")

It compiles and runs in Linux and Windows but might look a bit different on both, depending on the terminal(-settings) used.<br/><br/>
Not every (graphical) terminal program in Linux seems to support automatic resizing. Your terminal program needs to support control sequences. To enable them for xterm and compatible terminals edit <br/>
```~/.Xdefaults```<br/>
```
*VT100.allowWindowOps: true
*VT100.allowTitleOps: true
*VT100.allowFontOps: true
```
Then log out and log back in.

## Compilation:

To compile you need a c++ compiler (like clang or gcc with g++) and optionally cmake (with e.g. make or ninja). No extra libraries needed.

***Example for cmake and ninja in Linux:***

```
git clone https://github.com/q-g-j/minesweeper.git
mkdir -p minesweeper/build
cd minesweeper/build
cmake -G "Ninja" ..
cmake --build .
```
