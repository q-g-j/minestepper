#include <iostream>

#include "common.hpp"
#include "input.hpp"

Common common;

int Input::getDifficulty()
{   
    std::string line = "";
    int difficulty = 0;
    bool isValidInput = false;
   
    while (true)
    {
        common.clearScreen();
        std::cout << "Choose the size of the board!" << nl;
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

Input::dimensionsStruct Input::getDimensions()
{
    dimensionsStruct dimensions;
    std::string line = "";
    int beforeX = 0;
    int afterX = 0;
    bool isValidInput = false;
    
    while (true)
    {
        common.clearScreen();
        std::cout << "How large do you want the board to be?" << nl;
        std::cout << "(e.g. 15x10): ";
        getline(std::cin, line);
        if (line == "")
        {
            dimensions.rows = 0;
            dimensions.cols = 0;
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
                    dimensions.rows = 0;
                    dimensions.cols = 0;
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
                        dimensions.rows = 0;
                        dimensions.cols = 0;
                        isValidInput = false;
                    }
                    dimensions.rows = 0;
                    dimensions.cols = 0;
                    isValidInput = false;
                }
            }
            else
            {
                dimensions.rows = 0;
                dimensions.cols = 0;
                isValidInput = false;
            }
            
            if (beforeX < 5 || afterX < 5 || beforeX > 25 || afterX > 25)
            {
                dimensions.rows = 0;
                dimensions.cols = 0;
                isValidInput = false;
            }
            
            dimensions.cols = beforeX;
            dimensions.rows = afterX;
            isValidInput = true;
        }
        if (isValidInput == true)
            return dimensions;
        else
        {
            std::cout << "Wrong input, Press ENTER... ";
            getAnyKey();
        }
    }
}

int Input::getBombsCount(int boardSize)
{
    std::string line = "";
    int bombsCount = 0;
    bool isValidInput = false;
    
    while (true)
    {
        common.clearScreen();
        std::cout << "How many bombs to place on the board? ";
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
            if (bombsCount > 1 && bombsCount < boardSize)
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
