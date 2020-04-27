#include <iostream>
#include "common.hpp"
#include "drawboard.hpp"
#include "input.hpp"

using namespace std;

int main()
{
    char ** board;
    int rows, cols;
    
    while (true)
    {
        int difficulty = 0;
        string difficultyString;
        int bombsCount = 0;
        Input::dimensionsStruct dimensions;
        Common common;
        Input input;
        Draw draw;
        
        while (true)
        {
            common.clearScreen();
            cout << "Minesweeper" << nl;
            cout << nl;
            if ( !(difficulty = input.getDifficulty()))
            {
                cout << "Wrong input, Press ENTER... ";
                input.getAnyKey();
            }
            else break;
        }
        
        if (difficulty == 4)
        {
            while (true)
            {
                common.clearScreen();
                cout << "Minesweeper" << nl;
                cout << nl;
                dimensions = input.getDimensions();
                if ( !(dimensions.cols) && !(dimensions.rows) )
                {
                    cout << "Wrong input, Press ENTER... ";
                    input.getAnyKey();
                }
                else break;
            }
            while (true)
            {
                common.clearScreen();
                cout << "Minesweeper" << nl;
                cout << nl;
                if ( !(bombsCount = input.getBombsCount(sizeof dimensions.cols*dimensions.rows)) )
                {
                    cout << "Wrong input, Press ENTER... ";
                    input.getAnyKey();
                }
                else break;
            }
        }
        
        if (difficulty == 1) 
        {
            difficultyString = " - Small (9x9) - 10 Bombs";
            rows = 9;
            cols = 9;
        }        
        else if (difficulty == 2) 
        {
            difficultyString = " - Medium (16x16) - 40 Bombs";
            rows = 16;
            cols = 16;
        }
        else if (difficulty == 3) 
        {
            difficultyString = " - Large (22x22) - 99 Bombs";
            rows = 22;
            cols = 22;
        }
        else
        {
            difficultyString = " - Custom (" + to_string(dimensions.cols) + "x" + to_string(dimensions.rows) + ") - " + to_string(bombsCount) + " Bombs";
            rows = dimensions.rows;
            cols = dimensions.cols;
        }        
        
        board = new char* [rows];
        for (int i=0; i < rows; i++)
            board[i] = new char[cols];

        common.clearScreen();
        cout << "Minestepper" << difficultyString << nl << nl;
        common.clearBoard(board, rows, cols);
        draw.drawBoard(board, rows, cols);
        cout << nl;
        draw.printExplanation();
        
        input.getAnyKey();
        break;
        
    }
    
    delete board;
    board = NULL;    
    
    return 0;
}
