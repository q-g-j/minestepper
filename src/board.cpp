#include <algorithm>
#include <iostream>
#include <random>
#include <time.h>

#include "board.hpp"
#include "common.hpp"
#include "input.hpp"

Board::Board(int cols, int rows, int bombsCount, std::string difficultyString)
{
    this->cols = cols;
    this->rows = rows;
    this->bombsCount = bombsCount;
    this->bombsLeft = bombsCount;
    this->difficultyString = difficultyString;
    this->boardArray = createArray();
    this->bombsArray = createArray();
    clearBoard(this->boardArray);
    clearBoard(this->bombsArray);
    fillBombsArray();
}

Board::~Board()
{    
    for (int i=0; i <= cols; i++)
        delete this->boardArray[i];
    delete[] this->boardArray;
}

char** Board::createArray()
{
    char** array = new char*[this->cols+1];
    for (int i=0; i <= this->cols; i++)
        array[i] = new char[this->rows+1];
    return array;
}

void Board::clearBoard(char** array)
{
    for (int i=0; i <= this->cols; i++)
    {
        for (int j=0; j <= this->rows; j++)
        {
            array[i][j] = ' ';
        }
    }
}
void Board::fillBombsArray()
{    
    Common::coordsStruct coords;
    int sizeOfboardArray = this->cols * this->rows;
    int tempArray[sizeOfboardArray+1];
    tempArray[0] = 0;
    for (int i = 1; i <= sizeOfboardArray; i++)
        tempArray[i] = i;
    std::random_shuffle(&tempArray[1], &tempArray[sizeOfboardArray+1]);
    for (int i = 1; i <= this->bombsCount; i++)
    {
        coords = intToStruct(tempArray[i]);
        this->bombsArray[coords.col][coords.row] = 'X';
    }
}

void Board::drawBoard(char** array)
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

void Board::printBombsLeft()
{
    std::cout << "There are " << this->bombsLeft << " bombs left." << nl;
}

void Board::printExplanation()
{
    std::cout << "' '   = not uncovered yet" << nl;
    std::cout << "'0-9' = number of neighbour bombs" << nl;
    std::cout << "'X'   = bomb :-(" << nl;
}

void Board::printAll()
{
    Common common;
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->bombsCount << " bombs" << nl << nl;
    drawBoard(bombsArray);
    std::cout << nl;
    drawBoard(this->boardArray);
    std::cout << nl;
    printBombsLeft();
    std::cout << nl;
    printExplanation();
    std::cout << nl;
}

Common::coordsStruct Board::intToStruct(int position)
{
    Common::coordsStruct coords;
    
    if (position <= this->cols)
    {
        coords.col = position;
        coords.row = 1;
    }
    else if (position % this->cols == 0)
    {
        coords.col = this->cols;
        coords.row = position / this->cols;
    }
    else
    {
        coords.col = position % this->cols;
        coords.row = position / this->cols + 1;
    }
    return coords;
}

int Board::structToInt(Common::coordsStruct coords)
{
    int position = 0;
    position = (this->cols) * (coords.row) + coords.col;
    return position;
}

bool Board::hasLost()
{
    return false;
}

