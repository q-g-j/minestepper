// system headers:
#include <iostream>

// project headers:
#include <colors.hpp>
#include <common.hpp>
#include <field.hpp>
#include <game.hpp>
#include <input.hpp>
#include <print.hpp>

Game::Game()
:
    colors(std::make_unique<Colors>()),
    common(std::make_unique<Common>()),
    input(std::make_unique<Input>()),
    print(std::make_unique<Print>())
{
}

Game::~Game()
{
}

Common::GameModeReturnStruct Game::chooseGamemode()
{
    Common::UserInputReturnStruct userInput;
    Common::CoordsStruct dimensions;
    Common::GameModeReturnStruct returnStruct;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common->setWindowProperties();
    #endif

    common->resizeConsole(33, 13);
    std::cout << colors->setTextColor(colors->color_default);
    common->setWindowTitle("Minesweeper");
    print->showBlinkingCursor(false);
    common->clearScreen();

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common->centerWindow();
        print->showBlinkingCursor(false);
    #endif

    #if MEM_LEAK_TEST_LOOP == 1
        difficulty = 1;
    #else
        difficulty = input->getInputDifficulty();
    #endif

    if (difficulty == 1)
    {
        difficultyString = print->setDifficultyTexts(1);
        returnStruct.cols = smallCols;
        returnStruct.rows = smallRows;
        returnStruct.mines = smallMines;
        returnStruct.cellWidth = 3;
    }
    else if (difficulty == 2)
    {
        difficultyString = print->setDifficultyTexts(2);
        returnStruct.cols = mediumCols;
        returnStruct.rows = mediumRows;
        returnStruct.mines = mediumMines;
        returnStruct.cellWidth = 3;
    }
    else if (difficulty == 3)
    {
        difficultyString = print->setDifficultyTexts(3);
        returnStruct.cols = largeCols;
        returnStruct.rows = largeRows;
        returnStruct.mines = largeMines;
        returnStruct.cellWidth = 3;
    }
    else
    {
        difficultyString = print->setDifficultyTexts(4);
        common->resizeConsole(38, 7);
        common->clearScreen();

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            common->centerWindow();
        #endif

        print->showBlinkingCursor(true);
        returnStruct.cellWidth = input->getInputCustomCellWidth();

        common->resizeConsole(42, 7);

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            common->centerWindow();
        #endif

        dimensions = input->getInputCustomDimensions(returnStruct.cellWidth);
        returnStruct.cols = dimensions.col;
        returnStruct.rows = dimensions.row;

        common->resizeConsole(41, 6);
        common->clearScreen();

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            common->centerWindow();
        #endif

        returnStruct.mines = input->getInputCustomMinesCount(returnStruct.cols * returnStruct.rows);

        print->showBlinkingCursor(false);
    }

    return returnStruct;
}

void Game::startGame()
{
    if (fieldCellWidth % 2 == 0) common->exitProgram(1);
    Common::UserInputReturnStruct userInput;
    Common::PlaceUserInputReturnStruct placeUserInputReturn;
    Common::GameModeReturnStruct gameMode = chooseGamemode();
    Field field(gameMode.cols, gameMode.rows, fieldOffsetX, fieldOffsetY, gameMode.cellWidth, gameMode.mines, difficultyString);

    common->resizeConsole(fieldOffsetX + (gameMode.cols * (((field.getCellWidth() - 1) / 2) * 2 + 2)) + fieldOffsetX - 3, fieldOffsetY + (gameMode.rows * 2) + 5);
    common->clearScreen();

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common->centerWindow();
        print->showBlinkingCursor(false);
    #endif

    common->gotoXY(field.getOffsetX() - 1, 1);
    print->printTitle(difficultyString, gameMode.cols, gameMode.rows, gameMode.mines);
    common->gotoXY(1, 3);
    field.drawField();

    int turn = 1;
    int firstrun = 1;

    while (true)
    {
        print->printMinesLeft(field);

        #if DEBUG == 1
            print->printDebugCoveredLeft(field);
        #endif

        common->gotoXY(1, fieldOffsetY + field.getRows()*2 + 4);
        print->showBlinkingCursor(false);

        #if MEM_LEAK_TEST_LOOP == 1
            userInput.Coords.col = 3;
            userInput.Coords.row = 3;
        #else
            userInput = input->getUserInput(field, firstrun);
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

