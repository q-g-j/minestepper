#pragma once
#include <vector>

class Field
{
private:
    int cols;
    int rows;
    int fieldOffsetX;
    int fieldOffsetY;
    int fieldCellWidth;
    int minesCount;
    int minesLeft = 0;
    int countEmpty = 0;
    std::string difficultyString;
    stringsym** fieldArray;
    stringsym** minesArray;
public:
    Field(int cols = 0, int rows = 0, int fieldOffsetX = 0, int FieldOffsetY = 0, int fieldCellWidth=0, int minesCount = 0, std::string difficultyString = "");
    ~Field();
    
    // field frames:
    stringsym cornerTopLeftSymbol;
    stringsym cornerTopRightSymbol;
    stringsym cornerBottomLeftSymbol;
    stringsym cornerBottomRightSymbol;
    stringsym horizontalLineSymbol;
    stringsym verticalLineSymbol;
    stringsym downTSymbol;
    stringsym upTSymbol;
    stringsym rightTSymbol;
    stringsym leftTSymbol;
    stringsym plusSymbol;
    
    stringsym flagSymbol;
    stringsym bombSymbol;
    stringsym bombHitSymbol;
    stringsym uncoveredSymbol;
    stringsym coveredSymbol;
    
    enum colors
    {
        
    };
    
    void setFrameSymbols();
    void setCellSymbols();
    int getCols();
    int getRows();
    int getOffsetX();
    int getOffsetY();
    int getMinesLeft();
    stringsym** createArray();
    void clearFieldArray();
    void clearMinesArray();
    void fillMinesArray(coordsStruct);
    void drawField(stringsym**);
    void gotoXY(int, int);
    void printCoords(coordsStruct coords);
    void printExplanation();
    void printHasWon();
    void printHasLost();
    void printAll();
    bool isFlagSet(Common::coordsStruct);
    bool isNumber(Common::coordsStruct);
    Common::placeUserInputReturnStruct placeUserInput(Common::userInputReturnStruct, int);
    std::vector<Common::coordsStruct> findNeighbours(stringsym **tempArray, Common::coordsStruct, stringsym);
};
