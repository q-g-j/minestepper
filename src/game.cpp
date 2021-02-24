// system headers:
#include <iostream>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <process.h>
#endif

// project headers:
#include <colors.hpp>
#include <common.hpp>
#include <field.hpp>
#include <game.hpp>
#include <input.hpp>
#include <print.hpp>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    CONSOLE_SCREEN_BUFFER_INFO origScreenSize;
#else
    struct winsize origScreenSize;
#endif

bool isGameRunning;
bool isTimerPrinting;
bool doPauseTimer;
bool doPrintTimer;
bool hasCheated;
bool isCheatedPrinted;

Game::Game()
:
    colors(std::make_unique<Colors>()),
    common(std::make_unique<Common>()),
    input(std::make_unique<Input>()),
    print(std::make_unique<Print>())
{
    isGameRunning = false;
    isTimerPrinting = false;
    doPauseTimer = false;
    doPrintTimer = false;
    hasCheated = false;
    isCheatedPrinted = false;

    #if !defined(_WIN32) && !defined(WIN32) && !defined(_WIN64) && !defined(WIN64)
        atexit(disableNonCanonicalMode);
    #endif
}

Game::~Game()
{
}

Common::GameModeReturnStruct Game::chooseGamemode()
{
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

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    void Game::gameThread(void* field_)
#else
    void *Game::gameThread(void* field_)
#endif
    {
        Common common;
        Input input;
        Print print;
        Field* field = reinterpret_cast<Field*>(field_);

        Common::UserInputReturnStruct userInput;
        Common::PlaceUserInputReturnStruct placeUserInputReturn;

        int turn = 1;
        int firstrun = 1;

        #if MEM_LEAK_TEST_LOOP != 1
            isGameRunning = true;
        #endif

        while (true)
        {
            print.printMinesLeft(*field);

            #if DEBUG == 1
                print.printDebugCoveredLeft(*field);
            #endif

            common.gotoXY(1, field->getOffsetX() + field->getRows() * 2 + 4);
            print.showBlinkingCursor(false);

            #if MEM_LEAK_TEST_LOOP == 1
                userInput.Coords.col = 3;
                userInput.Coords.row = 3;
            #else
                userInput = input.getUserInput(*field, firstrun);
            #endif

            firstrun = 0;

            placeUserInputReturn = field->placeUserInput(userInput, turn);

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
        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            _endthread();
        #else
            return NULL;
        #endif
    }

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    void Game::timerThread(void* field_)
#else
    void *Game::timerThread(void* field_)
#endif
    {
        Colors colors;
        Common common;

        Field* field = reinterpret_cast<Field*>(field_);

        int timer = 0;

        while (isGameRunning)
        {
            if (hasCheated == false)
            {
                if (doPauseTimer == false)
                {
                    isTimerPrinting = true;
                    if (timer % 20 == 0 || doPrintTimer == true)
                    {
                        if ((timer / 20) < 10)
                            common.gotoXY(field->getOffsetX() + (field->getCols() * (((field->getCellWidth() - 1) / 2) * 2 + 2)) - 5, field->getOffsetY() - 2);
                        else if ((timer / 20) < 100)
                            common.gotoXY(field->getOffsetX() + (field->getCols() * (((field->getCellWidth() - 1) / 2) * 2 + 2)) - 6, field->getOffsetY() - 2);
                        else
                            common.gotoXY(field->getOffsetX() + (field->getCols() * (((field->getCellWidth() - 1) / 2) * 2 + 2)) - 7, field->getOffsetY() - 2);
                        std::cout << colors.setTextColor(colors.fg_light_red);
                        std::cout << timer / 20 << std::flush << " s" << std::flush;
                        std::cout << colors.setTextColor(colors.color_default);
                        doPrintTimer = false;
                    }
                    if (timer < 999 * 20) ++timer;
                    isTimerPrinting = false;
                }
            }
            else if (hasCheated == true && (isCheatedPrinted == false || doPrintTimer == true))
            {
                isTimerPrinting = true;
                common.gotoXY(field->getOffsetX() + (field->getCols() * (((field->getCellWidth() - 1) / 2) * 2 + 2)) - 10, field->getOffsetY() - 2);
                std::cout << colors.setTextColor(colors.fg_light_red);
                std::cout << "cheated!" << std::flush;
                std::cout << colors.setTextColor(colors.color_default);
                doPrintTimer = false;
                isTimerPrinting = false;
                isCheatedPrinted = true;
            }
            #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
                Sleep(50);
            #else
                usleep(50 * 1000);
            #endif
        }

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            _endthread();
        #else
            return NULL;
        #endif
    }

void Game::startGame()
{
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

    Field* fieldP = &field;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        HANDLE hThreads[2];

        hThreads[0] = reinterpret_cast<HANDLE>(_beginthread(&gameThread, 0, fieldP));
        Sleep(10);
        hThreads[1] = reinterpret_cast<HANDLE>(_beginthread(&timerThread, 0, fieldP));

        WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);
    #else
        pthread_t threads[2];

        pthread_create(&threads[0], NULL, &gameThread, fieldP);
        usleep(10 * 1000);
        pthread_create(&threads[1], NULL, &timerThread, fieldP);

        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
    #endif
}
