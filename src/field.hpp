#pragma once
#include <vector>

class Field
{
private:
    
    // color codes:
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        enum Code {        
            DEFAULT          = 7,
            FG_WHITE            = 2,
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
        enum Code {        
            DEFAULT             = 39,
            FG_BLACK            = 30,
            FG_WHITE            = 97,
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

    // private constants:
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
        const std::string color_default         = "\033[0;" + std::to_string(DEFAULT) + "m";
        const std::string fg_white              = "\033[" + std::to_string(FG_WHITE) + "m";
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

    // private variables:
    int cols;
    int rows;
    int fieldOffsetX;
    int fieldOffsetY;
    int fieldCellWidth;
    int minesCount;
    int minesLeft;
    int countCovered;
    int flagsCount;
    std::string difficultyString;
    stringconv** fieldArray;
    stringconv** minesArray;
    stringconv** cursorArray;
    stringconv** createArray();
    
    // private methods
    void clearFieldArray();
    void clearMinesArray();
    void fillMinesArray(coordsStruct&);
    std::vector<Common::coordsStruct> findNeighbours(stringconv **tempArray, Common::coordsStruct const&, stringconv const&);
    
public:
    Field(int const& cols_, int const& rows_, int const& fieldOffsetX_, int const& fieldOffsetY_, int const& fieldCellWidth_, int const& minesCount_, std::string const& difficultyString_);
    ~Field();

    // define frame and cell symbol constants:
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
        const stringconv symbolMine = fg_red + "\u263c" + color_default;
        const stringconv symbolMineHit = fg_red + "#" + color_default;
        const stringconv symbolUncovered = " ";
        const stringconv symbolCovered = "\u2591";
        const stringconv symbolCursor = "\u2588";
    #endif

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

    // getter methods:
    int getCols();
    int getRows();
    int getOffsetX();
    int getOffsetY();
    int getMinesCount();
    int getMinesLeft();
    int getCellWidth();
    std::string getDifficultyString();
    stringconv getCoordsContent(Common::coordsStruct const&);

    // public methods:
    void drawField();
    void gotoXY(int const&, int const&);
    void printCoords(coordsStruct&, bool);
    bool isFlagSet(Common::coordsStruct&);
    bool isNumber(Common::coordsStruct&);
    Common::placeUserInputReturnStruct placeUserInput(Common::userInputReturnStruct&, int&);
};

typedef struct Field::DifficultyStruct DifficultyStruct;
