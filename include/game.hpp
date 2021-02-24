#pragma once

// system headers:
#include <memory>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

// project headers:
#include <common.hpp>
#include <debug.hpp>

class Colors;
class Common;
class Field;
class Input;
class Print;

class Game
{
private:    
    // declare class objects as unique pointers:
    std::unique_ptr<Colors> colors;
    std::unique_ptr<Common> common;
    std::unique_ptr<Input> input;
    std::unique_ptr<Print> print;

    // define the 3 main game modes:
    const int smallCols       = 9;
    const int smallRows       = 9;
    const int smallMines      = 10;

    const int mediumCols      = 16;
    const int mediumRows      = 16;
    const int mediumMines     = 40;

    const int largeCols       = 30;
    const int largeRows       = 16;
    const int largeMines      = 99;

    // set X/Y position of the field
    const int fieldOffsetX    = 5;
    const int fieldOffsetY    = 6;

    // set width of each cell (must be an odd number!):
    const int fieldCellWidth  = 3;

    int difficulty = 1;
    std::string difficultyString;

    Common::GameModeReturnStruct chooseGamemode();
    
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        static unsigned __stdcall gameThread(void*);
        static unsigned __stdcall timerThread(void*);
    #else
        static void* gameThread(void*);
        static void* timerThread(void*);
    #endif
    
public:
    Game();
    ~Game();

    void startGame();
};