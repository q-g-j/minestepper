#include <iostream>
#include <math.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <conio.h>
    #include <stdio.h>
    #include <windows.h>
#else
    #include <termios.h>
#endif

#include "common.hpp"
#include "field.hpp"
#include "common.hpp"
#include "input.hpp"

#if !defined(_WIN32) && !defined(WIN32) && !defined(_WIN64) && !defined(WIN64)
    struct termios orig_termios;

    void enableNonCanonicalMode()
    {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableNonCanonicalMode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    }

    void disableNonCanonicalMode()
    {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    }

#endif

void Input::getEnterKey(std::string const& text)
{
    std::string line;
    while (true)
    {
        std::cout << text;
        getline(std::cin, line);
        if (line == "")
            break;
        else
        {
            deleteLastLine(text.length() + line.length());
            continue;
        }
    }
}

void Input::hideCursor()
{
    coutconv << "\e[?25l";
}

void Input::showCursor()
{
    coutconv << "\e[?25h";
}

void Input::deleteLastLine(size_t const& stringLength)
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
        for (unsigned int i = 0; i < stringLength; i++)
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
    std::cout << "Welcome to Minestepper - A Minesweeper Clone!" << newline << newline << newline;
    std::cout << "Choose the size of the field!" << newline;
    std::cout << "(make sure, that your terminal window is large enough!)" << newline << newline;
    std::cout << "1: small" << newline;
    std::cout << "2: medium" << newline;
    std::cout << "3: large" << newline;
    std::cout << "4: custom" << newline << newline;
    std::cout << "q: quit at any time" << newline << newline;

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
    std::cout << "How large do you want the field to be (e.g. 15x10)?" << newline << newline;

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
int Input::getMinesCount(int const& fieldSize)
{
    Common common;
    std::string line = "";
    int minesCount = 0;
    bool isValidInput = false;
    std::string inputText = "Input: ";
    std::string wrongInputText = "Wrong input, Press ENTER...";
    
    common.clearScreen();
    std::cout << "How many mines to place on the field?" << newline << newline;

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
userInputReturnStruct Input::getUserInput(Field &field, int firstrun)
{
    Common common;
    char input;
    static coordsStruct currentArrayPosition;
    coordsStruct currentCursorPosition;
    userInputReturnStruct userInput;
    std::string inputText = "Input: ";
    std::string wrongInputText = "Wrong input, Press ENTER...";
    
    #if !defined(_WIN32) && !defined(WIN32) && !defined(_WIN64) && !defined(WIN64)
        enableNonCanonicalMode();    
    #endif
    hideCursor();

    field.gotoXY(1, field.getOffsetY() + field.getRows() * 2 + 1);
    std::cout << "'h' or 'H': Help" << newline << newline;
    
    if (firstrun == 1)
    {
        if (field.getCols() % 2 == 0)
            currentArrayPosition.col = (field.getCols()) / 2;
        else
            currentArrayPosition.col = int(field.getCols()/ 2) + 1;
        if (field.getRows() % 2 == 0)
            currentArrayPosition.row = (field.getRows()) / 2;
        else
            currentArrayPosition.row = int(field.getRows()/ 2) + 1;
    }
    
    currentCursorPosition = common.convCoordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
    
    field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
    
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    common.setUnicode(true);
    coutconv << symbolCursor << std::flush;
    while(1)
    {
        int inputTmp = 0;
        input = 0;
        if ((inputTmp = _getch()) == 224)
        {
            input = _getch();
            if (input == KEY_UP)
            {
                if (currentArrayPosition.row > 1)
                {
                    coutconv << L"\b";
                    field.printCoords(currentArrayPosition);
                    currentArrayPosition.row--;
                    currentCursorPosition = common.convCoordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
                    field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
                    coutconv << symbolCursor << std::flush;
                }
            }
            else if (input == KEY_DOWN)
            {
                if (currentArrayPosition.row < field.getRows())
                {
                    coutconv << L"\b";
                    field.printCoords(currentArrayPosition);
                    currentArrayPosition.row++;
                    currentCursorPosition = common.convCoordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
                    field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
                    coutconv << symbolCursor << std::flush;
                }
            }
            else if (input == KEY_LEFT)
            {
                if (currentArrayPosition.col > 1)
                {
                    coutconv << L"\b";
                    field.printCoords(currentArrayPosition);
                    currentArrayPosition.col--;
                    currentCursorPosition = common.convCoordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
                    field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
                    coutconv << symbolCursor << std::flush;
                }
            }
            else if (input == KEY_RIGHT)
            {
                if (currentArrayPosition.col < field.getCols())
                {
                    coutconv << L"\b";
                    field.printCoords(currentArrayPosition);
                    currentArrayPosition.col++;
                    currentCursorPosition = common.convCoordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
                    field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
                    coutconv << symbolCursor << std::flush;
                }
            }
        }
        else if (inputTmp == 'q' || inputTmp == 'Q')
            exit(0);
        else if (inputTmp == 'h' || inputTmp == 'H')
        {
            common.clearScreen();
            field.printExplanation();
            getEnterKey("");
            field.printAll();
            field.gotoXY(1,3);
            coutconv << field.getMinesLeft() << L" Mines left...";
            #if DEBUG == 1
                coutconv << L" DEBUG: Turn: " << turn << L"      ";
            #endif
            field.gotoXY(1, field.getOffsetY() + field.getRows()*2 + 1);
            coutconv << L"'h' or 'H': Help" << newline << newline;            
            field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
            coutconv << symbolCursor << std::flush;
            continue;
        }
        else if (inputTmp == KEY_ENTER)
        {
            coutconv << L"\b" << std::flush;
            if (field.getCoordsContent(currentArrayPosition) == symbolFlag)
                continue;
            else
                break;
        }
        else if  (inputTmp == KEY_SPACE)
        {
            coutconv << L"\b" << std::flush;
            if (field.isNumber(currentArrayPosition))
                continue;
            else;
            userInput.isFlag = true;
            break;
        }
        else
            continue;
    }
    common.setUnicode(false);

    #else
    coutconv << symbolCursor << std::flush;
    while (read(STDIN_FILENO, &input, 1) == 1)
    {
        if (input == 'q' || input == 'Q')
            exit(0);
        else if (input == KEY_ENTER)
        {
            coutconv << "\b" << std::flush;
            if (field.getCoordsContent(currentArrayPosition) == symbolFlag)
                continue;
            else
                break;
        }
        else if (input == KEY_SPACE)
        {
            coutconv << "\b" << std::flush;
            if (field.isNumber(currentArrayPosition))
                continue;
            else;
            userInput.isFlag = true;
            break;
        }
        else if (input == 'h' || input == 'H')
        {
            common.clearScreen();
            field.printExplanation();
            getEnterKey("");
            field.printAll();
            field.gotoXY(1,3);
            coutconv << field.getMinesLeft() << " Mines left...";
            #if DEBUG == 1
                coutconv << " DEBUG: Turn: " << turn << "      ";
            #endif
            field.gotoXY(1, field.getOffsetY() + field.getRows()*2 + 1);
            coutconv << "'h' or 'H': Help" << newline << newline;            
            field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
            coutconv << symbolCursor << std::flush;
            continue;
        }
        else if (input == KEY_UP)
        {
            if (currentArrayPosition.row > 1)
            {
                coutconv << "\b";
                field.printCoords(currentArrayPosition);
                currentArrayPosition.row--;
                currentCursorPosition = common.convCoordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
                field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
                coutconv << symbolCursor << std::flush;
            }
        }
        else if (input == KEY_DOWN)
        {
            if (currentArrayPosition.row < field.getRows())
            {
                coutconv << "\b";
                field.printCoords(currentArrayPosition);
                currentArrayPosition.row++;
                currentCursorPosition = common.convCoordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
                field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
                coutconv << symbolCursor << std::flush;
            }
        }
        else if (input == KEY_LEFT)
        {
            if (currentArrayPosition.col > 1)
            {
                coutconv << "\b";
                field.printCoords(currentArrayPosition);
                currentArrayPosition.col--;
                currentCursorPosition = common.convCoordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
                field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
                coutconv << symbolCursor << std::flush;
            }
        }
        else if (input == KEY_RIGHT)
        {
            if (currentArrayPosition.col < field.getCols())
            {
                coutconv << "\b";
                field.printCoords(currentArrayPosition);
                currentArrayPosition.col++;
                currentCursorPosition = common.convCoordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
                field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
                coutconv << symbolCursor << std::flush;
            }
        }
        else
            continue;
    }
    #endif
    
    userInput.coords.col = currentArrayPosition.col;
    userInput.coords.row = currentArrayPosition.row;
    
    showCursor();
    #if !defined(_WIN32) && !defined(WIN32) && !defined(_WIN64) && !defined(WIN64)
        disableNonCanonicalMode();
    #endif
    
    firstrun++;

    return userInput;
}
