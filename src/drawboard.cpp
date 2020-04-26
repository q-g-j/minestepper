#include <iostream>
#include "common.hpp"

void drawBoard(char ** board, int rows, int cols)
{
    std::cout << "   ";
    for (int colNum = 0; colNum < cols; colNum++)
    {
        if (colNum <= 9)
            std::cout << "  " << colNum + 1 << " ";
        else
            std::cout << " " << colNum + 1 << " ";
    }
    std::cout << nl;
    std::cout << "   ";
    for (int bar = 0; bar < cols; bar++)
    {
        std::cout << " " << "---";
    }
    std::cout << nl;

    for (int rowNum = 0; rowNum < rows; rowNum++)
    {
        for (int colnum = 0; colnum < cols; colnum++)
        {
            if (colnum == 0)
            {
                if (rowNum < 9)
                    std::cout << rowNum+1 << "  ";
                else
                    std::cout << rowNum+1 << " ";
            }
            std::cout << "|" << " " << board[rowNum][colnum] << " ";
            if (colnum == cols-1)
            {
                std::cout << "|" << nl;
                std::cout << "   ";
            }
        }
        for (int bar = 0; bar < cols; bar++)
        {
            std::cout << " " << "---";
        }
        std::cout << nl;
    }
}
