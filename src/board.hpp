#pragma once
#include "common.hpp"

class Board
{
private:
    int cols;
    int rows;
    int bombsCount;
    int bombsLeft = 0;
    std::string difficultyString;
    char** boardArray;    
public:
    Board(int cols = 0, int rows = 0, int bombsCount = 0, std::string difficultyString = "");
    ~Board();
    
    char** createBoardArray();
    void clearBoard();
    void drawBoard();
    void printBombsLeft();
    void printExplanation();
    void printAll(Common &common);
    Common::coordsStruct intToStruct(int);
    int structToInt(Common::coordsStruct);
    void fillBoardWithBombs();
    bool hasLost();
};
