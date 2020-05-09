#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <conio.h>
    #include <stdio.h>
    #include <windows.h>
#else
    #include <termios.h>
#include <unistd.h>
#endif

#include "common.hpp"
#include "field.hpp"
#include "common.hpp"
#include "input.hpp"
#include "print.hpp"

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
void Input::showCursor(bool showFlag)
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_CURSOR_INFO     cursorInfo;

        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = showFlag; // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);
    #else
        if (showFlag == true)
            coutconv << "\e[?25h";
        else
            coutconv << "\e[?25l";
    #endif
}

void Input::moveCursor(Field &field, coordsStruct& CurrentArrayPosition, Direction &direction)
{
    Common common;
    coordsStruct currentCursorPosition;
    std::wcout << L"\b";
    field.printCoords(CurrentArrayPosition, false);
    if (direction == Direction::UP)
        CurrentArrayPosition.row--;
    else if (direction == Direction::DOWN)
        CurrentArrayPosition.row++;
    else if (direction == Direction::LEFT)
        CurrentArrayPosition.col--;
    else if (direction == Direction::RIGHT)
        CurrentArrayPosition.col++;
    currentCursorPosition = common.convCoordsToCursorPosition(CurrentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
    field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
    coutconv << field.symbolCursor << std::flush;
    if (field.getCoordsContent(CurrentArrayPosition) == field.symbolFlag || field.isNumber(CurrentArrayPosition))
        field.printCoords(CurrentArrayPosition, true);
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
    Print print;
    
    std::string line = "";
    int difficulty = 0;
    bool isValidInput = false;

    common.clearScreen();
    print.printMenu();

    while (true)
    {
        std::cout << print.inputText;
        getline(std::cin, line);
        if (line == "")
            isValidInput = false;
        if (line == "q" || line == "Q")
        {
            common.clearScreen();
            exit(0);
        }
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
            getEnterKey(print.wrongInputText);
            deleteLastLine(print.wrongInputText.length());
            deleteLastLine(print.inputText.length() + line.length());
        }
    }
}

// custom mode: ask user for the size of the field:
coordsStruct Input::getDimensions()
{
    Common common;
    Print print;
    
    coordsStruct dimensions;
    std::string line = "";
    int beforeX = 0;
    int afterX = 0;
    bool isValidInput = false;
    
    common.clearScreen();
    print.printCustomGetDimensions();

    while (true)
    {        
        std::cout << print.inputText;
        getline(std::cin, line);
        if (line == "")
            isValidInput = false;
        if (line == "q" || line == "Q")
        {
            common.clearScreen();
            exit(0);
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
            getEnterKey(print.wrongInputText);
            deleteLastLine(print.wrongInputText.length());
            deleteLastLine(print.inputText.length() + line.length());
        }
    }
}

// custom mode: ask user for the number of mines:
int Input::getMinesCount(int const& fieldSize)
{
    Common common;
    Print print;
    
    std::string line = "";
    int minesCount = 0;
    bool isValidInput = false;
    
    common.clearScreen();
    print.printCustomGetMinesCount();

    while (true)
    {
        std::cout << print.inputText;
        getline(std::cin, line);
        if (line == "")
            isValidInput = false;
        if (line == "q" || line == "Q")
        {
            common.clearScreen();
            exit(0);
        }
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
            getEnterKey(print.wrongInputText);
            deleteLastLine(print.wrongInputText.length());
            deleteLastLine(print.inputText.length() + line.length());
        }
    }
}

