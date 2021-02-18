#pragma once

// system headers:
#include <memory>

// project headers:
#include <debug.hpp>

// forward declarations of classes:
class Colors;
class Common;

class Print
{
private:
    // declare class objects as unique pointers:
    std::unique_ptr<Colors> colors;
    std::unique_ptr<Common> common;
public:
    Print();
    ~Print();

    // public constants:
    const std::string inputText = "  Input: ";
    const std::string wrongInputText = "  Wrong input, Press ENTER...";
    const std::string getHelpText = "'h' or 'H': Help";
    const std::string minesLeftText = " Mines left...";
    const std::string debugTurnCountText = " DEBUG: Turn: ";

    // public methods:
    void printTitle(std::string const&, int const&, int const&, int const&);
    std::string setDifficultyTexts(int const&);
    void printMenu();
    void printCustomGetCellWidth();
    void printCustomGetDimensions(int const&);
    void printCustomGetMinesCount();
    void printHasWon(Field&);
    void printHasLost(Field&);
    void printExplanation();
};
