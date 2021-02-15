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
#include <memory>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#endif

#include "../include/colors.hpp"
#include "../include/common.hpp"
#include "../include/debug.hpp"
#include "../include/field.hpp"
#include "../include/input.hpp"
#include "../include/print.hpp"
#include "../include/symbols.hpp"

Print::Print()
:    
    colors(std::make_unique<Colors>()),
    common(std::make_unique<Common>())
{
}

Print::~Print()
{
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
    std::cout << colors->setTextColor(colors->fg_light_blue);
    std::cout << "MINESWEEPER";
    std::cout << colors->setTextColor(colors->fg_white);
    std::cout << " - " << difficultyString << ": " << cols << "x" << rows << ", " << minesTotal << " mines";
    std::cout << colors->setTextColor(colors->color_default);
    std::cout << newline << newline << newline;
}

void Print::printMenu()
{
    std::cout << newline;
    std::cout << colors->setTextColor(colors->fg_white);
    std::cout << "  Welcome to ";
    std::cout << colors->setTextColor(colors->fg_light_blue);
    std::cout << "MINESWEEPER!";
    std::cout << colors->setTextColor(colors->color_default);
    std::cout << newline << newline << newline;
    std::cout << "  Choose the size of the field:" << newline << newline;
    std::cout << colors->setTextColor(colors->fg_light_green);
    std::cout << "   1:  ";
    std::cout << colors->setTextColor(colors->color_default);
    std::cout << "small" << newline;
    std::cout << colors->setTextColor(colors->fg_light_blue);
    std::cout << "   2:  ";
    std::cout << colors->setTextColor(colors->color_default);
    std::cout << "medium" << newline;
    std::cout << colors->setTextColor(colors->fg_yellow);
    std::cout << "   3:  ";
    std::cout << colors->setTextColor(colors->color_default);
    std::cout << "large" << newline;
    std::cout << colors->setTextColor(colors->fg_magenta);
    std::cout << "   4:  ";
    std::cout << colors->setTextColor(colors->color_default);
    std::cout << "custom" << newline << newline;
    std::cout << colors->setTextColor(colors->fg_light_red);
    std::cout << "   q:  ";
    std::cout << colors->setTextColor(colors->color_default);
    std::cout << "quit at any time" << std::flush;
}

void Print::printCustomGetDimensions()
{
    std::cout << newline;
    std::cout << "  How large do you want the field to be (min. 8x8 / max. 30x20)?" << newline << newline;
}

void Print::printCustomGetMinesCount()
{
    std::cout << newline;
    std::cout << "  How many mines to place on the field?" << newline << newline;
}

void Print::printHasWon(Field &field)
{
    field.gotoXY(1, 4);
    common->deleteLastLine(20);
    field.gotoXY(field.getOffsetX() - 1, field.getOffsetY() - 2);
    std::cout << colors->setTextColor(colors->fg_light_red);
    std::cout << field.getMinesLeft() << minesLeftText << std::flush;

    field.gotoXY(field.getOffsetX() - 1, field.getOffsetY() + field.getRows()*2 + 2);
    std::cout << colors->setTextColor(colors->fg_white);
    std::cout << "Congratulation, you have won!" << newline;
    std::cout << colors->setTextColor(colors->color_default);
}

void Print::printHasLost(Field &field)
{
    field.gotoXY(field.getOffsetX() - 1, field.getOffsetY() + field.getRows()*2 + 2);
    std::cout << colors->setTextColor(colors->fg_white);
    std::cout << "Sorry, you have lost!" << newline;
    std::cout << colors->setTextColor(colors->color_default);
}

void Print::printExplanation()
{
    coutconv << newline;
    std::cout << colors->setTextColor(colors->fg_light_blue);
    coutconv << "  MINESWEEPER" << newline << newline;
    std::cout << colors->setTextColor(colors->color_default);
    coutconv << "  In this game your task is to find all hidden mines by uncovering all safe positions." << newline << newline;
    coutconv << "  You can guess and sometimes combine where the next mine is." << newline;
    coutconv << "  The number on each uncovered square shows how many neighbors contain a mine." << newline;
    coutconv << "  If you're sure that you have found a mine, place a flag on it's position by pressing SPACE." << newline;
    coutconv << "  To remove the flag, just repeat your input. You may place or remove as many flags in a row as you wish." << newline << newline;
    coutconv << "  You can reselect a numbered cell with ENTER to automatically uncover all remaining neighbors," << newline;
    coutconv << "  as long as you put all flags right! Otherwise you might lose..." << newline << newline;
    std::cout << colors->setTextColor(colors->fg_white);
    coutconv << "  Explanations:" << newline << newline;
    std::cout << colors->setTextColor(colors->color_default);
    coutconv << "    Arrow Keys:    navigate" << newline;
    coutconv << "    ENTER:         uncover" << newline;
    coutconv << "    SPACE:         place or remove a flag" << newline;
    coutconv << "    f or F:        let the computer place all the flags for you" << newline;
    coutconv << "    r or R:        trigger auto revealing" << newline;
    coutconv << "    s or S:        place all flags, auto-reveal and repeat recursively" << newline;
    coutconv << "    c or C:        toggle cursor jump to opposite edge on or off" << newline << newline;
    coutconv << "    q or Q:        quit" << newline << newline;
    std::cout << colors->setTextColor(colors->fg_white);
    coutconv << "  Press ENTER to go back...";
    std::cout << colors->setTextColor(colors->color_default);
}