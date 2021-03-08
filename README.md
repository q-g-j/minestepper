# Minesweeper

A console based version of Minesweeper to teach myself C++.

![](https://github.com/q-g-j/minesweeper/blob/master/images/game_small.jpg?raw=true)

## Controls:
- Arrow keys:    move cursor
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
- automatically resize the console window (Linux: see notes below)
- Windows version only: automatically center the console window

It compiles and runs in Linux and Windows but might look a bit different on both, depending on the terminal(-settings) used.

Linux version: to make automatic resizing work you need a terminal that supports control sequences, e.g. "xterm".
By default xterm might look quite ugly and won't have control sequences enabled. I recommend to install a nice looking mono font like "Hack":
```
sudo apt install fonts-hack-ttf
```
Now edit ```~/.Xdefaults```:
```
*VT100.allowWindowOps: true
*VT100.allowTitleOps: true
*VT100.allowFontOps: true

XTerm*background: black
XTerm*foreground: lightgray
XTerm*renderFont: true
XTerm*faceName: Hack
XTerm*faceSize: 10
```

Finally type in a terminal (and re-open xterm):
```
xrdb ~/.Xdefaults
```

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
