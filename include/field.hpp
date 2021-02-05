#pragma once
#include <vector>

#include "../include/debug.hpp"

class Field
{
private:
    // private variables:
    int cols;
    int rows;
    int fieldOffsetX;
    int fieldOffsetY;
    int fieldCellWidth;
    int minesTotal;
    int minesLeft;
    int countCovered;
    int flagsCount;
    std::string difficultyString;
    std::vector<std::vector<stringconv>> field2DVector;
    std::vector<std::vector<stringconv>> mines2DVector;
    std::vector<std::vector<stringconv>> cursor2DVector;

    // private methods
    std::vector<std::vector<stringconv>> create2DVector(std::string const&);
    void fillMines(Common::CoordsStruct&);
    std::vector<Common::CoordsStruct> findNeighbours(std::vector<std::vector<stringconv>> const&, Common::CoordsStruct const&, stringconv const&);
    void autoUncoverRecursive(Common::CoordsStruct const&, std::vector<unsigned int>&);
    void gameWon();
    void gameLost();

    #if DEBUG == 1
        void debugPrintCountCovered(Common::CoordsStruct const&);
    #endif

public:
    Field(int const& cols_, int const& rows_, int const& fieldOffsetX_, int const& fieldOffsetY_, int const& fieldCellWidth_, int const& minesTotal_, std::string const& difficultyString_);
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
    void drawField(bool);
    void gotoXY(int const&, int const&);
    void printCoords(Common::CoordsStruct const&, bool);
    bool isFlag(Common::CoordsStruct const&);
    bool isNumber(Common::CoordsStruct const&);
    Common::PlaceUserInputReturnStruct placeUserInput(Common::UserInputReturnStruct&, int&);
};
