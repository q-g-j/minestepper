#include <iostream>
#include <string>

#include "common.hpp"
#include "field.hpp"
#include "common.hpp"
#include "input.hpp"

// custom mode: ask user for the game mode (difficulty):
int Input::getDifficulty()
{
    Common common;   
    std::string line = "";
    int difficulty = 0;
    bool isValidInput = false;
   
    while (true)
    {
        common.clearScreen();
        std::cout << "Welcome to Minestepper - a Minesweeper clone!" << nl << nl << nl;
        std::cout << "Choose the size of the field!" << nl << nl;
        std::cout << "1: small" << nl;
        std::cout << "2: medium" << nl;
        std::cout << "3: large" << nl;
        std::cout << "4: custom" << nl << nl;
        std::cout << "q: quit at any time" << nl << nl;
        std::cout << "Make your choice: ";
        
        getline(std::cin, line);
        if (line == "")
            isValidInput = false;
        if (line == "q" || line == "Q")
            exit (0);
        else
        {
            try
            {
                difficulty = stoi(line);
            }
            catch (...)
            {
                isValidInput = false;
            }
            if (difficulty <= 4 && difficulty >= 1)
                isValidInput = true;
            else
                isValidInput = false;
        }
        if (isValidInput == true)
            return difficulty;
        else
        {
            std::cout << "Wrong input, Press ENTER... ";
            getAnyKey();
            std::cout << "\e[A\r\e[K" << std::flush;
        }
    }
}

// custom mode: ask user for the size of the field:
coordsStruct Input::getDimensions()
{
    Common common;
    coordsStruct dimensions;
    std::string line = "";
    int beforeX = 0;
    int afterX = 0;
    bool isValidInput = false;
    
    while (true)
    {
        common.clearScreen();
        std::cout << "How large do you want the field to be?" << nl << nl;
        std::cout << "(e.g. 15x10): ";
        getline(std::cin, line);
        if (line == "")
        {
            isValidInput = false;
        if (line == "q" || line == "Q")
            exit (0);
        }
        else
        {
            if(line.find("x") != std::string::npos)
            {
                try
                {
                    beforeX = stoi(line.substr(0, line.find("x")));
                }
                catch (...)
                {
                    isValidInput = false;
                }
                try
                {
                    afterX = stoi(line.substr(line.find("x") + 1));
                }
                catch (...)
                {
                    try
                    {
                        afterX = stoi(line.substr(line.find("x")));
                    }
                    catch (...)
                    {
                        isValidInput = false;
                    }
                    isValidInput = false;
                }
                isValidInput = true;
            }
            else
            {
                isValidInput = false;
            }
            if (beforeX < 5 || afterX < 5 || beforeX > 25 || afterX > 25)
            {
                isValidInput = false;
            }
        }
        if (isValidInput == true)
        {
            dimensions.col = beforeX;
            dimensions.row = afterX;
            return dimensions;
        }
        else
        {
            std::cout << "Wrong input, Press ENTER... ";
            getAnyKey();
            std::cout << "\e[A\r\e[K" << std::flush;
        }
    }
}

// custom mode: ask user for the number of mines:
int Input::getMinesCount(int fieldSize)
{
    Common common;
    std::string line = "";
    int minesCount = 0;
    bool isValidInput = false;
    
    while (true)
    {
        common.clearScreen();
        std::cout << "How many mines to place on the field? ";
        getline(std::cin, line);
        if (line == "")
            isValidInput = false;
        if (line == "q" || line == "Q")
            exit (0);
        else
        {
            try
            {
                minesCount = stoi(line);
            }
            catch (...)
            {
                isValidInput = false;
            }
            if (minesCount > 0 && minesCount < fieldSize)
                isValidInput = true;
            else
                isValidInput = false;
        }
        if (isValidInput == true)
            return minesCount;
        else
        {
            std::cout << "Wrong input, Press ENTER... ";
            getAnyKey();
            std::cout << "\e[A\r\e[K" << std::flush;
        }
    }
}

bool Input::getAnyKey()
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

// the main function to ask the user for valid coordinates:
userInputReturnStruct Input::getUserInput(Field &field)
{
    Common common;
    std::string line = "";
    int beforeComma = 0;
    int afterComma = 0;
    bool isValidInput = false;
    coordsStruct coords;
    userInputReturnStruct userInput;
    
    while (true)
    {
        field.gotoXY(1, field.getOffsetY() + field.getRows()*2 + 4);
        std::cout << "'h' or 'H': Help" << nl << nl;
        std::cout << "Input: ";
        std::cout << "                              ";
        field.gotoXY(8, field.getOffsetY() + field.getRows()*2 + 6);
        std::cin.clear();
        getline(std::cin, line);
        if (line == "")
            isValidInput = false;
        else if (line == "q" || line == "Q")
            exit (0);
        else if (line == "h" || line == "H")
        {
            common.clearScreen();
            field.printExplanation();
            getAnyKey();
            field.printAll();
            continue;
        }
        else
        {
            if(line.find(",") != std::string::npos && line.find("f") == std::string::npos)
            {
                isValidInput = true;
                try
                {
                    beforeComma = stoi(line.substr(0, line.find(",")));
                }
                catch (...)
                {
                    isValidInput = false;
                }
                try
                {
                    afterComma = stoi(line.substr(line.find(",") + 1));
                }
                catch (...)
                {
                    try
                    {
                        afterComma = stoi(line.substr(line.find(",")));
                    }
                    catch (...)
                    {
                        isValidInput = false;
                    }
                }
                coords.col = beforeComma;
                coords.row = afterComma;
            }
            else if (line.find(",") != std::string::npos && line.find("f") != std::string::npos)
            {
                isValidInput = true;
                try
                {
                    beforeComma = stoi(line.substr(1, line.find(",")));
                }
                catch (...)
                {
                    isValidInput = false;
                }
                try
                {
                    afterComma = stoi(line.substr(line.find(",") + 1));
                }
                catch (...)
                {
                    try
                    {
                        afterComma = stoi(line.substr(line.find(",")));
                    }
                    catch (...)
                    {
                        isValidInput = false;
                    }
                }
                coords.col = beforeComma;
                coords.row = afterComma;
                if (coords.col > field.getCols() || coords.row > field.getRows())
                    isValidInput = false;
                else
                    if (field.isNumber(coords) != true)
                        userInput.isFlag = true;
                    else
                        isValidInput = false;
            }     
        }
        if (coords.col > field.getCols() || coords.row > field.getRows())
            isValidInput = false;
        if (isValidInput == true)
        {
            userInput.coords = coords;
            return userInput;
        }
        else
        {
            std::cout << "Wrong input, Press ENTER...";
            getAnyKey();
            std::cout << "\e[A\r\e[K" << std::flush;
        }
    }
}
