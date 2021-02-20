#pragma once

// system headers:
#include <memory>

// project headers:
#include <debug.hpp>

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
private:

public:
    struct CoordsStruct
    {
        int col = 0;
        int row = 0;
    };
    struct GameModeReturnStruct
    {
        int cols;
        int rows;
        int mines;
        int cellWidth;
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

    // public methods:
    Common();
    ~Common();

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        std::wstring intToString(int const&);
        std::wstring stringConvert(std::string const&);
        int stringToInt(std::wstring);
        void setWindowProperties();
        void centerWindow();
        void setUnicode(bool);
    #else
        std::string intToString(int const&);
        std::string stringConvert(std::wstring const&);
        int stringToInt(std::string);
    #endif

    void resizeConsole(int const&, int const&);
    void setWindowTitle(std::string const&);
    void clearScreen();
    void gotoXY(int const&, int const&);
    CoordsStruct intToCoords(int const&, int const&);
    unsigned int coordsToInt(CoordsStruct const&, int const&);
    CoordsStruct coordsToCursorPosition(CoordsStruct const&, int const&, int const&, int const&);
    void exitProgram(int const&);
};
