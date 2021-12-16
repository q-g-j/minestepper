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
#include <iostream>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#endif

// project headers:
#include <colors.h>
#include <common.h>
#include <debug.h>
#include <field.h>
#include <input.h>
#include <print.h>
#include <symbols.h>

Print::Print() = default;
Print::~Print() = default;

const std::string Print::inputText = "  Input: ";
const std::string Print::wrongInputText = "  Wrong input, Press ENTER...";
const std::string Print::getHelpText = "'h' or 'H': Help";
const std::string Print::minesLeftText = " mines left  ";
const std::string Print::debugTurnCountText = " DEBUG: Turn: ";

// erase particular lines instead of clearing the whole screen:
void Print::deleteLastLine(size_t const& stringLength)
{
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;

    if (GetConsoleScreenBufferInfo(console, &cbsi))
    {
        cursorPosition = cbsi.dwCursorPosition;
    }
    --cursorPosition.Y;

    SetConsoleCursorPosition(console, cursorPosition);
    std::cout << "\r";
    for (size_t i = 0; i < stringLength; ++i)
    {
        std::cout << " ";
    }
    std::cout << "\r";
    std::cout << std::flush;
#else
    std::cout << "\x1b[A";
    std::cout << "\r";
    for (unsigned int i = 0; i < stringLength; ++i)
    {
        std::cout << " ";
    }
    std::cout << "\r";
    std::cout << std::flush;
#endif
}

std::string Print::setDifficultyTexts(int const& mode)
{
    if (mode == 1) return "Small";
    else if (mode == 2) return "Medium";
    else if (mode == 3) return "Large";
    else return "Custom";
}

void Print::printTitle(std::string const& difficultyString, int const& cols, int const& rows, int const& minesTotal)
{
    Colors::setTextColor("fg_light_blue");
    std::cout << "MINESWEEPER";
    Colors::setTextColor("fg_white");
    std::cout << " - " << difficultyString << ": " << cols << "x" << rows << ", " << minesTotal << " mines";
    Colors::setTextColor("color_default");
    std::cout << newline << newline << newline;
}

void Print::printMinesLeft(Field &field)
{
    Common::gotoXY(field.getOffsetX() - 1, field.getOffsetY() - 2);
    deleteLastLine(20);
    Common::gotoXY(field.getOffsetX() - 1, field.getOffsetY() - 2);
    Colors::setTextColor("fg_light_red");
    std::cout << field.getMinesLeft() << minesLeftText << std::flush;
    Colors::setTextColor("color_default");
}

#if DEBUG == 1
    void Print::printDebugCoveredLeft(Field &field)
    {
        #if DEBUG == 1
            Common::gotoXY(field.getOffsetX() + (field.getCols() * (((field.getCellWidth() - 1) / 2) * 2 + 2)) - 15, field.getOffsetY() + (field.getRows() * 2));
            std::cout << "Covered: ";
            if (field.getCoveredLeft() >= 1000)
                std::cout << field.getCoveredLeft() << std::flush;
            else if (field.getCoveredLeft() >= 100)
                std::cout << " " << field.getCoveredLeft() << std::flush;
            else if (field.getCoveredLeft() >= 10)
                std::cout << "  " << field.getCoveredLeft() << std::flush;
            else
                std::cout << "   " << field.getCoveredLeft() << std::flush;
        #endif
    }
#endif

void Print::printMenu()
{
    std::cout << newline;
    Colors::setTextColor("fg_white");
    std::cout << "  Welcome to ";
    Colors::setTextColor("fg_light_blue");
    std::cout << "MINESWEEPER!";
    Colors::setTextColor("color_default");
    std::cout << newline << newline << newline;
    std::cout << "  Choose the size of the field:" << newline << newline;
    Colors::setTextColor("fg_light_green");
    std::cout << "   1:  ";
    Colors::setTextColor("color_default");
    std::cout << "small" << newline;
    Colors::setTextColor("fg_light_blue");
    std::cout << "   2:  ";
    Colors::setTextColor("color_default");
    std::cout << "medium" << newline;
    Colors::setTextColor("fg_yellow");
    std::cout << "   3:  ";
    Colors::setTextColor("color_default");
    std::cout << "large" << newline;
    Colors::setTextColor("fg_magenta");
    std::cout << "   4:  ";
    Colors::setTextColor("color_default");
    std::cout << "custom" << newline << newline;
    Colors::setTextColor("fg_light_red");
    std::cout << "   q:  ";
    Colors::setTextColor("color_default");
    std::cout << "quit at any time" << std::flush;
}

