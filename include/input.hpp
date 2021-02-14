#pragma once

#include "../include/debug.hpp"

#if !defined(_WIN32) && !defined(WIN32) && !defined(_WIN64) && !defined(WIN64)
    void enableNonCanonicalMode();
    void disableNonCanonicalMode();
#endif

// forward declarations of classes:
class Colors;
class Common;
class Field;
class Print;
class Solver;
class Symbols;

class Input
{
private:
    // declare class objects as unique pointers:
    std::unique_ptr<Colors> colors;
    std::unique_ptr<Common> common;
    std::unique_ptr<Print> print;
    std::unique_ptr<Solver> solver;
    std::unique_ptr<Symbols> symbols;

    enum class Direction : int
    {
        UP, DOWN, LEFT, RIGHT
    };

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        const char KEY_UP = 72;
        const char KEY_DOWN = 80;
        const char KEY_RIGHT = 77;
        const char KEY_LEFT = 75;
        const char KEY_ENTER = '\r';
        const char KEY_SPACE = ' ';
    #else
        const char KEY_UP = 'A';
        const char KEY_DOWN = 'B';
        const char KEY_RIGHT = 'C';
        const char KEY_LEFT = 'D';
        const char KEY_ENTER = '\n';
        const char KEY_SPACE = ' ';
    #endif

    // private methods:
    void moveCursor(Field&, Common::CoordsStruct&, Direction&, bool*);
    void helpToggle(Field&, Common::CoordsStruct const&);

public:
    Input();
    ~Input();

    // public methods:
    int getInputDifficulty();
    Common::CoordsStruct getInputCustomDimensions();
    int getInputCustomMinesCount(int const&);
    void getInputEnterKey(std::string const&);
    Common::UserInputReturnStruct getUserInput(Field&, int);
    void showBlinkingCursor(bool);
};


