#pragma once
#include <vector>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #define coutsym std::wcout
    #define stringsym std::wstring
#else
    #define coutsym std::cout
    #define stringsym std::string
#endif

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
    char** fieldArray;
    char** minesArray;
public:
    Field(int cols = 0, int rows = 0, int fieldOffsetX = 0, int FieldOffsetY = 0, int fieldCellWidth=0, int minesCount = 0, std::string difficultyString = "");
    ~Field();
    
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
    
    enum colors
    {
        
    };
            
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
