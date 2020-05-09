#pragma once
#include <vector>

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

class Field
{
private:
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