#include <iostream>
#include <random>
#include <time.h>

#include "board.hpp"
#include "common.hpp"

Board::Board(int cols, int rows, int bombsCount, std::string difficultyString)
{
    this->cols = cols;
    this->rows = rows;
    this->bombsCount = bombsCount;
    this->bombsLeft = bombsCount;
    this->boardArray = createBoardArray();
    this->difficultyString = difficultyString;
}

Board::~Board()
{    
    for (int i=0; i < rows; i++)
        delete this->boardArray[i];
    delete[] this->boardArray;
}

char** Board::createBoardArray()
{
    char** boardArray = new char* [rows];
    for (int i=0; i < rows; i++)
        boardArray[i] = new char[cols];
    return boardArray;
}

void Board::clearBoard()
{
    for (int i=0; i < this->rows; i++)
    {
        for (int j=0; j < this->cols; j++)
        {
            this->boardArray[i][j] = '#';
        }
    }
}

void Board::drawBoard()
{
    std::cout << "    ";
    for (int colNum = 0; colNum < this->cols; colNum++)
    {
        if (colNum < 9)
            std::cout << "  " << colNum + 1 << " ";
        else
            std::cout << " " << colNum + 1 << " ";
    }
    std::cout << nl;
    std::cout << "   ";
    for (int bar = 0; bar < this->cols; bar++)
    {
        std::cout << " " << "---";
    }
    std::cout << nl;

    for (int rowNum = 0; rowNum < this->rows; rowNum++)
    {
        for (int colnum = 0; colnum < this->cols; colnum++)
        {
            if (colnum == 0)
            {
                if (rowNum < 9)
                    std::cout << " " << rowNum+1 << " ";
                else
                    std::cout << rowNum+1 << " ";
            }
            std::cout << "|" << " " << this->boardArray[rowNum][colnum] << " ";
            if (colnum == this->cols-1)
            {
                std::cout << "|" << nl;
                std::cout << "   ";
            }
        }
        for (int bar = 0; bar < this->cols; bar++)
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
    std::cout << "'#'   = not uncovered yet" << nl;
    std::cout << "' '   = uncovered and no bomb" << nl;
    std::cout << "'1-9' = number of neighbour bombs" << nl;
    std::cout << "'X'   = bomb :-(" << nl;
}

void Board::printAll(Common &common)
{
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->bombsCount << " bombs" << nl << nl;
    clearBoard();
    drawBoard();
    std::cout << nl;
    printBombsLeft();
    std::cout << nl;
    printExplanation();
    std::cout << nl;
}

Common::coordsStruct Board::intToStruct(int position)
{
    Common::coordsStruct coords;
    
    if (position < 1 || position > this->cols * this-> rows)
    {
        std::cout << "Exiting with error!" << nl;
        exit(1);
    }
    else if (position < this->cols)
    {
        coords.col = position;
        coords.row = 1;
    }
    else if (position % this->cols == 0)
    {
        coords.col = this->cols;
        coords.row = position / this->cols;
    }
    else if (position % this->cols == position)
    {
        coords.col = position;
        coords.row = 1;
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
    if (coords.col < 1 || coords.col > this->cols || coords.row < 1 || coords.row > this->rows)
    {
        std::cout << "Exiting with error!" << nl;
        exit(1);
    }
    int position = 0;
    position = (this->cols) * (coords.row - 1) + coords.col;
    return position;
}

void Board::fillBoardWithBombs()
{
    
}
    
bool Board::hasLost()
{
    return false;
}

