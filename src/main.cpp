#include <iostream>

#include "../include/colors.hpp"
#include "../include/common.hpp"
#include "../include/debug.hpp"
#include "../include/field.hpp"
#include "../include/input.hpp"
#include "../include/print.hpp"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
//    #define _WIN32_WINNT 0x0500
    #include <windows.h>
#endif

int main()
{
    Colors colors;
    Common common;
    Input input;
    Print print;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common.setWindowProperties();
    #endif

    common.setWindowTitle("Minesweeper");

    Common::DifficultyStruct Size;

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
    int fieldOffsetX    = 6;
    int fieldOffsetY    = 7;
    /* ----------------------------*/

    common.setRandomSeed();
    int fieldCellWidth = 3;
    int rows, cols;
    int difficulty;
    int minesTotal;
    Common::CoordsStruct dimensions;
    std::string difficultyString;

    input.showBlinkingCursor(false);

    while (true)
    {
        Common::UserInputReturnStruct userInput;
        Common::PlaceUserInputReturnStruct placeUserInputReturn;
        common.clearScreen();

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            common.resizeConsole(33, 13);
            common.centerWindow();
            input.showBlinkingCursor(false);
        #endif
        difficulty = input.getInputDifficulty();

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

            #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
                common.resizeConsole(66, 6);
                common.centerWindow();
                input.showBlinkingCursor(true);
            #endif

            dimensions = input.getInputDimensions();
            rows = dimensions.row;
            cols = dimensions.col;
            common.clearScreen();

            #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
                common.resizeConsole(42, 6);
                common.centerWindow();
            #endif

            minesTotal = input.getInputMinesCount(cols * rows);
            input.showBlinkingCursor(false);
        }

        Field field(cols, rows, fieldOffsetX, fieldOffsetY, fieldCellWidth, minesTotal, difficultyString);

        common.clearScreen();

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            common.resizeConsole(fieldOffsetX + (cols * 4) + fieldOffsetX - 2, fieldOffsetY + (rows * 2) + 5);
            common.centerWindow();
            input.showBlinkingCursor(false);
        #endif


        field.gotoXY(field.getOffsetX() - 1, 2);
        print.printTitle(difficultyString, cols, rows, minesTotal);
        field.gotoXY(1, 3);
        field.drawField(false);

        int turn = 1;
        int firstrun = 1;

        while (true)
        {
            field.gotoXY(1, 4);
            print.deleteLastLine(20);
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
            userInput = input.getUserInput(field, firstrun);
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
