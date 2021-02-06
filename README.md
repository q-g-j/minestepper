**Minesweeper**

A small console based version of Minesweeper to teach myself C++.

![](https://github.com/q-g-j/minesweeper/blob/master/images/screenshot.jpg?raw=true)

***Features:***
- small, medium, large and custom size
- cursor navigation with the arrow keys
- colored numbers, flags and mines.
- auto-revealing of safe cells with no neighbor
 mines
- press ENTER on a number to auto-reveal safe cells similar to the Windows-version (if flags are placed right)
- Windows version only: automatically resize and center the console window

It compiles and runs in Linux and Windows but might look a bit different on both, depending on the terminal(-settings) used.

To compile you need a c++ compiler (like gcc with g++ or clang) and optionally cmake. No extra libraries needed.

**Example for cmake in Linux:**

```
git clone https://github.com/q-g-j/minesweeper.git
mkdir -p minesweeper/build
cd minesweeper/build
cmake ..
cmake --build .
```

**Example without cmake in Linux:**

```
git clone https://github.com/q-g-j/minesweeper.git
mkdir -p minesweeper/build
cd minesweeper/build
g++ ../src/*.cpp -o minesweeper
```