void Print::printCustomGetCellWidth()
{
    std::cout << newline;
    std::cout << "  How wide do you want a cell to be?" << newline;
    std::cout << "  Valid input: 1 or 3." << newline << newline;
}

void Print::printCustomGetDimensions(int const& cellWidth)
{
    std::cout << newline;
    std::cout << "  How large do you want the field to be?" << newline;
    if (cellWidth == 1)
    {
        std::cout << "  (min. 16x8 / max. 80x20)" << newline << newline;
    }
    else
    {
        std::cout << "  (min. 8x8 / max. 40x20)" << newline << newline;
    }
}

void Print::printCustomGetMinesCount()
{
    std::cout << newline;
    std::cout << "  How many mines to place on the field?" << newline << newline;
}

void Print::printHasWon(Field &field)
{
    Common::gotoXY(1, 4);
    deleteLastLine(20);
    Common::gotoXY(field.getOffsetX() - 1, field.getOffsetY() - 2);
    Colors::setTextColor("fg_light_red");
    std::cout << field.getMinesLeft() << minesLeftText << std::flush;

    Common::gotoXY(field.getOffsetX() - 1, field.getOffsetY() + field.getRows()*2 + 2);
    Colors::setTextColor("fg_white");
    std::cout << "Congratulation, you have won!" << newline;
    Colors::setTextColor("color_default");
}

void Print::printHasLost(Field &field)
{
    Common::gotoXY(field.getOffsetX() - 1, field.getOffsetY() + field.getRows()*2 + 2);
    Colors::setTextColor("fg_white");
    std::cout << "Sorry, you have lost!" << newline;
    Colors::setTextColor("color_default");
}

void Print::printExplanation()
{
    coutconv << newline;
    Colors::setTextColor("fg_light_blue");
    coutconv << "  MINESWEEPER" << newline << newline;
    Colors::setTextColor("color_default");
    coutconv << "  In this game your task is to find all hidden mines by revealing all safe positions." << newline << newline;
    coutconv << "  You can guess and sometimes combine where the next mine is." << newline;
    coutconv << "  The number on each revealed square shows how many neighbors contain a mine." << newline;
    coutconv << "  If you're sure that you have found a mine, place a flag on its position by pressing SPACE." << newline;
    coutconv << "  To remove the flag, just repeat your input. You may place or remove as many flags in a row as you wish." << newline << newline;
    coutconv << "  You can reselect a numbered cell with ENTER to automatically reveal all remaining neighbors," << newline;
    coutconv << "  as long as you put all flags right! Otherwise you might lose..." << newline << newline;
    Colors::setTextColor("fg_white");
    coutconv << "  Controls:" << newline << newline;
    Colors::setTextColor("color_default");
    coutconv << "    Arrow Keys:    navigate" << newline;
    coutconv << "    ENTER:         reveal" << newline;
    coutconv << "    SPACE:         place or remove a flag" << newline;
    coutconv << "    f or F:        let the computer place the flags for you" << newline;
    coutconv << "    r or R:        trigger auto revealing (after placing flags)" << newline;
    coutconv << "    s or S:        automatically place flags, auto-reveal and repeat recursively" << newline;
    coutconv << "    c or C:        toggle cursor jump to opposite edge on or off" << newline << newline;
    coutconv << "    q or Q:        quit" << newline << newline;
    Colors::setTextColor("fg_white");
    coutconv << "  Press ENTER to go back...";
    Colors::setTextColor("color_default");
}

void Print::printTimer(Field &field, Common::TimeStruct &time, bool hasCheated)
{
    Colors::setTextColor("fg_light_red");
    if (hasCheated == false)
    {
        Common::gotoXY(field.getOffsetX() + (field.getCols() * (((field.getCellWidth() - 1) / 2) * 2 + 2)) - 7, field.getOffsetY() - 2);
        std::cout << time.minutes + ":" + time.seconds << std::flush;
    }
    else
    {
        Common::gotoXY(field.getOffsetX() + (field.getCols() * (((field.getCellWidth() - 1) / 2) * 2 + 2)) - 10, field.getOffsetY() - 2);
        std::cout << "cheated!" << std::flush;
    }
    Colors::setTextColor("color_default");
}
