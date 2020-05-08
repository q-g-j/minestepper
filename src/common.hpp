#pragma once

// Windows and Linux seem to handle unicode strings differently (use wide strings only for Windows):
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #define coutconv std::wcout
    #define stringconv std::wstring
#else
    #define coutconv std::cout
    #define stringconv std::string
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
    
    struct UserInputReturnStruct
    {
        CoordsStruct coords;
        bool isFlag = false;
    };
    struct PlaceUserInputReturnStruct
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
    CoordsStruct intToStruct(int&, int&);
    int structToInt(CoordsStruct&, int&);
    CoordsStruct convCursorPositionToCoords(CoordsStruct&, int const&, int const&, int const&);
    CoordsStruct convCoordsToCursorPosition(CoordsStruct&, int const&, int const&, int const&);
};

typedef struct Common::CoordsStruct CoordsStruct;
typedef struct Common::UserInputReturnStruct UserInputReturnStruct;
typedef struct Common::PlaceUserInputReturnStruct PlaceUserInputReturnStruct;

