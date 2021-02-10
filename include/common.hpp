#pragma once

#include <string>

#include "../include/debug.hpp"

// Windows and Linux seem to handle unicode strings differently (use wide strings for Windows):
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #define coutconv std::wcout
    #define stringconv std::wstring
    #define tostringconv std::to_wstring
#else
    #define coutconv std::cout
    #define stringconv std::string
    #define tostringconv std::to_string
#endif

const char newline = '\n';

class Common
{
public:
    struct SmallStruct
    {
        int columns;
        int rows;
        int mines;
    };
    struct MediumStruct
    {
        int columns;
        int rows;
        int mines;
    };
    struct LargeStruct
    {
        int columns;
        int rows;
        int mines;
    };
    struct DifficultyStruct
    {
        SmallStruct Small;
        MediumStruct Medium;
        LargeStruct Large;
    };
    struct CoordsStruct
    {
        int col = 0;
        int row = 0;
    };
    struct UserInputReturnStruct
    {
        CoordsStruct Coords;
        bool isFlag = false;
        bool isAutoFlag = false;
    };
    struct PlaceUserInputReturnStruct
    {
        bool hasLost = false;
        bool hasWon = false;
        bool isTurn = false;
    };

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        int stringToInt(std::wstring);
    #else        
        int stringToInt(std::string);
    #endif

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        stringconv intToString(int const&);
//        stringconv stringConvert(std::string const&);
    #else
        std::string intToString(int const&);
//        std::string stringConvert(std::wstring const&);
    #endif

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        void setWindowProperties();
        void centerWindow();
        void setUnicode(bool);
    #endif

    void resizeConsole(int const&, int const&);
    void setWindowTitle(std::string const&);
    void setRandomSeed();
    void clearScreen();
    CoordsStruct intToCoords(int const&, int const&);
    unsigned int coordsToInt(CoordsStruct const&, int const&);
    CoordsStruct coordsToCursorPosition(CoordsStruct const&, int const&, int const&, int const&);
};
