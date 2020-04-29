#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "common.hpp"
#include "debug.hpp"
#include "field.hpp"
#include "input.hpp"

Field::Field(int cols, int rows, int bombsCount, std::string difficultyString)
{
    this->cols = cols;
    this->rows = rows;
    this->bombsCount = bombsCount;
    this->bombsLeft = bombsCount;
    this->countEmpty = cols*rows;
    this->difficultyString = difficultyString;
    this->fieldArray = createArray();
    this->bombsArray = createArray();
    clearArray(this->fieldArray);
    clearArray(this->bombsArray);
}

Field::~Field()
{    
    for (int i=0; i <= cols; i++)
        delete this->fieldArray[i];
    delete[] this->fieldArray;
    
    for (int i=0; i <= cols; i++)
        delete this->bombsArray[i];
    delete[] this->bombsArray;
}

char** Field::createArray()
{
    char** array = new char*[this->cols+1];
    for (int i=0; i <= this->cols; i++)
        array[i] = new char[this->rows+1];
    return array;
}

void Field::clearArray(char** array)
{
    for (int i=0; i <= this->cols; i++)
    {
        for (int j=0; j <= this->rows; j++)
        {
            array[i][j] = ' ';
        }
    }
}

void Field::fillBombsArray()
{    
    Common common;
    Common::coordsStruct coords;
    int sizeOfFieldArray = this->cols * this->rows;
    int tempArray[sizeOfFieldArray+1];
    tempArray[0] = 0;
    for (int i = 1; i <= sizeOfFieldArray; i++)
        tempArray[i] = i;
    std::random_shuffle(&tempArray[1], &tempArray[sizeOfFieldArray+1]);
    for (int i = 1; i <= this->bombsCount; i++)
    {
        coords = common.intToStruct(tempArray[i], this->cols);
        this->bombsArray[coords.col][coords.row] = 'X';
    }
}

void Field::drawField(char** array)
{
    std::cout << "    ";
    for (int colNum = 1; colNum <= this->cols; colNum++)
    {
        if (colNum < 10)
            std::cout << "  " << colNum << " ";
        else
            std::cout << " " << colNum << " ";
    }
    std::cout << nl;
    std::cout << "   ";
    for (int bar = 1; bar <= this->cols; bar++)
    {
        std::cout << " " << "---";
    }
    std::cout << nl;

    for (int rowNum = 1; rowNum <= this->rows; rowNum++)
    {
        for (int colNum = 1; colNum <= this->cols; colNum++)
        {
            if (colNum == 1)
            {
                if (rowNum < 10)
                    std::cout << " " << rowNum << " ";
                else
                    std::cout << rowNum << " ";
            }
            std::cout << "|" << " " << array[colNum][rowNum] << " ";
            if (colNum == this->cols)
            {
                std::cout << "|" << nl;
                std::cout << "   ";
            }
        }
        for (int bar = 1; bar <= this->cols; bar++)
        {
            std::cout << " " << "---";
        }
        std::cout << nl;
    }
}

void Field::printBombsLeft()
{
    std::cout << "There are " << this->bombsLeft << " bombs left." << nl;
}

void Field::printExplanation()
{
    std::cout << "' '   = not uncovered yet" << nl;
    std::cout << "'0-9' = uncovered (0-9: number of neighbour bombs)" << nl;
    std::cout << "'F'   = flag" << nl;
    std::cout << "'X'   = bomb :-(" << nl;
}

void Field::printAll()
{
    Common common;
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->bombsCount << " bombs" << nl << nl;
    
    #if DEBUG == 1
        drawField(bombsArray);
        std::cout << nl;
    #endif
        
    drawField(this->fieldArray);
    std::cout << nl;
    printBombsLeft();
    std::cout << nl;
    printExplanation();
    std::cout << nl;
}

bool Field::isFlagOn(Common::coordsStruct coords)
{
    if (this->fieldArray[coords.col][coords.row] == 'F')
        return true;
    else 
        return false;
}

bool Field::isFree(Common::coordsStruct coords)
{
    for (int i = 48; i < 56; i++)
    {
        if (this->fieldArray[coords.col][coords.row] == i)
            return false;
    }
    return true;
}

