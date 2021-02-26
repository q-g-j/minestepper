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

// system headers:
#include <atomic>
#include <iostream>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <process.h>
    #include <thread>
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

std::atomic<bool> isGameRunning;
std::atomic<bool> isTimerPrinting;
std::atomic<bool> doPauseTimer;
std::atomic<bool> doPrintTimer;
std::atomic<bool> hasCheated;
std::atomic<bool> isCheatedPrinted;

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
}

Game::~Game() { }

Common::GameModeReturnStruct Game::chooseGamemode()
{
    Common::CoordsStruct dimensions;
    Common::GameModeReturnStruct gameMode;

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
        gameMode.difficultyString = print->setDifficultyTexts(1);
        gameMode.cols = smallCols;
        gameMode.rows = smallRows;
        gameMode.mines = smallMines;
        gameMode.cellWidth = 3;
    }
    else if (difficulty == 2)
    {
        gameMode.difficultyString = print->setDifficultyTexts(2);
        gameMode.cols = mediumCols;
        gameMode.rows = mediumRows;
        gameMode.mines = mediumMines;
        gameMode.cellWidth = 3;
    }
    else if (difficulty == 3)
    {
        gameMode.difficultyString = print->setDifficultyTexts(3);
        gameMode.cols = largeCols;
        gameMode.rows = largeRows;
        gameMode.mines = largeMines;
        gameMode.cellWidth = 3;
    }
    else
    {
        gameMode.difficultyString = print->setDifficultyTexts(4);
        common->resizeConsole(38, 7);
        common->clearScreen();
        print->showBlinkingCursor(true);

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            common->centerWindow();
        #else
            disableNonCanonicalMode();
        #endif

        gameMode.cellWidth = input->getInputCustomCellWidth();

        common->resizeConsole(42, 7);
        common->clearScreen();

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            common->centerWindow();
        #endif

        dimensions = input->getInputCustomDimensions(gameMode.cellWidth);
        gameMode.cols = dimensions.col;
        gameMode.rows = dimensions.row;

        common->resizeConsole(41, 6);
        common->clearScreen();

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            common->centerWindow();
        #endif

        gameMode.mines = input->getInputCustomMinesCount(gameMode.cols * gameMode.rows);

        print->showBlinkingCursor(false);
    }

    gameMode.offsetX = fieldOffsetX;
    gameMode.offsetY = fieldOffsetY;
    return gameMode;
}

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    unsigned __stdcall Game::gameThread(void* field_)
#else
    void *Game::gameThread(void* field_)
#endif
    {
        Colors colors;
        Common common;
        Input input;
        Print print;
        Field* field = reinterpret_cast<Field*>(field_);

        Common::UserInputReturnStruct userInput;
        Common::PlaceUserInputReturnStruct placeUserInputReturn;

        int turn = 1;
        int firstrun = 1;

        field->printAll();

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
                isGameRunning = true;
                userInput = input.getInputGameplay(*field, firstrun);
            #endif

            firstrun = 0;

            placeUserInputReturn = field->placeUserInput(userInput, turn);

            if (placeUserInputReturn.hasLost) { break; }
            else if (placeUserInputReturn.hasWon) { break; }
            else if (placeUserInputReturn.isTurn) { ++turn; }
        }
        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            return 0;
        #else
            return NULL;
        #endif
    }

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    unsigned __stdcall Game::timerThread(void* field_)
#else
    void *Game::timerThread(void* field_)
#endif
    {
        Colors colors;
        Common common;

        Field* field = reinterpret_cast<Field*>(field_);

        unsigned int timer = 0;

        while (isGameRunning)
        {
            if (hasCheated == false && doPauseTimer == false)
            {
                isTimerPrinting = true;
                Common::TimeStruct time;
                time = common.secondsToTimeStruct(timer / 10);
                if (timer % 10 == 0 || timer == 0 || doPrintTimer == true)
                {
                    common.gotoXY(field->getOffsetX() + (field->getCols() * (((field->getCellWidth() - 1) / 2) * 2 + 2)) - 7, field->getOffsetY() - 2);
                    std::cout << colors.setTextColor(colors.fg_light_red);
                    std::cout << time.minutes + ":" + time.seconds << std::flush;
                    std::cout << colors.setTextColor(colors.color_default);
                    doPrintTimer = false;
                }
                if (! (std::stoi(time.minutes) >= 99 && std::stoi(time.seconds) >= 59)) ++timer;
                isTimerPrinting = false;
            }
            else if (hasCheated == true && doPauseTimer == false && (isCheatedPrinted == false || doPrintTimer == true))
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
            common.preciseSleep(0.1);
        }

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            return 0;
        #else
            return NULL;
        #endif
    }

void Game::startGame()
{
    Common::GameModeReturnStruct gameMode = chooseGamemode();
    Field field(gameMode);
    Field* fieldP = &field;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        HANDLE hThreads[2];

        hThreads[0] = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &gameThread, fieldP, 0, 0));
        #if MEM_LEAK_TEST_LOOP == 0
            while (isGameRunning == false) Sleep(1);
        #endif
        hThreads[1] = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &timerThread, fieldP, 0, 0));

        WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

        if (hThreads[0] != nullptr) CloseHandle(hThreads[0]);
        if (hThreads[1] != nullptr) CloseHandle(hThreads[1]);
    #else
        pthread_t threads[2];

        pthread_create(&threads[0], NULL, &gameThread, fieldP);
        #if MEM_LEAK_TEST_LOOP == 0
            while (isGameRunning == false) usleep(1000);
        #endif
        pthread_create(&threads[1], NULL, &timerThread, fieldP);

        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
    #endif
}
