#pragma once

// Windows and Linux seem to handle unicode strings differently (use wide strings only for Windows):
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #define coutsym std::wcout
    #define stringsym std::wstring
#else
    #define coutsym std::cout
    #define stringsym std::string
#endif

// define frame and cell symbols:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    const std::wstring cornerTopLeftSymbol = L"\u250C";
    const std::wstring cornerTopRightSymbol = L"\u2510";
    const std::wstring cornerBottomLeftSymbol = L"\u2514";
    const std::wstring cornerBottomRightSymbol = L"\u2518";
    const std::wstring horizontalLineSymbol = L"\u2500";
    const std::wstring verticalLineSymbol = L"\u2502";
    const std::wstring downTSymbol = L"\u252C";
    const std::wstring upTSymbol = L"\u2534";
    const std::wstring rightTSymbol = L"\u251C";
    const std::wstring leftTSymbol = L"\u2524";
    const std::wstring plusSymbol = L"\u253C";
    
    const std::wstring flagSymbol = L"\u25ba";
    const std::wstring bombSymbol = L"\u263c";
    const std::wstring bombHitSymbol = L"#";
    const std::wstring uncoveredSymbol = L" ";
    const std::wstring coveredSymbol = L"\u2591";
#else
    const std::string cornerTopLeftSymbol = "\u250C";
    const std::string cornerTopRightSymbol = "\u2510";
    const std::string cornerBottomLeftSymbol = "\u2514";
    const std::string cornerBottomRightSymbol = "\u2518";
    const std::string horizontalLineSymbol = "\u2500";
    const std::string verticalLineSymbol = "\u2502";
    const std::string downTSymbol = "\u252C";
    const std::string upTSymbol = "\u2534";
    const std::string rightTSymbol = "\u251C";
    const std::string leftTSymbol = "\u2524";
    const std::string plusSymbol = "\u253C";
    
    const std::string flagSymbol = "\u25ba";
    const std::string bombSymbol = "\u263c";
    const std::string bombHitSymbol = "#";
    const std::string uncoveredSymbol = " ";
    const std::string coveredSymbol = "\u2591";
#endif

const char nl = '\n';
    
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
    std::wstring intToString(int const&);
    std::wstring stringConvert(std::string const&);
    #else
    std::string intToString(int const&);
    std::string stringConvert(std::wstring const&);
    #endif
    
    void setUnicode(bool);
    void setRandomSeed();
    void clearScreen();
    coordsStruct intToStruct(int&, int&);
    int structToInt(coordsStruct&, int&);
};

typedef struct Common::coordsStruct coordsStruct;
typedef struct Common::userInputReturnStruct userInputReturnStruct;
typedef struct Common::placeUserInputReturnStruct placeUserInputReturnStruct;

