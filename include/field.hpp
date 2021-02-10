#pragma once

#include <string>
#include <vector>

#include "../include/common.hpp"
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
    int coveredLeft;
    int minesTotal;
    int minesLeft;
    int flagsCount;
    std::string difficultyString;

    // private methods
    std::vector<std::vector<stringconv>> create2DVector(std::string const&);
    void fillMines(Common::CoordsStruct const&);
    void gameWon();
    void gameLost();

    #if DEBUG == 1
        void debugPrintCountCovered(Common::CoordsStruct const&);
    #endif

public:
    Field(int const& cols_, int const& rows_, int const& fieldOffsetX_, int const& fieldOffsetY_, int const& fieldCellWidth_, int const& minesTotal_, std::string const& difficultyString_);
    ~Field();

    std::vector<std::vector<stringconv>> field2DVector;
    std::vector<std::vector<stringconv>> mines2DVector;
    std::vector<std::vector<stringconv>> cursor2DVector;

    // getter methods:
    int getCols();
    int getRows();
    int getOffsetX();
    int getOffsetY();
    int getCoveredLeft();
    int getMinesTotal();
    int getMinesLeft();
    int getFlagsCount();
    int getCellWidth();
    std::string getDifficultyString();
    stringconv getCoordsContent(Common::CoordsStruct const&);

    // setter methods:
    void setCoveredLeft(int const&);
    void setFlagsCount(int const&);
    void setMinesLeft(int const&);

    // public methods:
    bool isFlag(Common::CoordsStruct const&);
    bool isNumber(Common::CoordsStruct const&);
    void drawField(bool);
    void gotoXY(int const&, int const&);
    void printCoords(Common::CoordsStruct const&, bool);
    std::vector<Common::CoordsStruct> findNeighbors(std::vector<std::vector<stringconv>> const&, Common::CoordsStruct const&, stringconv const&);
    void autoUncoverRecursive(Common::CoordsStruct const&, std::vector<unsigned int>&);
    void flagAutoUncover(Common::UserInputReturnStruct const&, Common::PlaceUserInputReturnStruct&);
    Common::PlaceUserInputReturnStruct placeUserInput(Common::UserInputReturnStruct&, int&);
};
