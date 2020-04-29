#include <iostream>
#include "time.h"

#include "common.hpp"
#include "debug.hpp"
#include "field.hpp"
#include "input.hpp"

using namespace std;

int main()
{
    int rows = 0, cols = 0;
    Input input;
    srand( time(NULL) ); // initialize random seed
    
    while (true)
    {
        int difficulty = 0;
        string difficultyString = "";
        int bombsCount = 0;
        Common::coordsStruct dimensions;
        Common::placeUserInput placeUserInput;
        
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
            bombsCount = 40;
        }
        else if (difficulty == 3) 
        {
            difficultyString = "Large";
            cols = 22;
            rows = 22;
            bombsCount = 99;
        }
        else
        {
            difficultyString = "Custom";
            dimensions = input.getDimensions();
            rows = dimensions.row;
            cols = dimensions.col;
            bombsCount = input.getBombsCount(cols*rows);
        }        
        
        Field field(cols, rows, bombsCount, difficultyString);

        int turn = 1;
        while (true)
        {
            field.printAll();
            
            #if DEBUG == 1
                std::cout << "Turn: " << turn << nl << nl;
            #endif
           
            placeUserInput = field.placeUserInput(input.getUserInput(field), turn);
            if (placeUserInput.hasLost)
                break;
            else if (placeUserInput.hasWon)
                break;
            else
                if (placeUserInput.isFlag == false)
                    turn++;
        }   
    }    
    
    return 0;
}
