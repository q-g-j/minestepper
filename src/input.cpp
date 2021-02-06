#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "../include/colors.hpp"
#include "../include/common.hpp"
#include "../include/debug.hpp"
#include "../include/field.hpp"
#include "../include/common.hpp"
#include "../include/input.hpp"
#include "../include/print.hpp"
#include "../include/symbols.hpp"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <conio.h>
    #include <stdio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

// Linux: need to enable "non canonical mode" to make arrow keys and SPACE work (no need to press ENTER):
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
    Colors colors;
    Print print;

    std::cout << colors.setTextColor(colors.fg_white);
    std::cout << text << std::flush;
    std::cout << colors.setTextColor(colors.color_default);

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        while (true)
        {
            int inputKey = 0;
            if ((inputKey = _getch()) == KEY_ENTER)
            {
                break;
            }
            else
            {
                continue;
            }
        }
    #else
        enableNonCanonicalMode();
        char inputKey = ' ';
        while (read(STDIN_FILENO, &inputKey, 1) == 1)
        {
            if (inputKey == KEY_ENTER)
            {
                break;
            }
            else
            {
                continue;
            }
        }
        disableNonCanonicalMode();
    #endif
    std::cout << newline;
}

// disable the input cursor during game play:
void Input::showCursor(bool show)
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_CURSOR_INFO cursorInfo;
        cursorInfo.dwSize = 1;

        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = show; // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);
    #else
        if (show == true)
        {
            coutconv << "\e[?25h";
        }
        else
        {
            coutconv << "\e[?25l";
        }
    #endif
        std::cout << std::flush;
}

// move the players cursor in 4 directions with the arrow keys:
void Input::moveCursor(Field &field, Common::CoordsStruct& currentArrayPosition, Direction &direction)
{
    Common common;
    Symbols symbols;

    Common::CoordsStruct currentCursorPosition;
    std::wcout << L"\b";
    field.printCoords(currentArrayPosition, false);
    if (direction == Direction::UP)
    {
        --currentArrayPosition.row;
    }
    else if (direction == Direction::DOWN)
    {
        ++currentArrayPosition.row;
    }
    else if (direction == Direction::LEFT)
    {
        --currentArrayPosition.col;
    }
    else if (direction == Direction::RIGHT)
    {
        ++currentArrayPosition.col;
    }
    currentCursorPosition = common.coordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
    field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
    coutconv << symbols.symbolCursor << std::flush;
    if (field.getCoordsContent(currentArrayPosition) == symbols.symbolFlag || field.isNumber(currentArrayPosition))
    {
        field.printCoords(currentArrayPosition, true);
    }
}

// custom mode: ask user for the game mode (difficulty):
int Input::getDifficulty()
{
    Common common;
    Print print;

    int difficulty = 0;

    print.printMenu();

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        while (true)
        {
            int inputKey = 0;
            if ((inputKey = _getch()) == 'q' || inputKey == 'Q')
            {
                common.clearScreen();
                showCursor(true);
                exit (0);
            }
            else if (inputKey == '1')
            {
                difficulty = 1;
                break;
            }
            else if (inputKey == '2')
            {
                difficulty = 2;
                break;
            }
            else if (inputKey == '3')
            {
                difficulty = 3;
                break;
            }
            else if (inputKey == '4')
            {
                difficulty = 4;
                break;
            }
            else
            {
                continue;
            }
        }
    #else
        enableNonCanonicalMode();
        char inputKey = ' ';
        while (read(STDIN_FILENO, &inputKey, 1) == 1)
        {
            if (inputKey == 'q' || inputKey == 'Q')
            {
                common.clearScreen();
                showCursor(true);
                exit (0);
            }
            if (inputKey == '1')
            {
                difficulty = 1;
                break;
            }
            else if (inputKey == '2')
            {
                difficulty = 2;
                break;
            }
            else if (inputKey == '3')
            {
                difficulty = 3;
                break;
            }
            else if (inputKey == '4')
            {
                difficulty = 4;
                break;
            }
            else
            {
                continue;
            }
        }
        disableNonCanonicalMode();
    #endif
    return difficulty;
}

// custom mode: ask user for the size of the field:
Common::CoordsStruct Input::getDimensions()
{
    Colors colors;
    Common common;
    Print print;

    Common::CoordsStruct dimensions;
    std::string line = "";
    int beforeX = 0;
    int afterX = 0;
    bool isValidInput = false;

    print.printCustomGetDimensions();

    while (true)
    {
        std::cout << colors.setTextColor(colors.fg_white);
        std::cout << print.inputText;
        std::cout << colors.setTextColor(colors.color_default);
        getline(std::cin, line);
        if (line == "")
        {
            isValidInput = false;
        }
        if (line == "q" || line == "Q")
        {
            common.clearScreen();
            showCursor(true);
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
            if (beforeX < 5 || afterX < 5 || beforeX > 26 || afterX > 20)
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
            print.deleteLastLine(print.wrongInputText.length());
            print.deleteLastLine(print.inputText.length() + line.length());
        }
    }
}

