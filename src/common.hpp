#pragma once

const char nl = '\n';

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #define coutsym std::wcout
    #define stringsym std::wstring
#else
    #define coutsym std::cout
    #define stringsym std::string
#endif

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
    std::wstring intToString(int);
    std::wstring stringConvert(std::string);
    #else
    std::string intToString(int);
    std::string stringConvert(std::wstring);
    #endif
    void setRandomSeed();
    void clearScreen();
    coordsStruct intToStruct(int, int);
    int structToInt(coordsStruct, int);
};

typedef struct Common::coordsStruct coordsStruct;
typedef struct Common::userInputReturnStruct userInputReturnStruct;
typedef struct Common::placeUserInputReturnStruct placeUserInputReturnStruct;

