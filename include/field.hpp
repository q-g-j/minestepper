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
    std::vector<std::vector<stringconv>> field2DVector;
    std::vector<std::vector<stringconv>> mines2DVector;
    std::vector<std::vector<stringconv>> cursor2DVector;

    // private methods
    void clearField();
    void clearMines();
    std::vector<std::vector<stringconv>> create2DVector(std::string const&);
    void fillMines(Common::CoordsStruct&);
    std::vector<Common::CoordsStruct> findNeighbours(std::vector<std::vector<stringconv>> const&, Common::CoordsStruct const&, stringconv const&);
    void autoUncoverRecursive(Common::CoordsStruct const&, std::vector<unsigned int>&);
    Common::PlaceUserInputReturnStruct gameWon();
    Common::PlaceUserInputReturnStruct gameLost();

public:
    Field(int const& cols_, int const& rows_, int const& fieldOffsetX_, int const& fieldOffsetY_, int const& fieldCellWidth_, int const& minesCount_, std::string const& difficultyString_);
    ~Field();

    // getter methods:
    int getCols();
    int getRows();
    int getOffsetX();
    int getOffsetY();
    int getCovered();
    int getMinesCount();
    int getMinesLeft();
    int getCellWidth();
    std::string getDifficultyString();
    stringconv getCoordsContent(Common::CoordsStruct const&);

    // public methods:
    void drawField();
    void gotoXY(int const&, int const&);
    void printCoords(Common::CoordsStruct&, bool);
    bool isFlag(Common::CoordsStruct&);
    bool isNumber(Common::CoordsStruct&);
    Common::PlaceUserInputReturnStruct placeUserInput(Common::UserInputReturnStruct&, int&);
};
