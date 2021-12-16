#pragma once

class Print
{    
private:
    
public:
    Print();
    ~Print();

    static const std::string inputText;
    static const std::string wrongInputText;
    static const std::string getHelpText;
    static const std::string minesLeftText;
    static const std::string debugTurnCountText;

    // public methods:
    static void deleteLastLine(size_t const&);
    static void printTitle(std::string const&, int const&, int const&, int const&);
    static void printMinesLeft(Field&);

    #if DEBUG == 1
        static void printDebugCoveredLeft(Field&);
    #endif

    static std::string setDifficultyTexts(int const&);
    static void printMenu();
    static void printCustomGetCellWidth();
    static void printCustomGetDimensions(int const&);
    static void printCustomGetMinesCount();
    static void printHasWon(Field&);
    static void printHasLost(Field&);
    static void printExplanation();
    static void printTimer(Field&, Common::TimeStruct& ,bool);
};
