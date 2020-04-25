#include <iostream>
#include "common.hpp"

void drawBoard(char ** board, int rows, int cols)
{
    std::cout << "  ";
    for (int colnum = 0; colnum < cols; colnum++)
    {
        if (colnum <= 9)
            std::cout << "  " << colnum + 1 << " ";
        else
            std::cout << " " << colnum + 1 << " ";
    }
    std::cout << nl;
    std::cout << "   ";
    for (int bar = 0; bar < cols; bar++)
    {
        std::cout << " " << "---";
    }
    std::cout << nl;

    for (int rownum = 0; rownum < rows; rownum++)
    {
        for (int colnum = 0; colnum < cols; colnum++)
        {
            if (colnum == 0)
            {
                if (rownum < 9)
                    std::cout << rownum+1 << "  ";
                else
                    std::cout << rownum+1 << " ";
            }
            std::cout << "|" << " " << board[rownum][colnum] << " ";
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
