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
        dimensionsStruct dimensions;
        int bombsCount = 0;
        while (true)
        {
            clearScreen();
            cout << "Minesweeper" << nl;
            cout << nl;
            if ( !(difficulty = getDifficulty()))
            {
                cout << "Wrong input, Press ENTER... ";
                getAnyKey();
            }
            else break;
        }
        
        if (difficulty == 4)
        {
            while (true)
            {
                clearScreen();
                cout << "Minesweeper" << nl;
                cout << nl;
                dimensions = getDimensions();
                if ( !(dimensions.cols) && !(dimensions.rows) )
                {
                    cout << "Wrong input, Press ENTER... ";
                    getAnyKey();
                }
                else break;
            }
            while (true)
            {
                clearScreen();
                cout << "Minesweeper" << nl;
                cout << nl;
                if ( !(bombsCount = getBombsCount(sizeof dimensions.cols*dimensions.rows)) )
                {
                    cout << "Wrong input, Press ENTER... ";
                    getAnyKey();
                }
                else break;
            }
        }
        
        if (difficulty == 1) 
        {
            difficultyString = " - Small (9x9)";
            rows = 9;
            cols = 9;
        }        
        else if (difficulty == 2) 
        {
            difficultyString = " - Medium (15x15)";
            rows = 15;
            cols = 15;
        }
        else if (difficulty == 3) 
        {
            difficultyString = " - Large (21x21)";
            rows = 21;
            cols = 21;
        }
        else if (difficulty == 4) 
        {
            difficultyString = " - Custom (" + to_string(dimensions.cols) + "x" + to_string(dimensions.rows) + ") - " + to_string(bombsCount) + " Bombs";
            rows = dimensions.rows;
            cols = dimensions.cols;
        }
        

        
        
        board = new char* [rows];
        for (int i=0; i < rows; i++)
            board[i] = new char[cols];

        clearScreen();
        cout << "Minesweeper" << difficultyString << nl << nl << nl;
        clearBoard(board, rows, cols);
        drawBoard(board, rows, cols);
        
        getAnyKey();
        
    }
    
    
    
    
    return 0;
}
