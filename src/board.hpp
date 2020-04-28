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
    char** bombsArray;
public:
    Board(int cols = 0, int rows = 0, int bombsCount = 0, std::string difficultyString = "");
    ~Board();
    
    char** createArray();
    void clearBoard(char**);
    void fillBombsArray();
    void drawBoard(char**);
    void printBombsLeft();
    void printExplanation();
    void printAll();
    Common::coordsStruct intToStruct(int);
    int structToInt(Common::coordsStruct);
    bool hasLost();
};
