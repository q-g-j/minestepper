#include <iostream>

#include "common.hpp"
#include "debug.hpp"
#include "field.hpp"
#include "input.hpp"
#include "print.hpp"

int main()
{
    Common common;
    Print print;
    
    common.setRandomSeed();
    int fieldOffsetX = 6;
    int fieldOffsetY = 6;
    int fieldCellWidth = 3;
    int rows = 0, cols = 0;
    int difficulty = 0;
    int minesCount = 0;
    std::string difficultyString;

    Input input;

    while (true)
    {
        CoordsStruct dimensions;
        DifficultyStruct D;
        
        UserInputReturnStruct userInput;
        PlaceUserInputReturnStruct placeUserInputReturn;
        difficulty = input.getDifficulty();
        
        if (difficulty == 1) 
        {
            difficultyString = D.Small.title;
            cols = D.Small.cols;
            rows = D.Small.rows;
            minesCount = D.Small.mines;
        }        
        else if (difficulty == 2) 
        {
            difficultyString = D.Medium.title;
            cols = D.Medium.cols;
            rows = D.Medium.rows;
            minesCount = D.Medium.rows;
        }
        else if (difficulty == 3) 
        {
            difficultyString = D.Large.title;
            cols = D.Large.cols;
            rows = D.Large.rows;
            minesCount = D.Large.mines;
        }
        else
        {
            difficultyString = D.Custom.title;
            dimensions = input.getDimensions();
            rows = dimensions.row;
            cols = dimensions.col;
            minesCount = input.getMinesCount(cols*rows);
        }
        
        Field field(cols, rows, fieldOffsetX, fieldOffsetY, fieldCellWidth, minesCount, difficultyString);            
        
        common.clearScreen();
        print.printTitle(difficultyString, cols, rows, minesCount);
        field.drawField();
        std::cout << newline;
        
        int turn = 1;
        int firstrun = 1;

        while (true)
        {
            field.gotoXY(1, 4);
            input.deleteLastLine(20);
            field.gotoXY(field.getOffsetX() - 1, 3);
            std::cout << field.getMinesLeft() << print.minesLeftText << std::flush;
            #if DEBUG == 1
                std::cout << print.debugTurnCountText << turn <<  "     " << std::flush;
            #endif
            field.gotoXY(1, fieldOffsetY + field.getRows()*2 + 4);

            userInput = input.getUserInput(field, firstrun);            
            firstrun = 0;
            placeUserInputReturn = field.placeUserInput(userInput, turn);
            if (placeUserInputReturn.hasLost)
                break;
            else if (placeUserInputReturn.hasWon)
                break;
            else
            {
                if (placeUserInputReturn.isTurn)
                    turn++;
            }
        }
    }    
    
    return 0;
}
