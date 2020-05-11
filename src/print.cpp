#include <iostream>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#endif

#include "../include/colors.hpp"
#include "../include/common.hpp"
#include "../include/field.hpp"
#include "../include/input.hpp"
#include "../include/print.hpp"
#include "../include/symbols.hpp"

std::string Print::setDifficultyTexts(int const& mode)
{
    if (mode == 1) return "small";
    else if (mode == 2) return "medium";
    else if (mode == 3) return "large";
    else return "custom";
}

void Print::printTitle(std::string const& difficultyString, int const& cols, int const& rows, int const& minesCount)
{
    Colors colors;
    std::cout << colors.setTextColor(colors.fg_light_blue);
    std::cout << "MINESWEEPER";
    std::cout << colors.setTextColor(colors.fg_white);
    std::cout << " - " << difficultyString << " (" << cols << "x" << rows << ") - " << minesCount << " mines";
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
    std::cout << "How large do you want the field to be (e.g. 15x10)?" << newline << newline;
}

void Print::printCustomGetMinesCount()
{
    std::cout << "How many mines to place on the field?" << newline << newline;
}

void Print::printHasWon()
{
    Input input;
    
    std::string goBackString =  "Press ENTER to go back...";
    std::cout << newline;
    std::cout << "Congratulation, you have won!" << newline;
    input.getEnterKey(goBackString);
    
}

void Print::printHasLost()
{
    Input input;
    
    std::string goBackString = "Press ENTER to go back...";
    std::cout << newline;
    std::cout << "Sry, you have lost!" << newline;
    input.getEnterKey(goBackString);
}

void Print::printHelp(Field &field, coordsStruct &CurrentArrayPosition)
{
    Colors colors;
    Common common;
    Input input;
    Symbols symbols;
    
    common.setUnicode(false);
    coordsStruct currentCursorPosition;
    common.clearScreen();
    printExplanation();
    input.getEnterKey("");
    common.clearScreen();
    printTitle(field.getDifficultyString(), field.getCols(), field.getRows(), field.getMinesCount());
    field.drawField();
    std::cout << newline;
    field.gotoXY(field.getOffsetX() - 1, field.getOffsetY() - 2);
    std::cout << colors.setTextColor(colors.fg_light_red);
    std::cout << field.getMinesLeft() << minesLeftText << std::flush;
    std::cout << colors.setTextColor(colors.color_default);
    #if DEBUG == 1
        std::cout << debugTurnCountText << turn << "      ";
    #endif
    field.gotoXY(field.getOffsetX() - 1, field.getOffsetY() + field.getRows()*2);
    std::cout << colors.setTextColor(colors.fg_white);
    std::cout << getHelpText << newline << newline;
    std::cout << colors.setTextColor(colors.color_default);
    currentCursorPosition = common.convCoordsToCursorPosition(CurrentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
    field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
    common.setUnicode(true);
    if (field.getCoordsContent(CurrentArrayPosition) == symbols.symbolFlag || field.isNumber(CurrentArrayPosition))
        field.printCoords(CurrentArrayPosition, true);
    else
        coutconv << symbols.symbolCursor << std::flush;
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
    coutconv << "as long as you put all flags right! Otherwise you might lose" << newline << newline;
    coutconv << "Explanations:" << newline << newline;
    coutconv << "Arrow Keys:    navigate" << newline;
    coutconv << "ENTER:         sweep" << newline;
    coutconv << "SPACE:         place or remove a flag" << newline;
    coutconv << "q or Q:        quit" << newline << newline;
    std::cout << colors.setTextColor(colors.fg_white);
    coutconv << "Press ENTER to go back...";
    std::cout << colors.setTextColor(colors.color_default);
}
