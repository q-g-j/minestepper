#pragma once

#include "../include/debug.hpp"

#if !defined(_WIN32) && !defined(WIN32) && !defined(_WIN64) && !defined(WIN64)
    void enableNonCanonicalMode();
    void disableNonCanonicalMode();
#endif

class Input
{
private:
    enum class Direction : int
    {
        UP, DOWN, LEFT, RIGHT
    };

    void moveCursor(Field &field, Common::CoordsStruct&, Direction &direction);
    void helpToggle(Field &field, Common::CoordsStruct const&);

public:
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        #define KEY_UP 72
        #define KEY_DOWN 80
        #define KEY_LEFT 75
        #define KEY_RIGHT 77
        #define KEY_ENTER '\r'
        #define KEY_SPACE ' '
    #else
        #define KEY_UP 'A'
        #define KEY_DOWN 'B'
        #define KEY_LEFT 'D'
        #define KEY_RIGHT 'C'
        #define KEY_ENTER '\n'
        #define KEY_SPACE ' '
    #endif
    int getInputDifficulty();
    Common::CoordsStruct getInputDimensions();
    int getInputMinesCount(int const&);
    void getInputEnterKey(std::string const&);
    Common::UserInputReturnStruct getUserInput(Field &field, int);
    void showBlinkingCursor(bool);
};


