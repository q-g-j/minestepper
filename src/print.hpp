#pragma once

#include "common.hpp"
#include "field.hpp"

class Print
{
private:
    
public:
    // public constants:
    const std::string inputText = "Input: ";
    const std::string wrongInputText = "Wrong input, Press ENTER...";
    const std::string getHelpText = "'h' or 'H': Help";
    const std::string minesLeftText = " Mines left...";
    const std::string debugTurnCountText = " DEBUG: Turn: ";

    // public methods:
    std::string setDifficultyTexts(int const&);
    void printTitle(std::string const&, int const&, int const&, int const&);
    void printMenu();
    void printCustomGetDimensions();
    void printCustomGetMinesCount();
    void printHasWon();
    void printHasLost();
    void printHelp(Field &field, coordsStruct&);
    void printExplanation();
};