// custom mode: ask user for the number of mines:
int Input::getMinesCount(int const& fieldSize)
{
    Colors colors;
    Common common;
    Print print;

    std::string line = "";
    int minesTotal = 0;
    bool isValidInput = false;

    common.clearScreen();
    print.printCustomGetMinesCount();

    while (true)
    {
        std::cout << colors.setTextColor(colors.fg_white);
        std::cout << print.inputText;
        std::cout << colors.setTextColor(colors.color_default);
        getline(std::cin, line);
        if (line == "")
        {
            isValidInput = false;
        }
        if (line == "q" || line == "Q")
        {
            common.clearScreen();
            showCursor(true);
            exit (0);
        }
        else
        {
            try
            {
                minesTotal = stoi(line);
            }
            catch (...)
            {
                isValidInput = false;
            }
            if (minesTotal > 0 && minesTotal < fieldSize)
            {
                isValidInput = true;
            }
            else
            {
                isValidInput = false;
            }
        }
        if (isValidInput == true)
        {
            return minesTotal;
        }
        else
        {
            getEnterKey(print.wrongInputText);
            print.deleteLastLine(print.wrongInputText.length());
            print.deleteLastLine(print.inputText.length() + line.length());
        }
    }
}

void Input::helpToggle(Field &field, Common::CoordsStruct const& currentArrayPosition)
{
    Colors colors;
    Common common;
    Print print;
    Symbols symbols;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common.resizeConsole(105, 24);
        common.centerWindow();
        showCursor(false);
    #endif

    common.setUnicode(false);
    Common::CoordsStruct currentCursorPosition;
    common.clearScreen();
    print.printExplanation();
    getEnterKey("");

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common.resizeConsole(field.getOffsetX() + (field.getCols() * 4) + field.getOffsetX() - 2, field.getOffsetY() + (field.getRows() * 2) + 5);
        common.centerWindow();
    #endif

    common.clearScreen();
    print.printTitle(field.getDifficultyString(), field.getCols(), field.getRows(), field.getMinesCount());
    field.drawField(true);
    std::cout << newline;
    field.gotoXY(field.getOffsetX() - 1, field.getOffsetY() - 2);
    std::cout << colors.setTextColor(colors.fg_light_red);
    std::cout << field.getMinesLeft() << print.minesLeftText << std::flush;
    std::cout << colors.setTextColor(colors.color_default);
    field.gotoXY(field.getOffsetX() - 1, field.getOffsetY() + field.getRows()*2);
    std::cout << colors.setTextColor(colors.fg_white);
    std::cout << print.getHelpText << newline << newline;
    std::cout << colors.setTextColor(colors.color_default);
    currentCursorPosition = common.coordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
    field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);
    common.setUnicode(true);
    if (field.getCoordsContent(currentArrayPosition) == symbols.symbolFlag || field.isNumber(currentArrayPosition))
    {
        field.printCoords(currentArrayPosition, true);
    }
    else
    {
        coutconv << symbols.symbolCursor << std::flush;
    }
}

