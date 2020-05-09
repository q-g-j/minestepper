#pragma once
#include <string>

// Windows and Linux seem to handle unicode strings differently (use wide strings only for Windows):
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
    struct coordsStruct
    {
        int col = 0;
        int row = 0;
    };
    
    struct userInputReturnStruct
    {
        coordsStruct Coords;
        bool isFlag = false;
    };
    struct placeUserInputReturnStruct
    {
        bool hasLost = false;
        bool hasWon = false;
        bool isTurn = false;
    };
    
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    stringconv intToString(int const&);
    stringconv stringConvert(std::string const&);
    #else
    std::string intToString(int const&);
    std::string stringConvert(std::wstring const&);
    #endif
    
    void setUnicode(bool);
    void setRandomSeed();
    void clearScreen();
    coordsStruct intToStruct(int&, int&);
    int structToInt(coordsStruct&, int&);
    coordsStruct convCursorPositionToCoords(coordsStruct&, int const&, int const&, int const&);
    coordsStruct convCoordsToCursorPosition(coordsStruct&, int const&, int const&, int const&);
};

typedef struct Common::coordsStruct coordsStruct;
typedef struct Common::userInputReturnStruct userInputReturnStruct;
typedef struct Common::placeUserInputReturnStruct placeUserInputReturnStruct;
