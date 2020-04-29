#pragma once
#include "common.hpp"
#include <vector>

class Field
{
private:
    int cols;
    int rows;
    int bombsCount;
    int bombsLeft = 0;
    int countEmpty = 0;
    std::string difficultyString;
    char** fieldArray;
    char** bombsArray;
public:
    Field(int cols = 0, int rows = 0, int bombsCount = 0, std::string difficultyString = "");
    ~Field();
    
    char** createArray();
    void clearArray(char**);
    void fillBombsArray();
    void drawField(char**);
    void printBombsLeft();
    void printExplanation();
    void printAll();
    bool isFlagOn(Common::coordsStruct);
    bool isFree(Common::coordsStruct);
    Common::placeUserInput placeUserInput(Common::userInputStruct, int);
    void hasWon();
    void hasLost();
    std::vector<Common::coordsStruct> findNeighbours(char **tempArray, Common::coordsStruct, char);
    int uncoverRecursive(Common::coordsStruct, int, int);
};
