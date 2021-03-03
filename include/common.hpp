#pragma once

// system headers:
#include <memory>
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

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    void saveScreenSize();
#else
    void saveScreenSize();
#endif

void cleanUp();

class Common
{
public:
    struct CoordsStruct
    {
        int col;
        int row;
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
    struct TimeStruct
    {
        std::string minutes;
        std::string seconds;
    };

private:
    // return variables for class methods:
    stringconv stringConvertReturn;
    stringconv intToStringConvReturn;
    int stringToIntReturn;
    CoordsStruct intToCoordsReturn;
    int coordsToIntReturn;
    CoordsStruct coordsToCursorPositionReturn;
    TimeStruct secondsToTimeStructReturn;

public:
    Common() = default;
    ~Common() = default;

    // public methods:
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        const std::wstring &intToStringConv(int const&);
        const std::wstring &stringConvert(std::string const&);
        const int &stringToInt(std::wstring);

        void setWindowProperties();
        void centerWindow();
        void setUnicode(bool);
    #else
        const std::string &intToStringConv(int const&);
        const std::string &stringConvert(std::wstring const&);
        const int &stringToInt(std::string);
    #endif

    void resizeConsole(int const&, int const&);
    void setWindowTitle(std::string const&);
    void clearScreen();
    void gotoXY(int const&, int const&);
    const CoordsStruct &intToCoords(int const&, int const&);
    const int &coordsToInt(CoordsStruct const&, int const&);
    const CoordsStruct &coordsToCursorPosition(CoordsStruct const&, int const&, int const&, int const&);
    void preciseSleep(double);
    const TimeStruct &secondsToTimeStruct(int);
};
