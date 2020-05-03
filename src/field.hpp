#pragma once
#include <vector>

class Field
{
private:
    int cols;
    int rows;
    int offsetX;
    int offsetY;
    int minesCount;
    int minesLeft = 0;
    int countEmpty = 0;
    std::string difficultyString;
    char** fieldArray;
    char** minesArray;
public:
    Field(int cols = 0, int rows = 0, int offsetX = 0, int offsetY = 0,int  minesCount = 0, std::string difficultyString = "");
    ~Field();
    
    int getCols();
    int getRows();
    int getOffsetX();
    int getOffsetY();
    int getMinesLeft();
    char** createArray();
    void clearFieldArray();
    void clearMinesArray();
    void fillMinesArray(coordsStruct);
    void drawField(char**);
    void gotoXY(int, int);
    void printCoords(coordsStruct coords);
    void printExplanation();
    void printHasWon();
    void printHasLost();
    void printAll();
    bool isFlagSet(Common::coordsStruct);
    bool isNumber(Common::coordsStruct);
    Common::placeUserInputReturnStruct placeUserInput(Common::userInputReturnStruct, int);
    std::vector<Common::coordsStruct> findNeighbours(char **tempArray, Common::coordsStruct, char);
};
