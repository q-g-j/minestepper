#include <iostream>

#include "common.hpp"
#include "debug.hpp"
#include "field.hpp"
#include "input.hpp"

int main()
{
    Common common;
    common.setRandomSeed();
    
    int rows = 0, cols = 0;
    int fieldOffsetX = 2;
    int fieldOffsetY = 5;
    int fieldCellWidth = 3;

    Input input;

    while (true)
    {
        int difficulty = 0;
        std::string difficultyString = "";
        int minesCount = 0;
        coordsStruct dimensions;
        userInputReturnStruct userInput;
        placeUserInputReturnStruct placeUserInputReturn;
        
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
            cols = 28;
            rows = 16;
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
        
        Field field(cols, rows, fieldOffsetX, fieldOffsetY, fieldCellWidth, minesCount, difficultyString);
            
        field.printAll();
        
        int turn = 1;
        while (true)
        {
            field.gotoXY(1, 4);
            input.deleteLastLine(20);
            field.gotoXY(1, 3);
            std::cout << field.getMinesLeft() << " Mines left...";
            #if DEBUG == 1
                std::cout << " DEBUG: Turn: " << turn;
            #endif
            field.gotoXY(1, fieldOffsetY + field.getRows()*2 + 4);

            userInput = input.getUserInput(field);
            placeUserInputReturn = field.placeUserInput(userInput, turn);
            if (placeUserInputReturn.hasLost)
                break;
            else if (placeUserInputReturn.hasWon)
                break;
            else
            {
                if (placeUserInputReturn.isTurn == true)
                    turn++;
            }
                
        }
    }    
    
    return 0;
}
