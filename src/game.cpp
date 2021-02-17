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
    print(std::make_unique<Print>()),

    Dimensions(std::make_unique<Common::CoordsStruct>()),
    GameModeReturn(std::make_unique<Common::GameModeReturnStruct>()),
    UserInput(std::make_unique<Common::UserInputReturnStruct>()),
    PlaceUserInputReturn(std::make_unique<Common::PlaceUserInputReturnStruct>())
{
}

Game::~Game()
{
}

Common::GameModeReturnStruct Game::choseGamemode()
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common->setWindowProperties();
        common->resizeConsole(33, 13);
    #endif

    std::cout << colors->setTextColor(colors->color_default);
    common->setWindowTitle("Minesweeper");
    input->showBlinkingCursor(false);
    common->clearScreen();

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common->centerWindow();
        input->showBlinkingCursor(false);
    #endif

    #if MEM_LEAK_TEST_LOOP == 1
        difficulty = 1;
    #else
        difficulty = input->getInputDifficulty();
    #endif

    if (difficulty == 1)
    {
        difficultyString = print->setDifficultyTexts(1);
        GameModeReturn->cols = smallCols;
        GameModeReturn->rows = smallRows;
        GameModeReturn->mines = smallMines;
    }
    else if (difficulty == 2)
    {
        difficultyString = print->setDifficultyTexts(2);
        GameModeReturn->cols = mediumCols;
        GameModeReturn->rows = mediumRows;
        GameModeReturn->mines = mediumMines;
    }
    else if (difficulty == 3)
    {
        difficultyString = print->setDifficultyTexts(3);
        GameModeReturn->cols = largeCols;
        GameModeReturn->rows = largeRows;
        GameModeReturn->mines = largeMines;
    }
    else
    {
        difficultyString = print->setDifficultyTexts(4);
        common->clearScreen();

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            common->resizeConsole(66, 6);
            common->centerWindow();
            input->showBlinkingCursor(true);
        #endif

        *Dimensions = input->getInputCustomDimensions();
        GameModeReturn->cols = Dimensions->col;
        GameModeReturn->rows = Dimensions->row;

        common->clearScreen();

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            common->resizeConsole(42, 6);
            common->centerWindow();
        #endif

        GameModeReturn->mines = input->getInputCustomMinesCount(GameModeReturn->cols * GameModeReturn->rows);
        input->showBlinkingCursor(false);
    }
    return *GameModeReturn;
}

void Game::startGame()
{
    if (fieldCellWidth % 2 == 0) common->exitProgram(1);
    Common::GameModeReturnStruct gameMode = choseGamemode();
    Field field(gameMode.cols, gameMode.rows, fieldOffsetX, fieldOffsetY, fieldCellWidth, gameMode.mines, difficultyString);

    common->clearScreen();

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common->resizeConsole(fieldOffsetX + (gameMode.cols * 4) + fieldOffsetX - 3, fieldOffsetY + (gameMode.rows * 2) + 5);
        common->centerWindow();
        input->showBlinkingCursor(false);
    #endif

    field.gotoXY(field.getOffsetX() - 1, 1);
    print->printTitle(difficultyString, gameMode.cols, gameMode.rows, gameMode.mines);
    field.gotoXY(1, 3);
    field.drawField();

    int turn = 1;
    int firstrun = 1;

    while (true)
    {
        field.gotoXY(1, 4);
        common->deleteLastLine(20);
        field.gotoXY(field.getOffsetX() - 1, field.getOffsetY() - 2);
        std::cout << colors->setTextColor(colors->fg_light_red);
        std::cout << field.getMinesLeft() << print->minesLeftText << std::flush;
        std::cout << colors->setTextColor(colors->color_default);
        #if DEBUG == 1
            field.gotoXY(field.getOffsetX() - 1 + 17, field.getOffsetY() - 2);
            std::cout << "Covered left: " << field.getCoveredLeft() <<  "     " << std::flush;
        #endif
        field.gotoXY(1, fieldOffsetY + field.getRows()*2 + 4);

        input->showBlinkingCursor(false);

        #if MEM_LEAK_TEST_LOOP == 1
            UserInput->Coords.col = 3;
            UserInput->Coords.row = 3;
        #else
            *UserInput = input->getUserInput(field, firstrun);
        #endif

        firstrun = 0;
        
        *PlaceUserInputReturn = field.placeUserInput(*UserInput, turn);

        if (PlaceUserInputReturn->hasLost)
        {
            break;
        }
        else if (PlaceUserInputReturn->hasWon)
        {
            break;
        }
        else
        {
            if (PlaceUserInputReturn->isTurn)
            {
                ++turn;
            }
        }
    }
}

