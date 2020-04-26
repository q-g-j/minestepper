#include <iostream>

#include "common.hpp"
#include "input.hpp"

int getDifficulty()
{   
    std::string line;
    int difficulty;
    std::cout << "Choose the size of the board!" << nl;
    std::cout << "('1' for small, '2' for medium, '3' for large or '4' for custom): ";
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
        if (difficulty <= 4 && difficulty >= 1)
            return difficulty;
        else
            return 0;
    }
}

dimensionsStruct getDimensions()
{
    dimensionsStruct dimensions;
    std::string line;
    unsigned int beforeX;
    unsigned int afterX;
    std::cout << "How large do you want the board to be?" << nl;
    std::cout << "(e.g. 15x10): ";
    getline(std::cin, line);
    if (line == "")
    {
        dimensions.rows = 0;
        dimensions.cols = 0;
        return dimensions;
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
                return dimensions;
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
                    return dimensions;
                }
                dimensions.rows = 0;
                dimensions.cols = 0;
                return dimensions;
            }
        }
        else
        {
            dimensions.rows = 0;
            dimensions.cols = 0;
            return dimensions;
        }
        
        if (beforeX < 5 || afterX < 5 || beforeX > 25 || afterX > 25)
        {
            dimensions.rows = 0;
            dimensions.cols = 0;
            return dimensions;
        }
        
        dimensions.cols = beforeX;
        dimensions.rows = afterX;
        return dimensions;
    }
}

int getBombsCount(int boardSize)
{
    std::string line;
    int bombsCount;
    std::cout << "How many bombs to place on the board? ";
    getline(std::cin, line);
    if (line == "")
        return 0;
    else
    {
        try
        {
            bombsCount = stoi(line);
        }
        catch (std::exception &err)
        {
            return 0;
        }
        if (bombsCount > 1 && bombsCount < boardSize)
            return bombsCount;
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
