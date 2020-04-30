#pragma once
#include "common.hpp"
#include <vector>

class Field
{
private:
    int cols;
    int rows;
    int minesCount;
    int minesLeft = 0;
    int countEmpty = 0;
    std::string difficultyString;
    char** fieldArray;
    char** minesArray;
public:
    Field(int cols = 0, int rows = 0, int minesCount = 0, std::string difficultyString = "");
    ~Field();
    
    char** createArray();
    void clearArray(char**);
    void fillMinesArray();
    void drawField(char**);
    void printMinesLeft();
    void printExplanation();
    void printAll();
    bool isFlagOn(Common::coordsStruct);
    bool isNumber(Common::coordsStruct);
    Common::placeUserInputStruct placeUserInput(Common::userInputStruct, int);
    void hasWon();
    void hasLost();
    std::vector<Common::coordsStruct> findNeighbours(char **tempArray, Common::coordsStruct, char);
};
