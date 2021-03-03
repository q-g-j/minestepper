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
#include <debug.hpp>
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

Game::~Game() = default;

const Common::GameModeReturnStruct &Game::chooseGamemode()
{
    int difficulty;
    Common::CoordsStruct dimensions;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common->setWindowProperties();
    #endif

    common->resizeConsole(33, 13);
    std::cout << colors->setTextColor(colors->color_default);
    common->setWindowTitle("Minesweeper");
    common->clearScreen();

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common->centerWindow();
    #endif

    #if MEM_LEAK_TEST_LOOP == 1
        difficulty = 3;
    #else
        difficulty = input->getInputDifficulty();
    #endif

    if (difficulty == 1)
    {
        this->chooseGamemodeReturn.difficultyString = print->setDifficultyTexts(1);
        this->chooseGamemodeReturn.cols = smallCols;
        this->chooseGamemodeReturn.rows = smallRows;
        this->chooseGamemodeReturn.mines = smallMines;
        this->chooseGamemodeReturn.cellWidth = 3;
    }
    else if (difficulty == 2)
    {
        this->chooseGamemodeReturn.difficultyString = print->setDifficultyTexts(2);
        this->chooseGamemodeReturn.cols = mediumCols;
        this->chooseGamemodeReturn.rows = mediumRows;
        this->chooseGamemodeReturn.mines = mediumMines;
        this->chooseGamemodeReturn.cellWidth = 3;
    }
    else if (difficulty == 3)
    {
        this->chooseGamemodeReturn.difficultyString = print->setDifficultyTexts(3);
        this->chooseGamemodeReturn.cols = largeCols;
        this->chooseGamemodeReturn.rows = largeRows;
        this->chooseGamemodeReturn.mines = largeMines;
        this->chooseGamemodeReturn.cellWidth = 3;
    }
    else
    {
        this->chooseGamemodeReturn.difficultyString = print->setDifficultyTexts(4);
        common->resizeConsole(38, 7);
        common->clearScreen();

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            common->centerWindow();
        #endif

        this->chooseGamemodeReturn.cellWidth = input->getInputCustomCellWidth();

        common->resizeConsole(42, 7);
        common->clearScreen();

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            common->centerWindow();
        #endif

        dimensions = input->getInputCustomDimensions(this->chooseGamemodeReturn.cellWidth);
        this->chooseGamemodeReturn.cols = dimensions.col;
        this->chooseGamemodeReturn.rows = dimensions.row;

        common->resizeConsole(41, 6);
        common->clearScreen();

        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            common->centerWindow();
        #endif

        this->chooseGamemodeReturn.mines = input->getInputCustomMinesCount(this->chooseGamemodeReturn.cols * this->chooseGamemodeReturn.rows);
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
        Colors colors;
        Common common;
        Input input;
        Print print;
        Field* field = reinterpret_cast<Field*>(field_);

        Common::UserInputReturnStruct userInput;
        Common::PlaceUserInputReturnStruct placeUserInputReturn;

        int turn = 1;
        bool firstrun = true;

        input.showBlinkingCursor(false);
        
        field->printAll();

        while (true)
        {
            print.printMinesLeft(*field);

            #if DEBUG == 1
                print.printDebugCoveredLeft(*field);
            #endif

            common.gotoXY(1, field->getOffsetX() + field->getRows() * 2 + 4);

            #if MEM_LEAK_TEST_LOOP == 1
                userInput.Coords.col = 6;
                userInput.Coords.row = 6;
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

        input.showBlinkingCursor(true);
        
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
                Common::TimeStruct time;
                time = common.secondsToTimeStruct(timer / 10);
                if (timer % 10 == 0 || timer == 0 || doPrintTimer == true)
                {
                    isTimerPrinting = true;
                    common.gotoXY(field->getOffsetX() + (field->getCols() * (((field->getCellWidth() - 1) / 2) * 2 + 2)) - 7, field->getOffsetY() - 2);
                    std::cout << colors.setTextColor(colors.fg_light_red);
                    std::cout << time.minutes + ":" + time.seconds << std::flush;
                    std::cout << colors.setTextColor(colors.color_default);
                    isTimerPrinting = false;
                    doPrintTimer = false;
                }
                if (! (std::stoi(time.minutes) >= 99 && std::stoi(time.seconds) >= 59)) ++timer;
            }
            else if (hasCheated == true && doPauseTimer == false && (isCheatedPrinted == false || doPrintTimer == true))
            {
                isTimerPrinting = true;
                common.gotoXY(field->getOffsetX() + (field->getCols() * (((field->getCellWidth() - 1) / 2) * 2 + 2)) - 10, field->getOffsetY() - 2);
                std::cout << colors.setTextColor(colors.fg_light_red);
                std::cout << "cheated!" << std::flush;
                std::cout << colors.setTextColor(colors.color_default);
                isTimerPrinting = false;
                doPrintTimer = false;
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
