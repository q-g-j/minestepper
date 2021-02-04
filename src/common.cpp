#include <codecvt>
#include <iostream>
#include <locale>
#include <string>
#include "time.h"

#include "../include/common.hpp"
#include "../include/debug.hpp"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <fcntl.h>
    #include <io.h>
    #include <stdio.h>
    #include <windows.h>
#endif

using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> strconverter;

// enable unicode mode in Windows to be able to print the symbols:
void Common::setUnicode(bool sw)
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        if (sw)
        {
            _setmode(_fileno(stdout), 0x00020000);
        }
        else
        {
            _setmode(_fileno(stdout), _O_TEXT);
        }
    #endif
}

// for Windows: convert a string to wide string and vice versa:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
std::wstring Common::stringConvert(std::string const& str)
{
    return strconverter.from_bytes(str);
}
#else
std::string Common::stringConvert(std::wstring const& wstr)
{
    return strconverter.to_bytes(wstr);
}
#endif

// convert an integer to string or wide string:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
std::wstring Common::intToString(int const& num)
{
    return std::to_wstring(num);
}
#else
std::string Common::intToString(int const& num)
{
    return std::to_string(num);
}
#endif

// needed for random_shuffle() (place the mines):
void Common::setRandomSeed()
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        srand(GetTickCount());
    #else
        srand(time(NULL));
    #endif
}

// clear the whole screen. Used rarely to avoid screen blinking / slow refresh during the game:
void Common::clearScreen()
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        if (system("cls") != 0) exit(1);
    #else
        //if (system("clear") != 0) exit(1);
        std::cout << "\x1B[2J\x1B[H";
    #endif
}

// convert coords of type integer to coords of type struct
// (e.g. "position = 4" will return "coords.col = 4, coords.row = 1"):
Common::CoordsStruct Common::intToCoords(int& position, int& cols)
{
    CoordsStruct coords;

    if (position <= cols)
    {
        coords.col = position;
        coords.row = 1;
    }
    else if (position % cols == 0)
    {
        coords.col = cols;
        coords.row = position / cols;
    }
    else
    {
        coords.col = position % cols;
        coords.row = position / cols + 1;
    }
    return coords;
}

// the above function the other way around
unsigned int Common::coordsToInt(CoordsStruct const& coords, int const& cols)
{
    int position = 0;
    if (coords.row == 1)
    {
        return position = coords.col;
    }
    else
    {
        return position = (cols) * (coords.row-1) + coords.col;
    }
}

// can't use raw coordinates when placing the players cursor, due to the drawn lines and the cell width:
Common::CoordsStruct Common::coordsToCursorPosition(CoordsStruct const& coords, int const& offsetX, int const& offsetY, int const& cellWidth)
{
    CoordsStruct cursorPosition;
    cursorPosition.col = offsetX;
    for (int i = 1; i < coords.col; ++i)
    {
        cursorPosition.col = cursorPosition.col + (cellWidth + 1);
    }

    cursorPosition.row = offsetY;
    for (int i = 1; i < coords.row; ++i)
    {
        cursorPosition.row = cursorPosition.row + 2;
    }
    return cursorPosition;
}
