#pragma once

// Windows and Linux seem to handle unicode strings differently (use wide strings only for Windows):
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #define coutconv std::wcout
    #define stringconv std::wstring
#else
    #define coutconv std::cout
    #define stringconv std::string
#endif

// define frame and cell symbols:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    const stringconv symbolCornerTopLeft = L"\u250C";
    const stringconv symbolCornerTopRight = L"\u2510";
    const stringconv symbolCornerBottomLeft = L"\u2514";
    const stringconv symbolCornerBottomRight = L"\u2518";
    const stringconv symbolHorizontalLine = L"\u2500";
    const stringconv symbolVerticalLine = L"\u2502";
    const stringconv symbolDownT = L"\u252C";
    const stringconv symbolUpT = L"\u2534";
    const stringconv symbolRightT = L"\u251C";
    const stringconv symbolLeftT = L"\u2524";
    const stringconv symbolPlus = L"\u253C";
    
    const stringconv symbolFlag = L"\u25ba";
    const stringconv symbolMine = L"\u263c";
    const stringconv symbolMineHit = L"#";
    const stringconv symbolUncovered = L" ";
    const stringconv symbolCovered = L"\u2591";
    const stringconv symbolCursor = L"\u2588";
#else
    const stringconv symbolCornerTopLeft = "\u250C";
    const stringconv symbolCornerTopRight = "\u2510";
    const stringconv symbolCornerBottomLeft = "\u2514";
    const stringconv symbolCornerBottomRight = "\u2518";
    const stringconv symbolHorizontalLine = "\u2500";
    const stringconv symbolVerticalLine = "\u2502";
    const stringconv symbolDownT = "\u252C";
    const stringconv symbolUpT = "\u2534";
    const stringconv symbolRightT = "\u251C";
    const stringconv symbolLeftT = "\u2524";
    const stringconv symbolPlus = "\u253C";
    
    const stringconv symbolFlag = "\u25ba";
    const stringconv symbolMine = "\u263c";
    const stringconv symbolMineHit = "#";
    const stringconv symbolUncovered = " ";
    const stringconv symbolCovered = "\u2591";
    const stringconv symbolCursor = "\u2588";
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
        coordsStruct coords;
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

