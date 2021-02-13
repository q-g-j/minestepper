#ifdef _DEBUG
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        #define _CRTDBG_MAP_ALLOC
        #include <stdlib.h>
        #include <crtdbg.h>
        #define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
        // Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
        // allocations to be of _CLIENT_BLOCK type
    #endif
#endif

#include <iostream>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#endif

#include "../include/colors.hpp"
#include "../include/common.hpp"
#include "../include/debug.hpp"
#include "../include/field.hpp"
#include "../include/input.hpp"
#include "../include/print.hpp"
#include "../include/solver.hpp"

int main()
{
    #if defined(_DEBUG)
        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );        
        #endif
    #endif

    Colors colors;
    Common common;
    Input input;
    Print print;
    
    Common::DifficultyStruct Size;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common.setWindowProperties();
    #endif

    common.setWindowTitle("Minesweeper");

    /* these values can be changed: */
    Size.Small.columns  = 9;
    Size.Small.rows     = 9;
    Size.Small.mines    = 10;

    Size.Medium.columns = 16;
    Size.Medium.rows    = 16;
    Size.Medium.mines   = 40;

    Size.Large.columns  = 30;
    Size.Large.rows     = 16;
    Size.Large.mines    = 99;

    // set X/Y position of the field
    int fieldOffsetX    = 5;
    int fieldOffsetY    = 6;
    /* ----------------------------*/

    int fieldCellWidth = 3;
    int rows, cols;
    int difficulty;
    int minesTotal;
    Common::CoordsStruct dimensions;
    std::string difficultyString;

    std::cout << colors.setTextColor(colors.color_default);

    input.showBlinkingCursor(false);

    while (true)
    {
        Common::UserInputReturnStruct userInput;
        Common::PlaceUserInputReturnStruct placeUserInputReturn;
        common.clearScreen();
        common.resizeConsole(33, 13);

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            common.centerWindow();
            input.showBlinkingCursor(false);
        #endif

        #if MEM_LEAK_TEST_LOOP == 1
            difficulty = 1;
        #else
            difficulty = input.getInputDifficulty();
        #endif

        if (difficulty == 1)
        {
            difficultyString = print.setDifficultyTexts(1);
            cols = Size.Small.columns;
            rows = Size.Small.rows;
            minesTotal = Size.Small.mines;
        }
        else if (difficulty == 2)
        {
            difficultyString = print.setDifficultyTexts(2);
            cols = Size.Medium.columns;
            rows = Size.Medium.rows;
            minesTotal = Size.Medium.mines;
        }
        else if (difficulty == 3)
        {
            difficultyString = print.setDifficultyTexts(3);
            cols = Size.Large.columns;
            rows = Size.Large.rows;
            minesTotal = Size.Large.mines;
        }
        else
        {
            difficultyString = print.setDifficultyTexts(4);
            common.clearScreen();
            common.resizeConsole(66, 6);

            #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
                common.centerWindow();
                input.showBlinkingCursor(true);
            #endif

            dimensions = input.getInputCustomDimensions();
            rows = dimensions.row;
            cols = dimensions.col;
            common.clearScreen();
            common.resizeConsole(42, 6);

            #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
                common.centerWindow();
            #endif

            minesTotal = input.getInputCustomMinesCount(cols * rows);
            input.showBlinkingCursor(false);
        }

        Field field(cols, rows, fieldOffsetX, fieldOffsetY, fieldCellWidth, minesTotal, difficultyString);

        common.clearScreen();
        common.resizeConsole(fieldOffsetX + (cols * 4) + fieldOffsetX - 3, fieldOffsetY + (rows * 2) + 5);

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            common.centerWindow();
            input.showBlinkingCursor(false);
        #endif

        field.gotoXY(field.getOffsetX() - 1, 1);
        print.printTitle(difficultyString, cols, rows, minesTotal);
        field.gotoXY(1, 3);
        field.drawField();

        int turn = 1;
        int firstrun = 1;

        while (true)
        {
            field.gotoXY(1, 4);
            common.deleteLastLine(20);
            field.gotoXY(field.getOffsetX() - 1, field.getOffsetY() - 2);
            std::cout << colors.setTextColor(colors.fg_light_red);
            std::cout << field.getMinesLeft() << print.minesLeftText << std::flush;
            std::cout << colors.setTextColor(colors.color_default);
            #if DEBUG == 1
                field.gotoXY(field.getOffsetX() - 1 + 17, field.getOffsetY() - 2);
                std::cout << "Covered left: " << field.getCovered() <<  "     " << std::flush;
            #endif
            field.gotoXY(1, fieldOffsetY + field.getRows()*2 + 4);

            input.showBlinkingCursor(false);

            #if MEM_LEAK_TEST_LOOP == 1
                userInput.Coords.col = 3;
                userInput.Coords.row = 3;
            #else
                userInput = input.getUserInput(field, firstrun);
            #endif

            firstrun = 0;
            
            placeUserInputReturn = field.placeUserInput(userInput, turn);

            if (placeUserInputReturn.hasLost)
            {
                break;
            }
            else if (placeUserInputReturn.hasWon)
            {
                break;
            }
            else
            {
                if (placeUserInputReturn.isTurn)
                {
                    ++turn;
                }
            }
        }
    }

    return 0;
}
