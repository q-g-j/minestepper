#include <iostream>
#include "time.h"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include "Windows.h"
#endif

#include "common.hpp"
#include "debug.hpp"
#include "field.hpp"
#include "input.hpp"

using namespace std;

int main()
{
    int rows = 0, cols = 0;
    Input input;
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        ::srand(GetTickCount());
    #else
        srand( time(NULL) ); // initialize random seed
    #endif
    
    while (true)
    {
        int difficulty = 0;
        string difficultyString = "";
        int minesCount = 0;
        Common::coordsStruct dimensions;
        Common::userInputStruct userInput;
        Common::placeUserInputStruct placeUserInput;
        
        difficulty = input.getDifficulty();
        
        if (difficulty == 1) 
        {
            difficultyString = "Small";
            cols = 9;
            rows = 9;
            minesCount = 10;
        }        
        else if (difficulty == 2) 
        {
            difficultyString = "Medium";
            cols = 16;
            rows = 16;
            minesCount = 40;
        }
        else if (difficulty == 3) 
        {
            difficultyString = "Large";
            cols = 22;
            rows = 22;
            minesCount = 99;
        }
        else
        {
            difficultyString = "Custom";
            dimensions = input.getDimensions();
            rows = dimensions.row;
            cols = dimensions.col;
            minesCount = input.getMinesCount(cols*rows);
        }        
        
        Field field(cols, rows, minesCount, difficultyString);

        int turn = 1;
        while (true)
        {
            field.printAll();
            
            #if DEBUG == 1
                std::cout << "Turn: " << turn << nl << nl;
            #endif
           
            userInput = input.getUserInput(field);
            placeUserInput = field.placeUserInput(userInput, turn);
            if (placeUserInput.hasLost)
                break;
            else if (placeUserInput.hasWon)
                break;
            else
                if (placeUserInput.isTurn == true)
                    turn++;
        }
    }    
    
    return 0;
}
