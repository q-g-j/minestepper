#include <iostream>
#include <string>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
    #include <stdio.h>
#endif

#include "common.hpp"
#include "field.hpp"
#include "common.hpp"
#include "input.hpp"

void Input::getEnterKey(std::string wrongInputText)
{
    std::string line;
    while (true)
    {
        std::cout << wrongInputText;
        getline(std::cin, line);
        if (line == "")
            break;
        else
        {
            deleteLastLine(wrongInputText.length() + line.length());
            continue;
        }
    }
}

void Input::deleteLastLine(size_t stringLength)
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        CONSOLE_SCREEN_BUFFER_INFO cbsi;
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD cursorPosition;
        cursorPosition.X = 0;
        cursorPosition.Y = 0;

        if (GetConsoleScreenBufferInfo(console, &cbsi))
        {
            cursorPosition = cbsi.dwCursorPosition;
        }
        cursorPosition.Y--;

        SetConsoleCursorPosition(console, cursorPosition);
        std::cout << "\r";
        for (int i = 0; i < stringLength; i++)
            std::cout << " ";
        std::cout << "\r";
        std::cout << std::flush;
    #else
        std::cout << "\x1b[A";
        std::cout << "\r";
        for (int i = 0; i < stringLength; i++)
            std::cout << " ";
        std::cout << "\r";
        std::cout << std::flush;
    #endif
}

// custom mode: ask user for the game mode (difficulty):
int Input::getDifficulty()
{
    Common common;   
    std::string line = "";
    int difficulty = 0;
    bool isValidInput = false;
    std::string inputText = "Input: ";
    std::string wrongInputText = "Wrong input, Press ENTER...";

    common.clearScreen();
    std::cout << "Welcome to Minestepper - a Minesweeper clone!" << nl << nl << nl;
    std::cout << "Choose the size of the field!" << nl;
    std::cout << "(make sure, that your terminal window is large enough!)" << nl << nl;
    std::cout << "1: small" << nl;
    std::cout << "2: medium" << nl;
    std::cout << "3: large" << nl;
    std::cout << "4: custom" << nl << nl;
    std::cout << "q: quit at any time" << nl << nl;

    while (true)
    {
        std::cout << inputText;
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
            getEnterKey(wrongInputText);
            deleteLastLine(wrongInputText.length());
            deleteLastLine(inputText.length() + line.length());
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
    std::string inputText = "Input: ";
    std::string wrongInputText = "Wrong input, Press ENTER...";
    
    common.clearScreen();
    std::cout << "How large do you want the field to be (e.g. 15x10)?" << nl << nl;

    while (true)
    {        
        std::cout << inputText;
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
            getEnterKey(wrongInputText);
            deleteLastLine(wrongInputText.length());
            deleteLastLine(inputText.length() + line.length());
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
    std::string inputText = "Input: ";
    std::string wrongInputText = "Wrong input, Press ENTER...";
    
    common.clearScreen();
    std::cout << "How many mines to place on the field?" << nl << nl;

    while (true)
    {
        std::cout << inputText;
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
            getEnterKey(wrongInputText);
            deleteLastLine(wrongInputText.length());
            deleteLastLine(inputText.length() + line.length());
        }
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
    std::string inputText = "Input: ";
    std::string wrongInputText = "Wrong input, Press ENTER...";

    field.gotoXY(1, field.getOffsetY() + field.getRows() * 2 + 4);
    std::cout << "'h' or 'H': Help" << nl << nl;

    while (true)
    {
        std::cout << inputText;
        field.gotoXY(8, field.getOffsetY() + field.getRows()*2 + 6);
        getline(std::cin, line);
        if (line == "")
            isValidInput = false;
        else if (line == "q" || line == "Q")
            exit (0);
        else if (line == "h" || line == "H")
        {
            common.clearScreen();
            field.printExplanation();
            getEnterKey("");
            field.printAll();            
            field.gotoXY(1,3);
            std::cout << field.getMinesLeft() << " Mines left...";
            #if DEBUG == 1
                std::cout << " DEBUG: Turn: " << turn;
            #endif
            field.gotoXY(1, field.getOffsetY() + field.getRows()*2 + 4);
            std::cout << "'h' or 'H': Help" << nl << nl;
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
            deleteLastLine(inputText.length() + line.length());
            return userInput;
        }
        else
        {
            getEnterKey(wrongInputText);
            deleteLastLine(wrongInputText.length());
            deleteLastLine(inputText.length() + line.length());
        }
    }
}
