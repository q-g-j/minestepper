#include <iostream>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#endif

#include "common.hpp"
#include "field.hpp"
#include "input.hpp"
#include "print.hpp"

std::string Print::setDifficultyTexts(int const& mode)
{
    if (mode == 1) return "small";
    else if (mode == 2) return "medium";
    else if (mode == 3) return "large";
    else return "custom";
}

void Print::printTitle(std::string const& difficultyString, int const& cols, int const& rows, int const& minesCount)
{
    std::cout << "Minestepper - " << difficultyString << " (" << cols << "x" << rows << ") - " << minesCount << " mines" << newline << newline << newline;
}

void Print::printMenu()
{
    std::cout << "Welcome to Minestepper - A Minesweeper Clone!" << newline << newline << newline;
    std::cout << "Choose the size of the field!" << newline;
    std::cout << "(make sure, that your terminal window is large enough!)" << newline << newline;
    std::cout << "1: small" << newline;
    std::cout << "2: medium" << newline;
    std::cout << "3: large" << newline;
    std::cout << "4: custom" << newline << newline;
    std::cout << "q: quit at any time" << newline << newline;
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
    Common common;
    Input input;
    
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
    coutconv << field.getMinesLeft();
    std::cout << minesLeftText;
    #if DEBUG == 1
        std::cout << debugTurnCountText << turn << "      ";
    #endif
    field.gotoXY(field.getOffsetX() - 1, field.getOffsetY() + field.getRows()*2);
    std::cout << getHelpText << newline << newline;
    currentCursorPosition = common.convCoordsToCursorPosition(CurrentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
    field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
    common.setUnicode(true);
    coutconv << symbolCursor << std::flush;
}

void Print::printExplanation()
{
    coutconv << "Minestepper - A Minesweeper Clone" << newline << newline;
    coutconv << "In this game your task is to find all hidden mines by uncovering all safe positions." << newline << newline;
    coutconv << "You can guess and sometimes combine where the next mine is." << newline;
    coutconv << "The number on each uncovered square shows how many neighbours contain a mine." << newline << newline;
    coutconv << "If you're sure that you have found a mine, place a flag on it's position by pressing SPACE" << newline;
    coutconv << "To remove the flag, just repeat your input. You may place or remove as many flags in a row as you wish." << newline << newline;
    coutconv << "You can reselect a numbered cell with ENTER to automatically uncover all remaining neighbours," << newline;
    coutconv << "as long as you put all flags right! Otherwise you might lose" << newline << newline;
    coutconv << "Explanations:" << newline << newline;
    coutconv << "Arrow Keys:    navigate" << newline;
    coutconv << "ENTER:         sweep" << newline;
    coutconv << "SPACE:         place or remove a flag" << newline;
    coutconv << "q or Q:        quit" << newline << newline;
    coutconv << "Press ENTER to go back...";
}
