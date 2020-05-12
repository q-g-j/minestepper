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
    void fillMinesArray(CoordsStruct&);
    std::vector<CoordsStruct> findNeighbours(stringconv **tempArray, CoordsStruct const&, stringconv const&);
    
public:
    Field(int const& cols_, int const& rows_, int const& fieldOffsetX_, int const& fieldOffsetY_, int const& fieldCellWidth_, int const& minesCount_, std::string const& difficultyString_);
    ~Field();

    // getter methods:
    int getCols();
    int getRows();
    int getOffsetX();
    int getOffsetY();
    int getMinesCount();
    int getMinesLeft();
    int getCellWidth();
    std::string getDifficultyString();
    stringconv getCoordsContent(Common::CoordsStruct const&);

    // public methods:
    void drawField();
    void gotoXY(int const&, int const&);
    void printCoords(CoordsStruct&, bool);
    bool isFlagSet(Common::CoordsStruct&);
    bool isNumber(Common::CoordsStruct&);
    Common::PlaceUserInputReturnStruct placeUserInput(Common::UserInputReturnStruct&, int&);
};
