#include <codecvt>
#include <iostream>
#include <locale>
#include <string>
#include "time.h"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <fcntl.h>
    #include <io.h>
    #include <windows.h>
#endif

#include "common.hpp"
#include "debug.hpp"

using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> strconverter;

void Common::setUnicode(bool sw)
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        if (sw)
            _setmode(_fileno(stdout), _O_U16TEXT);
        else
            _setmode(_fileno(stdout), _O_TEXT);
    #endif
}

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
std::wstring Common::stringConvert(std::string const& str)
{
    return strconverter.from_bytes(str);
}

std::wstring Common::intToString(int const& num)
{
    return std::to_wstring(num);
}

#else
std::string Common::stringConvert(std::wstring const& wstr)
{
    return strconverter.to_bytes(wstr);
}

std::string Common::intToString(int const& num)
{
    return std::to_string(num);
}
#endif

void Common::setRandomSeed()
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        srand(GetTickCount());
    #else
        srand(time(NULL));
    #endif
}

void Common::clearScreen()
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        if (system("cls") != 0) exit(1);
    #else
        if (system("clear") != 0) exit(1);
    #endif
}

// convert coords in type integer to coords in type struct (e.g. position = 4 will return coords.col = 4, coords.row = 1):
coordsStruct Common::intToStruct(int& position, int& cols)
{
    coordsStruct coords;
    
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
int Common::structToInt(coordsStruct& coords, int& cols)
{
    int position = 0;
    if (coords.row == 1)
        return position = coords.col;
    else
        return position = (cols) * (coords.row-1) + coords.col;
}

coordsStruct Common::convCoordsToCursorPosition(coordsStruct& coords, int const& offsetX, int const& offsetY, int const& cellWidth)
{
    coordsStruct cursorPosition;
    cursorPosition.col = offsetX;
    for (int i = 1; i < coords.col; i ++)
        cursorPosition.col = cursorPosition.col + (cellWidth + 1);
    
    cursorPosition.row = offsetY;
    for (int i = 1; i < coords.row; i ++)
        cursorPosition.row = cursorPosition.row + 2;
    return cursorPosition;
}

coordsStruct Common::convCursorPositionToCoords(coordsStruct& cursorPosition, int const& offsetX, int const& offsetY, int const& cellWidth)
{
    coordsStruct coords;
    coords.col = 1;
    for (int i = cursorPosition.col; i > offsetX; i = i - cellWidth + 1)
        coords.col++;
    coords.row = 1;
    for (int i = cursorPosition.row; i > offsetY; i = i - (2))
        coords.row++;
    return coords;
}

