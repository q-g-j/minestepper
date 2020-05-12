#include <iostream>

#include "../include/colors.hpp"
#include "../include/common.hpp"
#include "../include/debug.hpp"
#include "../include/field.hpp"
#include "../include/input.hpp"
#include "../include/print.hpp"

int main()
{
    Colors colors;
    Common common;
    Input input;
    Print print;
    
    DifficultyStruct Size;

    /* these values can be changed: */
    Size.small.columns  = 9;
    Size.small.rows     = 9;
    Size.small.mines    = 10;

    Size.medium.columns = 16;
    Size.medium.rows    = 16;
    Size.medium.mines   = 40;

    Size.large.columns  = 22;
    Size.large.rows     = 16;
    Size.large.mines    = 99;

    // set X/Y position of the field
    int fieldOffsetX    = 6;
    int fieldOffsetY    = 5;
    /* ----------------------------*/
    
    common.setRandomSeed();
    int fieldCellWidth = 3;
    int rows = 0, cols = 0;
    int difficulty = 0;
    int minesCount = 0;
    CoordsStruct dimensions;
    std::string difficultyString;

    input.showCursor(false);
    
    while (true)
    {        
        UserInputReturnStruct userInput;
        PlaceUserInputReturnStruct placeUserInputReturn;
        common.clearScreen();
        difficulty = input.getDifficulty();
        
        if (difficulty == 1) 
        {
            difficultyString = print.setDifficultyTexts(1);
            cols = Size.small.columns;
            rows = Size.small.rows;
            minesCount = Size.small.mines;
        }        
        else if (difficulty == 2) 
        {
            difficultyString = print.setDifficultyTexts(2);
            cols = Size.medium.columns;
            rows = Size.medium.rows;
            minesCount = Size.medium.mines;
        }
        else if (difficulty == 3) 
        {
            difficultyString = print.setDifficultyTexts(3);
            cols = Size.large.columns;
            rows = Size.large.rows;
            minesCount = Size.large.mines;
        }
        else
        {
            difficultyString = print.setDifficultyTexts(4);
            common.clearScreen();
            input.showCursor(true);
            dimensions = input.getDimensions();
            rows = dimensions.row;
            cols = dimensions.col;
            common.clearScreen();
            minesCount = input.getMinesCount(cols * rows);
            input.showCursor(false);
        }
        
        Field field(cols, rows, fieldOffsetX, fieldOffsetY, fieldCellWidth, minesCount, difficultyString);            
        
        common.clearScreen();
        print.printTitle(difficultyString, cols, rows, minesCount);
        field.drawField();
        
        int turn = 1;
        int firstrun = 1;

        while (true)
        {
            field.gotoXY(1, 4);
            input.deleteLastLine(20);
            field.gotoXY(field.getOffsetX() - 1, field.getOffsetY() - 2);
            std::cout << colors.setTextColor(colors.fg_light_red);
            std::cout << field.getMinesLeft() << print.minesLeftText << std::flush;
            std::cout << colors.setTextColor(colors.color_default);
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
