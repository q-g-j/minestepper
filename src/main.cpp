#include <iostream>
#include "board.hpp"
#include "common.hpp"
#include "input.hpp"

using namespace std;

int main()
{
    unsigned int rows, cols;
    Common common;
    
    while (true)
    {
        unsigned int difficulty = 0;
        string difficultyString = "";
        unsigned int bombsCount = 0;
        Input::dimensionsStruct dimensions;
        

        Input input;
        
        difficulty = input.getDifficulty();
        
        if (difficulty == 1) 
        {
            difficultyString = "Small";
            cols = 9;
            rows = 9;
            bombsCount = 10;
        }        
        else if (difficulty == 2) 
        {
            difficultyString = "Medium";
            cols = 16;
            rows = 16;
            bombsCount = 10;
        }
        else if (difficulty == 3) 
        {
            difficultyString = "Large";
            cols = 22;
            rows = 22;
            bombsCount = 10;
        }
        else
        {
            difficultyString = "Custom";
            dimensions = input.getDimensions();
            rows = dimensions.rows;
            cols = dimensions.cols;
            bombsCount = input.getBombsCount(sizeof dimensions.cols*dimensions.rows);
        }        
        
        Board board(cols, rows, bombsCount);

        common.clearScreen();
        cout << "Minestepper" << " - " << difficultyString << " (" << cols << "x" << rows << ") - " << bombsCount << " bombs" << nl << nl;
        board.clearBoard();
        board.drawBoard();
        cout << nl;
        board.printExplanation();
        
        input.getAnyKey();
        
    }    
    
    return 0;
}
