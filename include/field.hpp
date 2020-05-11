#pragma once
#include <vector>

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
    std::vector<coordsStruct> findNeighbours(stringconv **tempArray, coordsStruct const&, stringconv const&);
    
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
    void printCoords(coordsStruct&, bool);
    bool isFlagSet(Common::coordsStruct&);
    bool isNumber(Common::coordsStruct&);
    Common::placeUserInputReturnStruct placeUserInput(Common::userInputReturnStruct&, int&);
};

typedef struct Field::DifficultyStruct DifficultyStruct;
