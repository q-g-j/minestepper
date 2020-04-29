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
        std::cout << "Choose the size of the field!" << nl;
        std::cout << "('1' for small, '2' for medium, '3' for large or '4' for custom): ";
        getline(std::cin, line);
        if (line == "")
            isValidInput = false;
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
        std::cout << "How large do you want the field to be?" << nl;
        std::cout << "(e.g. 15x10): ";
        getline(std::cin, line);
        if (line == "")
        {
            isValidInput = false;
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

int Input::getBombsCount(int fieldSize)
{
    Common common;
    std::string line = "";
    int bombsCount = 0;
    bool isValidInput = false;
    
    while (true)
    {
        common.clearScreen();
        std::cout << "How many bombs to place on the field? ";
        getline(std::cin, line);
        if (line == "")
            isValidInput = false;
        else
        {
            try
            {
                bombsCount = stoi(line);
            }
            catch (std::exception &err)
            {
                isValidInput = false;
            }
            if (bombsCount > 0 && bombsCount < fieldSize)
                isValidInput = true;
            else
                isValidInput = false;
        }
        if (isValidInput == true)
            return bombsCount;
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
    std::string line = "";
    int beforeComma = 0;
    int afterComma = 0;
    bool isValidInput = false;
    bool isFlag = false;
    Common::coordsStruct coords;
    Common::userInputStruct userInput;
    
    while (true)
    {
        std::cout << "Choose a position in this format: 'column,row' - e.g. 5,4" << nl;
        std::cout << "(To place or remove a flag: f5,5): ";
        getline(std::cin, line);
        if (line == "")
            isValidInput = false;
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
                isFlag = true;
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
            }
        }
        if (field.isFree(coords) == false)
        {
            std::cout << "Position not free, Press ENTER... ";
            getAnyKey();
            field.printAll();
        }
        else if (isValidInput == true)
        {
            userInput.coords = coords;
            userInput.isFlag = isFlag;
            return userInput;
        }
        else
        {
            isFlag = false;
            std::cout << "Wrong input, Press ENTER... ";
            getAnyKey();
            field.printAll();
        }
    }
}
