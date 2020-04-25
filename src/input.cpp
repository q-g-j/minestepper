#include <iostream>

#include "common.hpp"

int getDifficulty()
{   
    std::string line;
    int difficulty;
    std::cout << "Choose the size of the board ('1' for small, '2' for medium and '3' for large): ";
    getline(std::cin, line);
    if (line == "")
        return 0;
    else
    {
        try
        {
            difficulty = stoi(line);
        }
        catch (std::exception &err)
        {
            return 0;
        }
        if (difficulty == 1 || difficulty == 2 || difficulty == 3)
            return difficulty;
        else
            return 0;
    }
}

bool getAnyKey()
{
    std::string line;
    while (true)
    {
        getline(std::cin, line);
        if (line == "")
            return true;
        else
            return true;
    }
}
