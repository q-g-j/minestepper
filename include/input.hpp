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

    void moveCursor(Field&, Common::CoordsStruct&, Direction&, bool*);
    void helpToggle(Field&, Common::CoordsStruct const&);

public:
    int getInputDifficulty();
    Common::CoordsStruct getInputCustomDimensions();
    int getInputCustomMinesCount(int const&);
    void getInputEnterKey(std::string const&);
    Common::UserInputReturnStruct getUserInput(Field&, int);
    void showBlinkingCursor(bool);
};


