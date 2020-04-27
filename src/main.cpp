#include <iostream>

#include "board.hpp"
#include "common.hpp"
#include "input.hpp"

using namespace std;

int main()
{
    int rows = 0, cols = 0;
    Common common;
    Input input;
    srand( time(NULL) ); // initialize random seed
    
    while (true)
    {
        int difficulty = 0;
        string difficultyString = "";
        int bombsCount = 0;
        Common::coordsStruct dimensions;
        Common::coordsStruct userInputCoords;
        
        difficulty = input.getDifficulty(common);
        
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
            dimensions = input.getDimensions(common);
            rows = dimensions.row;
            cols = dimensions.col;
            bombsCount = input.getBombsCount(common, cols*rows);
        }        
        
        Board board(cols, rows, bombsCount, difficultyString);

        while (true)
        {
            int position = 0;
            board.printAll(common);
            
            // test conversion coords -> int and vice versa            
            /*Common::coordsStruct testStruct;
            Common::coordsStruct testStruct2;
            testStruct.col = 6;
            testStruct.row = 1;
            cout << board.structToInt(testStruct) << endl;
            testStruct2 = board.intToStruct(22);
            cout << testStruct2.col << "," << testStruct2.row << endl;*/
            
            input.getAnyKey();
            
            userInputCoords = input.getUserInput(common, board);
            
        }   
    }    
    
    return 0;
}
