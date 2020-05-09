#pragma once
#include <vector>

class Field
{
private:
    
    // color codes:
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        enum Code {        
            FG_DEFAULT          = 7,
            FG_RED              = 4,
            FG_GREEN            = 2,
            FG_YELLOW           = 14,
            FG_BLUE             = 1,
            FG_MAGENTA          = 5,
            FG_CYAN             = 3,
            FG_BROWN            = 6,
            FG_BRIGHT_RED       = 12,
            FG_BRIGHT_GREEN     = 10,
            FG_BRIGHT_BLUE      = 9,
            FG_BRIGHT_MAGENTA   = 13,
            FG_BRIGHT_CYAN      = 11,

            BG_DEFAULT          = 49,
            BG_RED              = 41,
            BG_GREEN            = 42,
            BG_BLUE             = 44
        };
    #else
        enum Code {        
            FG_DEFAULT          = 39,
            FG_RED              = 31,
            FG_GREEN            = 32,
            FG_YELLOW           = 33,
            FG_BLUE             = 34,
            FG_MAGENTA          = 35,
            FG_CYAN             = 36,        
            FG_BRIGHT_RED       = 91,
            FG_BRIGHT_GREEN     = 92,
            FG_BRIGHT_YELLOW    = 93,
            FG_BRIGHT_BLUE      = 94,
            FG_BRIGHT_MAGENTA   = 95,
            FG_BRIGHT_CYAN      = 96,

            BG_DEFAULT          = 49,
            BG_RED              = 41,
            BG_GREEN            = 42,
            BG_BLUE             = 44
        };
    #endif

    // private constants:
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        const int fg_reset              = FG_DEFAULT;
        const int fg_red                = FG_RED;
        const int fg_green              = FG_GREEN;
        const int fg_yellow             = FG_YELLOW;
        const int fg_blue               = FG_BLUE;
        const int fg_magenta            = FG_MAGENTA;
        const int fg_cyan               = FG_CYAN;
        const int fg_brown              = FG_BROWN;
        const int fg_bright_red         = FG_BRIGHT_RED;
        const int fg_bright_green       = FG_BRIGHT_GREEN;
        const int fg_bright_blue        = FG_BRIGHT_BLUE;
        const int fg_bright_magenta     = FG_BRIGHT_MAGENTA;
        const int fg_bright_cyan        = FG_BRIGHT_CYAN;
    #else
        const std::string fg_reset              = "\033[" + std::to_string(FG_DEFAULT) + "m";
        const std::string fg_red                = "\033[" + std::to_string(FG_RED) + "m";
        const std::string fg_green              = "\033[" + std::to_string(FG_GREEN) + "m";
        const std::string fg_yellow             = "\033[" + std::to_string(FG_YELLOW) + "m";
        const std::string fg_blue               = "\033[" + std::to_string(FG_BLUE) + "m";
        const std::string fg_magenta            = "\033[" + std::to_string(FG_MAGENTA) + "m";
        const std::string fg_cyan               = "\033[" + std::to_string(FG_CYAN) + "m";
        const std::string fg_bright_red         = "\033[" + std::to_string(FG_BRIGHT_RED) + "m";
        const std::string fg_bright_green       = "\033[" + std::to_string(FG_BRIGHT_GREEN) + "m";
        const std::string fg_bright_yellow      = "\033[" + std::to_string(FG_BRIGHT_YELLOW) + "m";
        const std::string fg_bright_blue        = "\033[" + std::to_string(FG_BRIGHT_BLUE) + "m";
        const std::string fg_bright_magenta     = "\033[" + std::to_string(FG_BRIGHT_MAGENTA) + "m";
        const std::string fg_bright_cyan        = "\033[" + std::to_string(FG_BRIGHT_CYAN) + "m";
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
        const stringconv symbolMine = fg_red + "\u263c" + fg_reset;
        const stringconv symbolMineHit = fg_red + "#" + fg_reset;
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
    void printCoords(coordsStruct&);
    bool isFlagSet(Common::coordsStruct&);
    bool isNumber(Common::coordsStruct&);
    Common::placeUserInputReturnStruct placeUserInput(Common::userInputReturnStruct&, int&);
};

typedef struct Field::DifficultyStruct DifficultyStruct;
