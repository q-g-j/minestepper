#pragma once
#include <vector>

class Field
{
private:
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
    stringsym** fieldArray;
    stringsym** minesArray;
public:
    Field(int const& cols_, int const& rows_, int const& fieldOffsetX_, int const& fieldOffsetY_, int const& fieldCellWidth_, int const& minesCount_, std::string const& difficultyString_);
    
    ~Field();    
    
    // getter methods:
    int getCols();
    int getRows();
    int getOffsetX();
    int getOffsetY();
    int getMinesLeft();
    stringsym getCoordsContent(Common::coordsStruct&);
    
    stringsym** createArray();
    void clearFieldArray();
    void clearMinesArray();
    void fillMinesArray(coordsStruct&);
    void drawField(stringsym**);
    void gotoXY(int const&, int const&);
    void printCoords(coordsStruct& coords);
    void printExplanation();
    void printHasWon();
    void printHasLost();
    void printAll();
    bool isFlagSet(Common::coordsStruct&);
    bool isNumber(Common::coordsStruct&);
    Common::placeUserInputReturnStruct placeUserInput(Common::userInputReturnStruct&, int&);
    std::vector<Common::coordsStruct> findNeighbours(stringsym **tempArray, Common::coordsStruct const&, stringsym const&);
};