// the main function to get the users input during a game:
Common::UserInputReturnStruct Input::getUserInput(Field &field, int firstrun)
{
    Colors colors;
    Common common;
    Print print;
    Symbols symbols;

    char inputKey;
    static Common::CoordsStruct currentArrayPosition;
    Common::CoordsStruct currentCursorPosition;
    Common::UserInputReturnStruct userInput;

    #if !defined(_WIN32) && !defined(WIN32) && !defined(_WIN64) && !defined(WIN64)
        enableNonCanonicalMode();
    #endif

    field.gotoXY(field.getOffsetX() - 1, field.getOffsetY() + field.getRows() * 2);
    std::cout << colors.setTextColor(colors.fg_white);
    std::cout << print.getHelpText << newline << newline;
    std::cout << colors.setTextColor(colors.color_default);

    if (firstrun == 1)
    {
        if (field.getCols() % 2 == 0)
        {
            currentArrayPosition.col = (field.getCols()) / 2;
        }
        else
        {
            currentArrayPosition.col = int(field.getCols()/ 2) + 1;
        }
        if (field.getRows() % 2 == 0)
        {
            currentArrayPosition.row = (field.getRows()) / 2;
        }
        else
        {
            currentArrayPosition.row = int(field.getRows()/ 2) + 1;
        }
    }

    currentCursorPosition = common.coordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
    field.gotoXY(currentCursorPosition.col, currentCursorPosition.row);

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common.setUnicode(true);

        if (field.getCoordsContent(currentArrayPosition) == symbols.symbolFlag || field.isNumber(currentArrayPosition))
        {
            field.printCoords(currentArrayPosition, true);
        }
        else
        {
            coutconv << symbols.symbolCursor << std::flush;
        }

        while(1)
        {
            int inputTmp = 0;
            inputKey = 0;
            if ((inputTmp = _getch()) == 224)
            {
                inputKey = _getch();
                if (inputKey == KEY_UP)
                {
                    if (currentArrayPosition.row > 1)
                    {
                        Direction direction = Direction::UP;
                        moveCursor(field, currentArrayPosition, direction);
                    }
                }
                else if (inputKey == KEY_DOWN)
                {
                    if (currentArrayPosition.row < field.getRows())
                    {
                        Direction direction = Direction::DOWN;
                        moveCursor(field, currentArrayPosition, direction);
                    }
                }
                else if (inputKey == KEY_LEFT)
                {
                    if (currentArrayPosition.col > 1)
                    {
                        Direction direction = Direction::LEFT;
                        moveCursor(field, currentArrayPosition, direction);
                    }
                }
                else if (inputKey == KEY_RIGHT)
                {
                    if (currentArrayPosition.col < field.getCols())
                    {
                        Direction direction = Direction::RIGHT;
                        moveCursor(field, currentArrayPosition, direction);
                    }
                }
            }
            else if (inputTmp == 'q' || inputTmp == 'Q')
            {
                common.clearScreen();
                showCursor(true);
                exit (0);
            }
            else if (inputTmp == 'h' || inputTmp == 'H')
            {
                helpToggle(field, currentArrayPosition);
                continue;
            }
            else if (inputTmp == KEY_ENTER)
            {
                coutconv << L"\b" << std::flush;
                if (field.getCoordsContent(currentArrayPosition) == symbols.symbolFlag)
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            else if  (inputTmp == KEY_SPACE)
            {
                coutconv << L"\b" << std::flush;
                if (field.isNumber(currentArrayPosition) || field.getCoordsContent(currentArrayPosition) == L" ")
                {
                    continue;
                }
                else
                {
                    userInput.isFlag = true;
                }
                break;
            }
            else
            {
                continue;
            }
        }
        common.setUnicode(false);

    #else
        if (field.getCoordsContent(currentArrayPosition) == symbols.symbolFlag || field.isNumber(currentArrayPosition))
        {
            field.printCoords(currentArrayPosition, true);
        }
        else
        {
            std::cout << symbols.symbolCursor << std::flush;
        }
        while (read(STDIN_FILENO, &inputKey, 1) == 1)
        {
            if (inputKey == 'q' || inputKey == 'Q')
            {
                common.clearScreen();
                showCursor(true);
                exit (0);
            }
            else if (inputKey == 'h' || inputKey == 'H')
            {
                helpToggle(field, currentArrayPosition);
                continue;
            }
            else if (inputKey == KEY_ENTER)
            {
                std::cout << "\b" << std::flush;
                if (field.getCoordsContent(currentArrayPosition) == symbols.symbolFlag)
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            else if (inputKey == KEY_SPACE)
            {
                std::cout << "\b" << std::flush;
                if (field.isNumber(currentArrayPosition) || field.getCoordsContent(currentArrayPosition) == " ")
                {
                    continue;
                }
                else;
                userInput.isFlag = true;
                break;
            }
            else if (inputKey == KEY_UP)
            {
                if (currentArrayPosition.row > 1)
                {
                    Direction direction = Direction::UP;
                    moveCursor(field, currentArrayPosition, direction);
                }
            }
            else if (inputKey == KEY_DOWN)
            {
                if (currentArrayPosition.row < field.getRows())
                {
                    Direction direction = Direction::DOWN;
                    moveCursor(field, currentArrayPosition, direction);
                }
            }
            else if (inputKey == KEY_LEFT)
            {
                if (currentArrayPosition.col > 1)
                {
                    Direction direction = Direction::LEFT;
                    moveCursor(field, currentArrayPosition, direction);
                }
            }
            else if (inputKey == KEY_RIGHT)
            {
                if (currentArrayPosition.col < field.getCols())
                {
                    Direction direction = Direction::RIGHT;
                    moveCursor(field, currentArrayPosition, direction);
                }
            }
            else
                continue;
        }
    #endif

    userInput.Coords.col = currentArrayPosition.col;
    userInput.Coords.row = currentArrayPosition.row;

    #if !defined(_WIN32) && !defined(WIN32) && !defined(_WIN64) && !defined(WIN64)
        disableNonCanonicalMode();
    #endif

    ++firstrun;

    return userInput;
}
