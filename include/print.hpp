#pragma once

#include "common.hpp"
    
class Print : Colors
{
private:
    
public:
    // public constants:
    const std::string inputText = "Input: ";
    const std::string wrongInputText = "Wrong input, Press ENTER...";
    const std::string getHelpText = "'h' or 'H': Help";
    const std::string minesLeftText = " Mines left...";
    const std::string debugTurnCountText = " DEBUG: Turn: ";

    void printTitle(std::string const&, int const&, int const&, int const&);
    std::string setDifficultyTexts(int const&);
    void printMenu();
    void printCustomGetDimensions();
    void printCustomGetMinesCount();
    void printHasWon();
    void printHasLost();
    void printHelp(Field &field, coordsStruct&);
    void printExplanation();
};
