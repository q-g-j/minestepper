#include <iostream>
#include "board.hpp"
#include "common.hpp"

Board::Board(unsigned int cols, unsigned int rows, unsigned int bombsCount)
{
    this->cols = cols;
    this->rows = rows;
    this->bombsCount = bombsCount;
    this->boardArray = createBoardArray();    
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

void Board::printExplanation()
{
    std::cout << "'#'   = not uncovered yet" << nl;
    std::cout << "' '   = uncovered and no bomb" << nl;
    std::cout << "'1-9' = number of neighbour bombs" << nl;
    std::cout << "'X'   = bomb :-(" << nl;
}

