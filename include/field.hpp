#pragma once

#include <memory>
#include <random>
#include <string>
#include <vector>

#include "../include/common.hpp"
#include "../include/debug.hpp"

// forward declarations of classes:
class Colors;
class Common;
class Input;
class Print;
class Symbols;

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

    // declare class objects as unique pointers:
    std::unique_ptr<Colors> colors;
    std::unique_ptr<Common> common;
    std::unique_ptr<Input> input;
    std::unique_ptr<Print> print;
    std::unique_ptr<Symbols> symbols;

    // random number generator:
    std::random_device rng;
    std::mt19937 urng;

    // private methods:
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

    // setter methods to increment / decrement using operator overloading
    // (just for learning purposes):
    struct setFlagsCount
    {
        Field &field_;
        setFlagsCount(Field &field) : field_(field) { }
        void operator--();
        void operator++();
    };
    struct setMinesLeft
    {
        Field &field_;
        setMinesLeft(Field &field) : field_(field) { }
        void operator--();
        void operator++();
    };
    struct setCoveredLeft
    {
        Field &field_;
        setCoveredLeft(Field &field) : field_(field) { }
        void operator--();
        void operator++();
    };

    // public methods:
    bool isFlag(Common::CoordsStruct const&);
    bool isNumber(Common::CoordsStruct const&);
    void drawField();
    void gotoXY(int const&, int const&);
    void printCoords(Common::CoordsStruct const&, bool);
    std::vector<Common::CoordsStruct> findNeighbors(std::vector<std::vector<stringconv>> const&, Common::CoordsStruct const&, stringconv const&, bool const&);
    // std::vector<Common::CoordsStruct> findNeighborNumbers(std::vector<std::vector<stringconv>> const&, Common::CoordsStruct const&, stringconv const&);
    void autoUncoverRecursive(Common::CoordsStruct const&, std::vector<unsigned int>&);
    void flagAutoUncover(Common::CoordsStruct const&, Common::PlaceUserInputReturnStruct&);
    Common::PlaceUserInputReturnStruct placeUserInput(Common::UserInputReturnStruct&, int&);
};
