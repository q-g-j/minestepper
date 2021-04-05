#pragma once

// system headers:
#include <string>

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
    struct CoordsStruct
    {
        int col;
        int row;
    };
    struct TimeStruct
    {
        std::string minutes;
        std::string seconds;
    };
    struct GameModeReturnStruct
    {
        std::string difficultyString;
        int cols;
        int rows;
        int mines;
        int cellWidth;
        int offsetX;
        int offsetY;
    };
    struct UserInputReturnStruct
    {
        CoordsStruct coords;
        bool isFlag = false;
        bool isAutoFlag = false;
    };
    struct PlaceUserInputReturnStruct
    {
        bool hasLost = false;
        bool hasWon = false;
        bool isTurn = false;
    };
    
    Common() = default;
    ~Common() = default;

    // public methods:
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        static void saveScreenSize();
    #else
        static void saveScreenSize();
    #endif

    static void cleanUp();

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        static const std::wstring intToStringConv(int const&);
        static const std::wstring stringConvert(std::string const&);
        static const int stringToInt(std::wstring);

        static void setWindowProperties();
        static void centerWindow();
        static void setUnicode(bool);
    #else
        static const std::string intToStringConv(int const&);
        static const std::string stringConvert(std::wstring const&);
        static const int stringToInt(std::string);
    #endif

    static void resizeConsole(int const&, int const&);
    static void setWindowTitle(std::string const&);
    static void clearScreen();
    static void gotoXY(int const&, int const&);
    static const CoordsStruct intToCoords(int const&, int const&);
    static const int coordsToInt(CoordsStruct const&, int const&);
    static const CoordsStruct coordsToCursorPosition(CoordsStruct const&, int const&, int const&, int const&);
    static void preciseSleep(double);
    static const TimeStruct secondsToTimeStruct(int);
};