// the main function to ask the user for valid coordinates:
userInputReturnStruct Input::getUserInput(Field &field, int firstrun)
{
    Common common;
    Print print;
    
    char inputKey;
    static coordsStruct CurrentArrayPosition;
    coordsStruct CurrentCursorPosition;
    userInputReturnStruct UserInput;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    #else
        enableNonCanonicalMode();    
    #endif
    showCursor(false);

    field.gotoXY(field.getOffsetX() - 1, field.getOffsetY() + field.getRows() * 2);
    std::cout << print.getHelpText << newline << newline;
    
    if (firstrun == 1)
    {
        if (field.getCols() % 2 == 0)
            CurrentArrayPosition.col = (field.getCols()) / 2;
        else
            CurrentArrayPosition.col = int(field.getCols()/ 2) + 1;
        if (field.getRows() % 2 == 0)
            CurrentArrayPosition.row = (field.getRows()) / 2;
        else
            CurrentArrayPosition.row = int(field.getRows()/ 2) + 1;
    }
    
    CurrentCursorPosition = common.convCoordsToCursorPosition(CurrentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());    
    field.gotoXY(CurrentCursorPosition.col, CurrentCursorPosition.row);
    
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    common.setUnicode(true);

    if (field.getCoordsContent(CurrentArrayPosition) == field.symbolFlag || field.isNumber(CurrentArrayPosition))
        field.printCoords(CurrentArrayPosition, true);
    else
        coutconv << field.symbolCursor << std::flush;

    while(1)
    {
        int inputTmp = 0;
        inputKey = 0;
        if ((inputTmp = _getch()) == 224)
        {
            inputKey = _getch();
            if (inputKey == KEY_UP)
            {
                if (CurrentArrayPosition.row > 1)
                {
                    Direction direction = Direction::UP;
                    moveCursor(field, CurrentArrayPosition, direction);
                }
            }
            else if (inputKey == KEY_DOWN)
            {
                if (CurrentArrayPosition.row < field.getRows())
                {
                    Direction direction = Direction::DOWN;
                    moveCursor(field, CurrentArrayPosition, direction);
                }
            }
            else if (inputKey == KEY_LEFT)
            {
                if (CurrentArrayPosition.col > 1)
                {
                    Direction direction = Direction::LEFT;
                    moveCursor(field, CurrentArrayPosition, direction);
                }
            }
            else if (inputKey == KEY_RIGHT)
            {
                if (CurrentArrayPosition.col < field.getCols())
                {
                    Direction direction = Direction::RIGHT;
                    moveCursor(field, CurrentArrayPosition, direction);
                }
            }
        }
        else if (inputTmp == 'q' || inputTmp == 'Q')
        {
            common.clearScreen();
            exit(0);
        }
        else if (inputTmp == 'h' || inputTmp == 'H')
        {
            print.printHelp(field, CurrentArrayPosition);
            continue;
        }
        else if (inputTmp == KEY_ENTER)
        {
            coutconv << L"\b" << std::flush;
            if (field.getCoordsContent(CurrentArrayPosition) == field.symbolFlag)
            {
                continue;
            }
            else
                break;
        }
        else if  (inputTmp == KEY_SPACE)
        {
            coutconv << L"\b" << std::flush;
            if (field.isNumber(CurrentArrayPosition) || field.getCoordsContent(CurrentArrayPosition) == L" ")
                continue;
            else
                UserInput.isFlag = true;
            break;
        }
        else
            continue;
    }
    common.setUnicode(false);

    #else
    if (field.getCoordsContent(CurrentArrayPosition) == field.symbolFlag || field.isNumber(CurrentArrayPosition))
        field.printCoords(CurrentArrayPosition, true);
    else
        std::cout << field.symbolCursor << std::flush;
    while (read(STDIN_FILENO, &inputKey, 1) == 1)
    {
        if (inputKey == 'q' || inputKey == 'Q')
        {
            common.clearScreen();
            exit(0);
        }
        else if (inputKey == 'h' || inputKey == 'H')
        {
            print.printHelp(field, CurrentArrayPosition);
            continue;
        }
        else if (inputKey == KEY_ENTER)
        {
            std::cout << "\b" << std::flush;
            if (field.getCoordsContent(CurrentArrayPosition) == field.symbolFlag)
                continue;
            else
                break;
        }
        else if (inputKey == KEY_SPACE)
        {
            std::cout << "\b" << std::flush;
            if (field.isNumber(CurrentArrayPosition) || field.getCoordsContent(CurrentArrayPosition) == " ")
                continue;
            else;
            UserInput.isFlag = true;
            break;
        }
        else if (inputKey == KEY_UP)
        {
            if (CurrentArrayPosition.row > 1)
            {
                Direction direction = Direction::UP;
                moveCursor(field, CurrentArrayPosition, direction);
            }
        }
        else if (inputKey == KEY_DOWN)
        {
            if (CurrentArrayPosition.row < field.getRows())
            {
                Direction direction = Direction::DOWN;
                moveCursor(field, CurrentArrayPosition, direction);
            }
        }
        else if (inputKey == KEY_LEFT)
        {
            if (CurrentArrayPosition.col > 1)
            {
                Direction direction = Direction::LEFT;
                moveCursor(field, CurrentArrayPosition, direction);
            }
        }
        else if (inputKey == KEY_RIGHT)
        {
            if (CurrentArrayPosition.col < field.getCols())
            {
                Direction direction = Direction::RIGHT;
                moveCursor(field, CurrentArrayPosition, direction);
            }
        }
        else
            continue;
    }
    #endif
    
    UserInput.Coords.col = CurrentArrayPosition.col;
    UserInput.Coords.row = CurrentArrayPosition.row;
    
    showCursor(true);
    #if !defined(_WIN32) && !defined(WIN32) && !defined(_WIN64) && !defined(WIN64)
        disableNonCanonicalMode();
    #endif
    
    firstrun++;

    return UserInput;
}
