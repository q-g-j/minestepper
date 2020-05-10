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

// color codes:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    enum ColorCode {        
        DEFAULT             = 7,
        FG_WHITE            = 15,
        FG_RED              = 4,
        FG_GREEN            = 2,
        FG_YELLOW           = 14,
        FG_BLUE             = 1,
        FG_MAGENTA          = 5,
        FG_CYAN             = 3,
        FG_BROWN            = 6,
        FG_LIGHT_RED        = 12,
        FG_LIGHT_GREEN      = 10,
        FG_LIGHT_BLUE       = 9,
        FG_LIGHT_MAGENTA    = 13,
        FG_LIGHT_CYAN       = 11,

        // grey background with different colors:
        BG_BLACK            = 112,
        BG_DARK_BLUE        = 113,
        BG_GREEN            = 114,
        BG_LIGHT_BLUE       = 115,
        BG_RED              = 116,
        BG_DARKEST_BLUE     = 117,
        BG_WHITE            = 118,
        BG_GREY             = 120,
        BG_BLUE             = 121,
        BG_LIGHT_GREEN      = 122,
        BG_CYAN             = 123,
        BG_LIGHT_RED        = 124,
        BG_MAGENTA          = 125,
        BG_YELLOW           = 126,
        BG_LIGHT_GREY       = 127
    };
#else
    enum ColorCode {        
        DEFAULT             = 39,
        FG_BLACK            = 30,
        FG_WHITE            = 37,
        FG_RED              = 31,
        FG_GREEN            = 32,
        FG_YELLOW           = 33,
        FG_BLUE             = 34,
        FG_MAGENTA          = 35,
        FG_CYAN             = 36,        
        FG_LIGHT_RED        = 91,
        FG_LIGHT_GREEN      = 92,
        FG_LIGHT_YELLOW     = 93,
        FG_LIGHT_BLUE       = 94,
        FG_LIGHT_MAGENTA    = 95,
        FG_LIGHT_CYAN       = 96
    };
#endif

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    const int color_default         = DEFAULT;
    const int fg_white              = FG_WHITE;
    const int fg_red                = FG_RED;
    const int fg_green              = FG_GREEN;
    const int fg_yellow             = FG_YELLOW;
    const int fg_blue               = FG_BLUE;
    const int fg_magenta            = FG_MAGENTA;
    const int fg_cyan               = FG_CYAN;
    const int fg_brown              = FG_BROWN;
    const int fg_light_red          = FG_LIGHT_RED;
    const int fg_light_green        = FG_LIGHT_GREEN;
    const int fg_light_blue         = FG_LIGHT_BLUE;
    const int fg_light_magenta      = FG_LIGHT_MAGENTA;
    const int fg_light_cyan         = FG_LIGHT_CYAN;

    const int bg_black              = BG_BLACK;
    const int bg_dark_blue          = BG_DARK_BLUE;
    const int bg_green              = BG_GREEN;
    const int bg_light_blue         = BG_LIGHT_BLUE;
    const int bg_red                = BG_RED;
    const int bg_darkest_blue       = BG_DARKEST_BLUE;
    const int bg_white              = BG_WHITE;
    const int bg_grey               = BG_GREY;
    const int bg_blue               = BG_BLUE;
    const int bg_light_green        = BG_LIGHT_GREEN;
    const int bg_cyan               = BG_CYAN;
    const int bg_light_red          = BG_LIGHT_RED;
    const int bg_magenta            = BG_MAGENTA;
    const int bg_yellow             = BG_YELLOW;
    const int bg_light_grey         = BG_LIGHT_GREY;
#else
    const std::string color_default         = "\033[" + std::to_string(DEFAULT) + ";0m";
    const std::string fg_white              = "\033[" + std::to_string(FG_WHITE) + ";1m";
    const std::string fg_red                = "\033[" + std::to_string(FG_RED) + "m";
    const std::string fg_green              = "\033[" + std::to_string(FG_GREEN) + "m";
    const std::string fg_yellow             = "\033[" + std::to_string(FG_YELLOW) + "m";
    const std::string fg_blue               = "\033[" + std::to_string(FG_BLUE) + "m";
    const std::string fg_magenta            = "\033[" + std::to_string(FG_MAGENTA) + "m";
    const std::string fg_cyan               = "\033[" + std::to_string(FG_CYAN) + "m";
    const std::string fg_light_red          = "\033[" + std::to_string(FG_LIGHT_RED) + "m";
    const std::string fg_light_green        = "\033[" + std::to_string(FG_LIGHT_GREEN) + "m";
    const std::string fg_light_yellow       = "\033[" + std::to_string(FG_LIGHT_YELLOW) + "m";
    const std::string fg_light_blue         = "\033[" + std::to_string(FG_LIGHT_BLUE) + "m";
    const std::string fg_light_magenta      = "\033[" + std::to_string(FG_LIGHT_MAGENTA) + "m";
    const std::string fg_light_cyan         = "\033[" + std::to_string(FG_LIGHT_CYAN) + "m";

    const std::string bg_red                = "\033[47;" + std::to_string(FG_RED) + "m";
    const std::string bg_black              = "\033[47;" + std::to_string(FG_BLACK) + "m";
    const std::string bg_green              = "\033[47;" + std::to_string(FG_GREEN) + "m";
    const std::string bg_yellow             = "\033[47;" + std::to_string(FG_YELLOW) + "m";
    const std::string bg_blue               = "\033[47;" + std::to_string(FG_BLUE) + "m";
    const std::string bg_magenta            = "\033[47;" + std::to_string(FG_MAGENTA) + "m";
    const std::string bg_cyan               = "\033[47;" + std::to_string(FG_CYAN) + "m";
    const std::string bg_light_red          = "\033[47;" + std::to_string(FG_LIGHT_RED) + "m";
    const std::string bg_light_green        = "\033[47;" + std::to_string(FG_LIGHT_GREEN) + "m";
    const std::string bg_light_yellow       = "\033[47;" + std::to_string(FG_LIGHT_YELLOW) + "m";
    const std::string bg_light_blue         = "\033[47;" + std::to_string(FG_LIGHT_BLUE) + "m";
    const std::string bg_light_magenta      = "\033[47;" + std::to_string(FG_LIGHT_MAGENTA) + "m";
    const std::string bg_light_cyan         = "\033[47;" + std::to_string(FG_LIGHT_CYAN) + "m";
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
    coordsStruct convCoordsToCursorPosition(coordsStruct&, int const&, int const&, int const&);
};

typedef struct Common::coordsStruct coordsStruct;
typedef struct Common::userInputReturnStruct userInputReturnStruct;
typedef struct Common::placeUserInputReturnStruct placeUserInputReturnStruct;
