#pragma once

// system headers:
#include <memory>
#include <random>

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
    int minesTotal;
    int minesLeft;
    int coveredLeft;
    int flagsCount;
    std::string difficultyString;

    // random number generator:
    std::random_device rng;
    std::mt19937 urng;

    // declare class objects as unique pointers:
    std::unique_ptr<Colors> colors;
    std::unique_ptr<Common> common;
    std::unique_ptr<Input> input;
    std::unique_ptr<Print> print;
    std::unique_ptr<Symbols> symbols;

    // private methods:
    std::vector<std::vector<stringconv>> create2DVector(std::string const&);
    void fillMines(Common::CoordsStruct const&);
    void gameWon();
    void gameLost();

public:
    Field(Common::GameModeReturnStruct&);
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
        private:
            Field &field_;
        public:
            setFlagsCount(Field& field) : field_(field) { }
            void operator--();
            void operator++();
    };
    struct setMinesLeft
    {
        private:
            Field& field_;
        public:
            setMinesLeft(Field& field) : field_(field) { }
            void operator--();
            void operator++();
    };
    struct setCoveredLeft
    {
        private:
            Field& field_;
        public:
            setCoveredLeft(Field& field) : field_(field) { }
            void operator--();
            void operator++();
    };

    // public methods:
    bool isFlag(Common::CoordsStruct const&);
    bool isNumber(Common::CoordsStruct const&);
    void drawField();
    void printAll();
    void printCoords(Common::CoordsStruct const&, bool);
    std::vector<Common::CoordsStruct> findNeighbors(std::vector<std::vector<stringconv>> const&, Common::CoordsStruct const&, stringconv const&);
    void autoUncoverRecursive(Common::CoordsStruct const&, std::vector<unsigned int>&);
    void flagAutoUncover(Common::CoordsStruct const&, Common::PlaceUserInputReturnStruct&, bool);
    Common::PlaceUserInputReturnStruct placeUserInput(Common::UserInputReturnStruct&, int&);
};
