#pragma once

#include "common.hpp"

class Print
{
private:
    
public:
    // public variables:
    std::string inputText = "Input: ";
    std::string wrongInputText = "Wrong input, Press ENTER...";
    std::string getHelpText = "'h' or 'H': Help";
    std::string minesLeftText = " Mines left...";
    std::string debugTurnCountText = " DEBUG: Turn: ";
    struct SmallStruct
    {
        std::string title = "small";
        int cols = 9;
        int rows = 9;
        int mines = 10;
    };
    struct MediumStruct
    {
        std::string title = "medium";
        int cols = 16;
        int rows = 16;
        int mines = 40;
    };
    struct LargeStruct
    {
        std::string title = "large";
        int cols = 28;
        int rows = 16;
        int mines = 99;
    };
    struct CustomStruct
    {
        std::string title = "custom";
    };
    struct DifficultyStruct
    {
        SmallStruct Small;
        MediumStruct Medium;
        LargeStruct Large;
        CustomStruct Custom;
    };
    
    // public methods:
    void printTitle(std::string const&, int const&, int const&, int const&);
    void printMenu();
    void printCustomGetDimensions();
    void printCustomGetMinesCount();
    void printHasWon();
    void printHasLost();
    void printHelp(Field &field, CoordsStruct&);
    void printExplanation();
};

typedef struct Print::DifficultyStruct DifficultyStruct;
