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
#include <colors.h>
#include <common.h>
#include <debug.h>
#include <field.h>
#include <game.h>
#include <input.h>
#include <print.h>

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
{
    isGameRunning = false;
    isTimerPrinting = false;
    doPauseTimer = false;
    doPrintTimer = false;
    hasCheated = false;
    isCheatedPrinted = false;
}

Game::~Game() = default;

const Common::GameModeReturnStruct &Game::chooseGamemode()
{
    Input input;

    int difficulty;
    Common::CoordsStruct dimensions;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        Common::setWindowProperties();
    #endif

    Common::resizeConsole(33, 13);
    Colors::setTextColor("color_default");
    Common::setWindowTitle("Minesweeper");
    Common::clearScreen();

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        Common::centerWindow();
    #endif

    #if MEM_LEAK_TEST_LOOP == 1
        difficulty = 3;
    #else
        difficulty = input.getInputDifficulty();
    #endif

    if (difficulty == 1)
    {
        this->chooseGamemodeReturn.difficultyString = Print::setDifficultyTexts(1);
        this->chooseGamemodeReturn.cols = smallCols;
        this->chooseGamemodeReturn.rows = smallRows;
        this->chooseGamemodeReturn.mines = smallMines;
        this->chooseGamemodeReturn.cellWidth = 3;
    }
    else if (difficulty == 2)
    {
        this->chooseGamemodeReturn.difficultyString = Print::setDifficultyTexts(2);
        this->chooseGamemodeReturn.cols = mediumCols;
        this->chooseGamemodeReturn.rows = mediumRows;
        this->chooseGamemodeReturn.mines = mediumMines;
        this->chooseGamemodeReturn.cellWidth = 3;
    }
    else if (difficulty == 3)
    {
        this->chooseGamemodeReturn.difficultyString = Print::setDifficultyTexts(3);
        this->chooseGamemodeReturn.cols = largeCols;
        this->chooseGamemodeReturn.rows = largeRows;
        this->chooseGamemodeReturn.mines = largeMines;
        this->chooseGamemodeReturn.cellWidth = 3;
    }
    else
    {
        this->chooseGamemodeReturn.difficultyString = Print::setDifficultyTexts(4);
        Common::resizeConsole(38, 7);
        Common::clearScreen();

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            Common::centerWindow();
        #endif

        this->chooseGamemodeReturn.cellWidth = input.getInputCustomCellWidth();

        Common::resizeConsole(42, 7);
        Common::clearScreen();

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            Common::centerWindow();
        #endif

        dimensions = input.getInputCustomDimensions(this->chooseGamemodeReturn.cellWidth);
        this->chooseGamemodeReturn.cols = dimensions.col;
        this->chooseGamemodeReturn.rows = dimensions.row;

        Common::resizeConsole(41, 6);
        Common::clearScreen();

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            Common::centerWindow();
        #endif

        this->chooseGamemodeReturn.mines = input.getInputCustomMinesCount(this->chooseGamemodeReturn.cols * this->chooseGamemodeReturn.rows);
    }

    this->chooseGamemodeReturn.offsetX = fieldOffsetX;
    this->chooseGamemodeReturn.offsetY = fieldOffsetY;
    return this->chooseGamemodeReturn;
}

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    unsigned __stdcall Game::gameThread(void* field_)
#else
    void *Game::gameThread(void* field_)
#endif
    {
        Input input;
        Field* field = reinterpret_cast<Field*>(field_);

        Common::UserInputReturnStruct userInput;
        Common::PlaceUserInputReturnStruct placeUserInputReturn;

        int turn = 1;
        bool firstrun = true;

        Input::showBlinkingCursor(false);
        
        field->printAll();

        while (true)
        {
            Print::printMinesLeft(*field);

            #if DEBUG == 1
                Print::printDebugCoveredLeft(*field);
            #endif

            Common::gotoXY(1, field->getOffsetX() + field->getRows() * 2 + 4);

            #if MEM_LEAK_TEST_LOOP == 1
                userInput.coords.col = 6;
                userInput.coords.row = 6;
            #else
                isGameRunning = true;
                userInput = input.getInputGameplay(*field, firstrun);
            #endif

            firstrun = 0;

            placeUserInputReturn = field->placeUserInput(userInput, turn);

            if (placeUserInputReturn.hasLost) { break; }
            else if (placeUserInputReturn.hasWon) { break; }
            else if (placeUserInputReturn.isTurn) { turn = 2; }
        }

        Input::showBlinkingCursor(true);
        
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
        Field* field = reinterpret_cast<Field*>(field_);

        unsigned int timer = 0;

        while (isGameRunning)
        {
            Common::TimeStruct time;
            if (hasCheated == false && doPauseTimer == false)
            {
                time = Common::secondsToTimeStruct(timer / 10);
                if (timer % 10 == 0 || timer == 0 || doPrintTimer == true)
                {
                    isTimerPrinting = true;
                    Print::printTimer(*field, time, false);
                    isTimerPrinting = false;
                    doPrintTimer = false;
                }
                if (! (std::stoi(time.minutes) >= 99 && std::stoi(time.seconds) >= 59)) ++timer;
            }
            else if (hasCheated == true && doPauseTimer == false && (isCheatedPrinted == false || doPrintTimer == true))
            {
                isTimerPrinting = true;
                Print::printTimer(*field, time, true);
                isTimerPrinting = false;
                doPrintTimer = false;
                isCheatedPrinted = true;
            }
            Common::preciseSleep(0.1);
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

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        HANDLE hThreads[2];

        hThreads[0] = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &gameThread, &field, 0, 0));
        #if MEM_LEAK_TEST_LOOP == 0
            while (isGameRunning == false) Sleep(1);
        #endif
        hThreads[1] = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &timerThread, &field, 0, 0));

        WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

        if (hThreads[0] != nullptr) CloseHandle(hThreads[0]);
        if (hThreads[1] != nullptr) CloseHandle(hThreads[1]);
    #else
        pthread_t threads[2];

        pthread_create(&threads[0], NULL, &gameThread, &field);
        #if MEM_LEAK_TEST_LOOP == 0
            while (isGameRunning == false) usleep(1000);
        #endif
        pthread_create(&threads[1], NULL, &timerThread, &field);

        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
    #endif
}
