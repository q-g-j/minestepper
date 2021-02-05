#include <iostream>

#include "../include/colors.hpp"
#include "../include/common.hpp"
#include "../include/debug.hpp"
#include "../include/field.hpp"
#include "../include/input.hpp"
#include "../include/print.hpp"
#include "../include/symbols.hpp"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#endif

std::string Print::setDifficultyTexts(int const& mode)
{
    if (mode == 1) return "small";
    else if (mode == 2) return "medium";
    else if (mode == 3) return "large";
    else return "custom";
}

void Print::printTitle(std::string const& difficultyString, int const& cols, int const& rows, int const& minesTotal)
{
    Colors colors;
    std::cout << colors.setTextColor(colors.fg_light_blue);
    std::cout << "MINESWEEPER";
    std::cout << colors.setTextColor(colors.fg_white);
    std::cout << " - " << difficultyString << " (" << cols << "x" << rows << ") - " << minesTotal << " mines";
    std::cout << colors.setTextColor(colors.color_default);
    std::cout << newline << newline << newline;
}

void Print::printMenu()
{
    Colors colors;
    std::cout << colors.setTextColor(colors.fg_white);
    std::cout << "Welcome to ";
    std::cout << colors.setTextColor(colors.fg_light_blue);
    std::cout << "MINESWEEPER!";
    std::cout << colors.setTextColor(colors.color_default);
    std::cout << newline << newline << newline;
    std::cout << "Choose the size of the field!" << newline;
    std::cout << "(make sure, that your terminal window is large enough!)" << newline << newline;
    std::cout << colors.setTextColor(colors.fg_light_green);
    std::cout << "   1:  ";
    std::cout << colors.setTextColor(colors.color_default);
    std::cout << "small" << newline;
    std::cout << colors.setTextColor(colors.fg_light_blue);
    std::cout << "   2:  ";
    std::cout << colors.setTextColor(colors.color_default);
    std::cout << "medium" << newline;
    std::cout << colors.setTextColor(colors.fg_yellow);
    std::cout << "   3:  ";
    std::cout << colors.setTextColor(colors.color_default);
    std::cout << "large" << newline;
    std::cout << colors.setTextColor(colors.fg_magenta);
    std::cout << "   4:  ";
    std::cout << colors.setTextColor(colors.color_default);
    std::cout << "custom" << newline << newline;
    std::cout << colors.setTextColor(colors.fg_light_red);
    std::cout << "   q:  ";
    std::cout << colors.setTextColor(colors.color_default);
    std::cout << "quit at any time" << newline << newline;
}

void Print::printCustomGetDimensions()
{
    std::cout << "How large do you want the field to be (min. 5x5 / max. 26x20)?" << newline << newline;
}

void Print::printCustomGetMinesCount()
{
    std::cout << "How many mines to place on the field?" << newline << newline;
}

void Print::printHasWon(Field &field)
{
    Colors colors;

    field.gotoXY(1, 4);
    deleteLastLine(20);
    field.gotoXY(field.getOffsetX() - 1, field.getOffsetY() - 2);
    std::cout << colors.setTextColor(colors.fg_light_red);
    std::cout << field.getMinesLeft() << minesLeftText << std::flush;

    field.gotoXY(field.getOffsetX() - 1, field.getRows()*2 + 7);
    std::cout << colors.setTextColor(colors.fg_white);
    std::cout << "Congratulation, you have won!" << newline;
    std::cout << colors.setTextColor(colors.color_default);
}

void Print::printHasLost(Field &field)
{
    Colors colors;

    field.gotoXY(field.getOffsetX() - 1, field.getRows()*2 + 7);
    std::cout << colors.setTextColor(colors.fg_white);
    std::cout << "Sorry, you have lost!" << newline;
    std::cout << colors.setTextColor(colors.color_default);
}

void Print::printExplanation()
{
    Colors colors;
    std::cout << colors.setTextColor(colors.fg_light_blue);
    coutconv << "MINESWEEPER" << newline << newline;
    std::cout << colors.setTextColor(colors.color_default);
    coutconv << "In this game your task is to find all hidden mines by uncovering all safe positions." << newline << newline;
    coutconv << "You can guess and sometimes combine where the next mine is." << newline;
    coutconv << "The number on each uncovered square shows how many neighbours contain a mine." << newline << newline;
    coutconv << "If you're sure that you have found a mine, place a flag on it's position by pressing SPACE." << newline;
    coutconv << "To remove the flag, just repeat your input. You may place or remove as many flags in a row as you wish." << newline << newline;
    coutconv << "You can reselect a numbered cell with ENTER to automatically uncover all remaining neighbours," << newline;
    coutconv << "as long as you put all flags right! Otherwise you might lose..." << newline << newline;
    coutconv << "Explanations:" << newline << newline;
    coutconv << "Arrow Keys:    navigate" << newline;
    coutconv << "ENTER:         uncover" << newline;
    coutconv << "SPACE:         place or remove a flag" << newline;
    coutconv << "q or Q:        quit" << newline << newline;
    std::cout << colors.setTextColor(colors.fg_white);
    coutconv << "Press ENTER to go back...";
    std::cout << colors.setTextColor(colors.color_default);

}// erase particular lines instead of clearing the whole screen:
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
        for (unsigned int i = 0; i < stringLength; ++i)
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
