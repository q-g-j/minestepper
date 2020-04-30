#include <iostream>

#include "common.hpp"
#include "debug.hpp"
#include "field.hpp"
#include "common.hpp"
#include "input.hpp"

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
            catch (std::exception &err)
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
        }
    }
}

Common::coordsStruct Input::getDimensions()
{
    Common common;
    Common::coordsStruct dimensions;
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
                catch (std::exception &err)
                {
                    isValidInput = false;
                }
                try
                {
                    afterX = stoi(line.substr(line.find("x") + 1));
                }
                catch (std::exception &err)
                {
                    try
                    {
                        afterX = stoi(line.substr(line.find("x")));
                    }
                    catch (std::exception &err)
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
        }
    }
}

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
            catch (std::exception &err)
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

Common::userInputStruct Input::getUserInput(Field &field)
{
    Common common;
    std::string line = "";
    int beforeComma = 0;
    int afterComma = 0;
    bool isValidInput = false;
    Common::coordsStruct coords;
    Common::userInputStruct userInput;
    
    while (true)
    {
        std::cout << "'h' or 'H': Help" << nl << nl;
        std::cout << "Input: ";
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
                catch (std::exception &err)
                {
                    isValidInput = false;
                }
                try
                {
                    afterComma = stoi(line.substr(line.find(",") + 1));
                }
                catch (std::exception &err)
                {
                    try
                    {
                        afterComma = stoi(line.substr(line.find(",")));
                    }
                    catch (std::exception &err)
                    {
                        isValidInput = false;
                    }
                    isValidInput = false;
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
                catch (std::exception &err)
                {
                    isValidInput = false;
                }
                try
                {
                    afterComma = stoi(line.substr(line.find(",") + 1));
                }
                catch (std::exception &err)
                {
                    try
                    {
                        afterComma = stoi(line.substr(line.find(",")));
                    }
                    catch (std::exception &err)
                    {
                        isValidInput = false;
                    }
                    isValidInput = false;
                }
                coords.col = beforeComma;
                coords.row = afterComma;
                if (field.isNumber(coords) != true)
                    userInput.isFlag = true;
                else
                    isValidInput = false;
            }
        }
        if (isValidInput == true)
        {
            userInput.coords = coords;
            return userInput;
        }
        else
        {
            std::cout << "Wrong input, Press ENTER... ";
            getAnyKey();
            field.printAll();
        }
    }
}
