#include <algorithm>
#include <iostream>
#include <random>

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
    if (userInput.isFlag == true)
    {            
        if (isFlagOn(userInput.coords) == true)
        {
            this->fieldArray[userInput.coords.col][userInput.coords.row] = ' ';
            returnStruct.hasLost = false;
            returnStruct.isFlag = true;
            return returnStruct;
        }
        else
        {
            this->fieldArray[userInput.coords.col][userInput.coords.row] = 'F';
            returnStruct.hasLost = false;
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
            returnStruct.isFlag = false;
            return returnStruct;
        }
        else
        {
            this->fieldArray[userInput.coords.col][userInput.coords.row] = '0';
            returnStruct.hasLost = false;
            returnStruct.isFlag = false;
            return returnStruct;
        }
    }
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