Common::placeUserInput Field::placeUserInput(Common::userInputStruct userInput, int turn)
{
    Common::placeUserInput returnStruct;
    int neighboursCount = 0;
    if (userInput.isFlag == true)
    {            
        if (isFlagOn(userInput.coords) == true)
        {
            this->fieldArray[userInput.coords.col][userInput.coords.row] = ' ';
            this->bombsLeft++;
            returnStruct.isFlag = true;
            return returnStruct;
        }
        else
        {
            this->fieldArray[userInput.coords.col][userInput.coords.row] = 'F';
            this->bombsLeft--;
            returnStruct.isFlag = true;
            return returnStruct;
        }
    }
    else
    {
        if (turn == 1)
        {
            fillBombsArray();
            while (this->bombsArray[userInput.coords.col][userInput.coords.row] == 'X')
            {
                clearArray(bombsArray);
                fillBombsArray();
            }
        }
        if (this->bombsArray[userInput.coords.col][userInput.coords.row] == 'X')
        {
            hasLost();
            returnStruct.hasLost = true;
            return returnStruct;
        }
        else
        {
            std::vector<Common::coordsStruct> neighboursBombsVector;
            neighboursBombsVector = findNeighbours(this->bombsArray, userInput.coords, 'X');

            neighboursCount = neighboursBombsVector.size();
            
            // place neighboursCount in fieldArray (convert int to char by adding 48):
            this->fieldArray[userInput.coords.col][userInput.coords.row] = neighboursCount+48;
            this->countEmpty--;
            
            int neighboursCountNew = 0;
            
            bool run = true;
            if (neighboursCount == 0)
            {
                while (run == true)
                {
                    // for each free position do:
                    for (int i = 1; i <= this->cols; i++)
                    {
                        for (int j = 1; j <= this->rows; j++)
                        {
                            if (this->fieldArray[i][j] == ' ')
                            {
                                // create a new vector of neighbours containing '0':
                                Common::coordsStruct coordsBase;
                                coordsBase.col = i;
                                coordsBase.row = j;
                                std::vector<Common::coordsStruct> neighboursZerosVector;
                                neighboursZerosVector.clear();
                                neighboursZerosVector = findNeighbours(this->fieldArray, coordsBase, 48);
                                

                                // if there is a neighbour containing a 0 create a new vector of neighbours containing bombs:
                                if (neighboursZerosVector.size() != 0)
                                {
                                    std::vector<Common::coordsStruct> neighboursBombsVectorNew;
                                    neighboursBombsVectorNew.clear();
                                    neighboursBombsVectorNew = findNeighbours(this->bombsArray, coordsBase, 'X');
                                    neighboursCountNew = neighboursBombsVectorNew.size();
            
                                    // place neighboursCountNew in fieldArray (convert int to char by adding 48):
                                    if (this->fieldArray[i][j] == ' ')
                                    {
                                        this->fieldArray[i][j] = neighboursCountNew+48;
                                        this->countEmpty--;
                                    }
                                }
                            }
                        }
                    }
                    run = false;
                    for (int a = 1; a <= this->cols; a++)
                    {
                        for (int b = 1; b <= this->rows; b++)
                        {
                            if (this->fieldArray[a][b] == ' ')
                            {
                                // create a new vector of neighbours containing '0':
                                Common::coordsStruct coordsBaseNew;
                                coordsBaseNew.col = a;
                                coordsBaseNew.row = b;
                                std::vector<Common::coordsStruct> neighboursZerosVectorNew;
                                neighboursZerosVectorNew.clear();
                                neighboursZerosVectorNew = findNeighbours(this->fieldArray, coordsBaseNew, 48);
                                if (neighboursZerosVectorNew.size() != 0)
                                    run = true;
                            }
                        }
                    }
                }
            }
        }
        if (this->countEmpty == bombsCount)
        {
            hasWon();
            returnStruct.hasWon = true;
            return returnStruct;
        }
        
        return returnStruct;
    }
}

void Field::hasWon()
{
    Common common;
    Input input;
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->bombsCount << " bombs" << nl << nl;
    for (int i = 1; i <= this->cols; i++)
        for (int j = 1; j <= this->rows; j++)
            if (bombsArray[i][j] == 'X')
                fieldArray[i][j] = 'X';
        
    drawField(this->fieldArray);
    std::cout << nl;
    std::cout << "Congratulation, you have won!" << nl;
    std::cout << "Press ENTER to go back...";
    input.getAnyKey();
    
}

void Field::hasLost()
{
    Common common;
    Input input;
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->bombsCount << " bombs" << nl << nl;
    drawField(this->bombsArray);
    std::cout << nl;
    std::cout << "Sry, you have lost!" << nl;
    std::cout << "Press ENTER to go back...";
    input.getAnyKey();
}

std::vector<Common::coordsStruct> Field::findNeighbours(char **tempArray, Common::coordsStruct coords, char mark)
{
    std::vector<Common::coordsStruct> neighboursVector;
    
    // up left:
    if (coords.col-1 > 0 && coords.row-1 > 0)
    {
        if (tempArray[coords.col-1][coords.row-1] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col-1;
            tempCoords.row = coords.row-1;
            neighboursVector.push_back(tempCoords);
        }
    }

    // up middle:
    if (coords.row-1 > 0)
    {
        if (tempArray[coords.col][coords.row-1] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col;
            tempCoords.row = coords.row-1;
            neighboursVector.push_back(tempCoords);
        }
    }

    // up right:
    if (coords.col+1 <= this->cols && coords.row-1 > 0)
    {
        if (tempArray[coords.col+1][coords.row-1] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col+1;
            tempCoords.row = coords.row-1;
            neighboursVector.push_back(tempCoords);
        }
    }

    // middle left:
    if (coords.col-1 > 0)
    {
        if (tempArray[coords.col-1][coords.row] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col-1;
            tempCoords.row = coords.row;
            neighboursVector.push_back(tempCoords);
        }
    }
    
    // middle right:
    if (coords.col+1 <= this->cols)
    {
        if (tempArray[coords.col+1][coords.row] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col+1;
            tempCoords.row = coords.row;
            neighboursVector.push_back(tempCoords);
        }
    }

    // down left:
    if (coords.col-1 > 0 && coords.row+1 <= this->rows)
    {
        if (tempArray[coords.col-1][coords.row+1] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col-1;
            tempCoords.row = coords.row+1;
            neighboursVector.push_back(tempCoords);
        }
    }

    // down middle:
    if (coords.row+1 <= this->rows)
    {
        if (tempArray[coords.col][coords.row+1] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col;
            tempCoords.row = coords.row+1;
            neighboursVector.push_back(tempCoords);
        }
    }

    // down right:
    if (coords.col+1 <= this->cols && coords.row+1 <= this->rows)
    {
        if (tempArray[coords.col+1][coords.row+1] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col+1;
            tempCoords.row = coords.row+1;
            neighboursVector.push_back(tempCoords);
        }
    }
    
    return neighboursVector;
}
