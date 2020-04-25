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
        
        clearScreen();
        
        board = new char* [rows];
        for (int i=0; i < rows; i++)
            board[i] = new char[cols];

        cout << "Minesweeper" << difficultyString << nl << nl << nl;
        clearBoard(board, rows, cols);
        drawBoard(board, rows, cols);
        
        getAnyKey();
        
    }
    
    
    
    
    return 0;
}
