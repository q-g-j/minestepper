#ifdef _DEBUG
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        #define _CRTDBG_MAP_ALLOC
        #include <stdlib.h>
        #include <crtdbg.h>
        #define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
        // Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
        // allocations to be of _CLIENT_BLOCK type
    #endif
#endif

// system headers:
#include <atomic>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

// project headers:
#include <colors.hpp>
#include <common.hpp>
#include <debug.hpp>
#include <field.hpp>
#include <input.hpp>
#include <print.hpp>
#include <solver.hpp>
#include <symbols.hpp>

// Linux: need to enable "non canonical mode" to make arrow keys and SPACE work (no need to press ENTER):
#if !defined(_WIN32) && !defined(WIN32) && !defined(_WIN64) && !defined(WIN64)
    struct termios orig_termios;

    void enableNonCanonicalMode()
    {
        tcgetattr(STDIN_FILENO, &orig_termios);

        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON);

        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    }

    void disableNonCanonicalMode()
    {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    }
#endif

Input::Input()
:
    toggleEdgeJump(false),
    toogleEdgeJumpP(&toggleEdgeJump),
    colors(std::make_unique<Colors>()),
    common(std::make_unique<Common>()),
    print(std::make_unique<Print>()),
    solver(std::make_unique<Solver>()),
    symbols(std::make_unique<Symbols>())
{ }

Input::~Input() = default;

// disable the input cursor during game play:
void Input::showBlinkingCursor(bool show)
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

void Input::getInputEnterKey(std::string const& text)
{
    std::cout << colors->setTextColor(colors->fg_white);
    std::cout << text << std::flush;
    std::cout << colors->setTextColor(colors->color_default);

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        while (true)
        {
            try
            {
                int inputKey;
                if ((inputKey = _getch()) == 'q' || inputKey == 'Q')
                {
                    exit(0);
                }
                else if (inputKey == KEY_ENTER) break;
                else continue;
            }
            catch (...)
            {
                continue;
            }
        }
    #else
        enableNonCanonicalMode();
        char inputKey;
        while (true)
        {
            try
            {
                inputKey = getchar();
                if (inputKey == 'q' || inputKey == 'Q')
                {
                    exit(0);
                }
                if (inputKey == KEY_ENTER) break;
                else continue;
            }
            catch (...)
            {
                continue;
            }
        }
        disableNonCanonicalMode();
    #endif
    std::cout << newline;
}

// custom mode: ask user for the game mode (this->getInputDifficultyReturn):
const int &Input::getInputDifficulty()
{
    print->printMenu();
    this->showBlinkingCursor(false);

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        int inputKey = 0;
        while (true)
        {
            try
            {
                if ((inputKey = _getch()) == 'q' || inputKey == 'Q')
                {
                    exit(0);
                }
                else if (inputKey == '1')
                {
                    this->getInputDifficultyReturn = 1;
                    break;
                }
                else if (inputKey == '2')
                {
                    this->getInputDifficultyReturn = 2;
                    break;
                }
                else if (inputKey == '3')
                {
                    this->getInputDifficultyReturn = 3;
                    break;
                }
                else if (inputKey == '4')
                {
                    this->getInputDifficultyReturn = 4;
                    break;
                }
                else
                {
                    continue;
                }
            }
            catch (...)
            {
                continue;
            }
        }
    #else
        enableNonCanonicalMode();
        char inputKey;
        while (true)
        {
            try
            {            
                inputKey = getchar();
                if (inputKey == 'q' || inputKey == 'Q')
                {
                    exit(0);
                }
                if (inputKey == '1')
                {
                    this->getInputDifficultyReturn = 1;
                    break;
                }
                else if (inputKey == '2')
                {
                    this->getInputDifficultyReturn = 2;
                    break;
                }
                else if (inputKey == '3')
                {
                    this->getInputDifficultyReturn = 3;
                    break;
                }
                else if (inputKey == '4')
                {
                    this->getInputDifficultyReturn = 4;
                    break;
                }
                else
                {
                    continue;
                }
            }
            catch (...)
            {
                continue;
            }
        }
        disableNonCanonicalMode();
    #endif

    this->showBlinkingCursor(true);
    return this->getInputDifficultyReturn;
}

const int &Input::getInputCustomCellWidth()
{
    bool isValidInput = false;

    common->clearScreen();
    print->printCustomGetCellWidth();

    std::cout << colors->setTextColor(colors->fg_white);
    std::cout << print->inputText;
    std::cout << colors->setTextColor(colors->color_default);

    this->showBlinkingCursor(false);

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        int inputKey;
        while (true)
        {
            try
            {
                if ((inputKey = _getch()) == 'q' || inputKey == 'Q')
                {
                    exit(0);
                }
                if (inputKey == '1')
                {
                    this->getInputCustomCellWidthReturn = 1;
                    break;
                }
                else if (inputKey == '3')
                {
                    this->getInputCustomCellWidthReturn = 3;
                    break;
                }
                else
                {
                    continue;
                }
            }
            catch (...)
            {
                continue;
            }
        }
    #else
        enableNonCanonicalMode();
        char inputKey;
        while (true)
        {
            try
            {            
                inputKey = getchar();
                if (inputKey == 'q' || inputKey == 'Q')
                {
                    exit(0);
                }
                if (inputKey == '1')
                {
                    this->getInputCustomCellWidthReturn = 1;
                    break;
                }
                else if (inputKey == '3')
                {
                    this->getInputCustomCellWidthReturn = 3;
                    break;
                }
                else
                {
                    continue;
                }
            }
            catch (...)
            {
                continue;
            }
        }
        disableNonCanonicalMode();
    #endif

    this->showBlinkingCursor(true);
    return this->getInputCustomCellWidthReturn;
}

// custom mode: ask user for the size of the field:
const Common::CoordsStruct &Input::getInputCustomDimensions(int const& fieldCellWidth)
{
    std::string line = "";
    int beforeX = 0;
    int afterX = 0;
    bool isValidInput = false;

    common->clearScreen();
    print->printCustomGetDimensions(fieldCellWidth);

    while (true)
    {
        try
        {
            std::cout << colors->setTextColor(colors->fg_white);
            std::cout << print->inputText;
            std::cout << colors->setTextColor(colors->color_default);
            getline(std::cin, line);
            if (line == "q" || line == "Q")
            {
                exit(0);
            }
            else if (line == "")
            {
                isValidInput = false;
            }
            else
            {
                if(line.find("x") != std::string::npos)
                {
                    isValidInput = true;
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
                        isValidInput = false;
                        try
                        {
                            afterX = stoi(line.substr(line.find("x")));
                        }
                        catch (...)
                        {
                            isValidInput = false;
                        }
                    }
                }
                else
                {
                    isValidInput = false;
                }
                if (fieldCellWidth == 1)
                {
                    if (beforeX < 20 || afterX < 8 || beforeX > 80 || afterX > 20)
                    {
                        isValidInput = false;
                    }
                }
                else
                {
                    if (beforeX < 8 || afterX < 8 || beforeX > 40 || afterX > 20)
                    {
                        isValidInput = false;
                    }
                }
            }
        }
        catch (...)
        {
            continue;
        }
        if (isValidInput == true)
        {
            this->getInputCustomDimensionsReturn.col = beforeX;
            this->getInputCustomDimensionsReturn.row = afterX;
            return this->getInputCustomDimensionsReturn;
        }
        else
        {
            getInputEnterKey(print->wrongInputText);
            print->deleteLastLine(print->wrongInputText.length());
            print->deleteLastLine(print->inputText.length() + line.length());
        }
    }
}

// custom mode: ask user for the number of this->getInputCustomMinesCountReturn:
const int &Input::getInputCustomMinesCount(int const& fieldSize)
{
    std::string line;
    bool isValidInput = false;

    common->clearScreen();
    print->printCustomGetMinesCount();

    while (true)
    {
        try
        {
            std::cout << colors->setTextColor(colors->fg_white);
            std::cout << print->inputText;
            std::cout << colors->setTextColor(colors->color_default);
            getline(std::cin, line);
            if (line == "q" || line == "Q")
            {
                exit(0);
            }
            else if (line == "")
            {
                isValidInput = false;
            }
            else
            {
                isValidInput = true;
                try
                {
                    this->getInputCustomMinesCountReturn = stoi(line);
                }
                catch (...)
                {
                    isValidInput = false;
                }
                if  (this->getInputCustomMinesCountReturn <= 0 || this->getInputCustomMinesCountReturn >= fieldSize)
                {
                    isValidInput = false;
                }
            }
        }
        catch (...)
        {
            continue;
        }
        if (isValidInput == true)
        {
            return this->getInputCustomMinesCountReturn;
        }
        else
        {
            getInputEnterKey(print->wrongInputText);
            print->deleteLastLine(print->wrongInputText.length());
            print->deleteLastLine(print->inputText.length() + line.length());
        }
    }
}

void Input::toggleHelp(Field &field, Common::CoordsStruct const& currentArrayPosition)
{
    extern std::atomic<bool> doPauseTimer;
    extern std::atomic<bool> doPrintTimer;
    doPauseTimer = true;

    common->resizeConsole(107, 27);

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common->centerWindow();
    #else
        disableNonCanonicalMode();
    #endif

    Common::CoordsStruct currentCursorPosition;

    common->clearScreen();
    print->printExplanation();
    getInputEnterKey("");

    field.printAll();

    currentCursorPosition = common->coordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
    common->gotoXY(currentCursorPosition.col, currentCursorPosition.row);

    field.printCoords(currentArrayPosition, true);

    doPauseTimer = false;
    doPrintTimer = true;

    #if !defined(_WIN32) && !defined(WIN32) && !defined(_WIN64) && !defined(WIN64)
        enableNonCanonicalMode();
    #endif
}

// move the players cursor in 4 directions with the arrow keys:
void Input::moveCursor(Field &field, Common::CoordsStruct& currentArrayPosition, Direction &direction, bool *toggleEdgeJump)
{
    Common::CoordsStruct currentCursorPosition;
    std::wcout << L"\b";
    if (direction == Direction::UP)
    {
        if (currentArrayPosition.row == 1)
        {
            if (*toggleEdgeJump)
            {
                field.printCoords(currentArrayPosition, false);
                currentArrayPosition.row = field.getRows();
            }
        }
        else
        {
            field.printCoords(currentArrayPosition, false);
            --currentArrayPosition.row;
        }
    }
    else if (direction == Direction::DOWN)
    {
        if (currentArrayPosition.row == field.getRows())
        {
            if (*toggleEdgeJump)
            {
                field.printCoords(currentArrayPosition, false);
                currentArrayPosition.row = 1;
            }
        }
        else
        {
            field.printCoords(currentArrayPosition, false);
            ++currentArrayPosition.row;
        }
    }
    else if (direction == Direction::LEFT)
    {
        if (currentArrayPosition.col == 1)
        {
            if (*toggleEdgeJump)
            {
                field.printCoords(currentArrayPosition, false);
                currentArrayPosition.col = field.getCols();
            }
        }
        else
        {
            field.printCoords(currentArrayPosition, false);
            --currentArrayPosition.col;
        }
    }
    else if (direction == Direction::RIGHT)
    {
        if (currentArrayPosition.col == field.getCols())
        {
            if (*toggleEdgeJump)
            {
                field.printCoords(currentArrayPosition, false);
                currentArrayPosition.col = 1;
            }
        }
        else
        {
            field.printCoords(currentArrayPosition, false);
            ++currentArrayPosition.col;
        }
    }
    currentCursorPosition = common->coordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
    common->gotoXY(currentCursorPosition.col, currentCursorPosition.row);
    field.printCoords(currentArrayPosition, true);
}

// the main function to get the users input during a game:
const Common::UserInputReturnStruct &Input::getInputGameplay(Field &field, bool firstrun)
{
    this->getInputGameplayReturn.Coords.col = 0;
    this->getInputGameplayReturn.Coords.row = 0;
    this->getInputGameplayReturn.isAutoFlag = false;
    this->getInputGameplayReturn.isFlag = false;

    static Common::CoordsStruct currentArrayPosition;
    Common::CoordsStruct currentCursorPosition;

    if (firstrun == true)
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

    currentCursorPosition = common->coordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
    common->gotoXY(currentCursorPosition.col, currentCursorPosition.row);

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        field.printCoords(currentArrayPosition, true);

        int inputKeyA;
        char inputKeyB;
        while (1)
        {
            try
            {
                if ((inputKeyA = _getch()) == 224)
                {
                    inputKeyB = _getch();
                    if (inputKeyB == KEY_UP)
                    {
                        Direction direction = Direction::UP;
                        moveCursor(field, currentArrayPosition, direction, toogleEdgeJumpP);
                    }
                    else if (inputKeyB == KEY_DOWN)
                    {
                        Direction direction = Direction::DOWN;
                        moveCursor(field, currentArrayPosition, direction, toogleEdgeJumpP);
                    }
                    else if (inputKeyB == KEY_LEFT)
                    {
                        Direction direction = Direction::LEFT;
                        moveCursor(field, currentArrayPosition, direction, toogleEdgeJumpP);
                    }
                    else if (inputKeyB == KEY_RIGHT)
                    {
                        Direction direction = Direction::RIGHT;
                        moveCursor(field, currentArrayPosition, direction, toogleEdgeJumpP);
                    }
                }
                else if (inputKeyA == 'q' || inputKeyA == 'Q')
                {
                    exit(0);
                }
                else if (inputKeyA == 'h' || inputKeyA == 'H')
                {
                    toggleHelp(field, currentArrayPosition);
                    continue;
                }
                else if (inputKeyA == 'f' || inputKeyA == 'F')
                {
                    solver->autoSolve(field, true, false, false);
                    this->getInputGameplayReturn.isAutoFlag = true;
                    break;
                }
                else if (inputKeyA == 'r' || inputKeyA == 'R')
                {
                    solver->autoSolve(field, false, true, false);
                    this->getInputGameplayReturn.isAutoFlag = true;
                    break;
                }
                else if (inputKeyA == 's' || inputKeyA == 'S')
                {
                    solver->autoSolve(field, true, true, true);
                    this->getInputGameplayReturn.isAutoFlag = true;
                    break;
                }
                else if (inputKeyA == 'c' || inputKeyA == 'C')
                {
                    this->toggleEdgeJump == true ? this->toggleEdgeJump = false : this->toggleEdgeJump = true;
                }
                else if (inputKeyA == KEY_ENTER)
                {
                    coutconv << L"\b" << std::flush;
                    if (field.getCoordsContent(currentArrayPosition) == symbols->symbolFlag || field.getCoordsContent(currentArrayPosition) == symbols->symbolZero)
                    {
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
                else if (inputKeyA == KEY_SPACE)
                {
                    coutconv << L"\b" << std::flush;
                    if (field.isNumber(currentArrayPosition) || field.getCoordsContent(currentArrayPosition) == L" ")
                    {
                        continue;
                    }
                    else
                    {
                        this->getInputGameplayReturn.isFlag = true;
                    }
                    break;
                }
                else
                {
                    continue;
                }
            }
            catch (...)
            {
                continue;
            }
        }

    #else
        enableNonCanonicalMode();
        field.printCoords(currentArrayPosition, true);

        char inputKeyA, inputKeyB, inputKeyC;

        while (true)
        {
            try
            {            
                inputKeyA = getchar();
                if (inputKeyA == 27)
                {
                    inputKeyB = getchar();
                    inputKeyC = getchar();
                    if (inputKeyB == 91)
                    {
                        if (inputKeyC == KEY_UP)
                        {
                            Direction direction = Direction::UP;
                            moveCursor(field, currentArrayPosition, direction, toogleEdgeJumpP);
                        }
                        else if (inputKeyC == KEY_DOWN)
                        {
                            Direction direction = Direction::DOWN;
                            moveCursor(field, currentArrayPosition, direction, toogleEdgeJumpP);
                        }
                        else if (inputKeyC == KEY_LEFT)
                        {
                            Direction direction = Direction::LEFT;
                            moveCursor(field, currentArrayPosition, direction, toogleEdgeJumpP);
                        }
                        else if (inputKeyC == KEY_RIGHT)
                        {
                            Direction direction = Direction::RIGHT;
                            moveCursor(field, currentArrayPosition, direction, toogleEdgeJumpP);
                        }
                    }
                }
                else if (inputKeyA == 'q' || inputKeyA == 'Q')
                {
                    exit(0);
                }
                else if (inputKeyA == 'h' || inputKeyA == 'H')
                {
                    toggleHelp(field, currentArrayPosition);
                    continue;
                }
                else if (inputKeyA == 'f' || inputKeyA == 'F')
                {
                    solver->autoSolve(field, true, false, false);
                    this->getInputGameplayReturn.isAutoFlag = true;
                    break;
                }
                else if (inputKeyA == 'r' || inputKeyA == 'R')
                {
                    solver->autoSolve(field, false, true, false);
                    this->getInputGameplayReturn.isAutoFlag = true;
                    break;
                }
                else if (inputKeyA == 's' || inputKeyA == 'S')
                {
                    solver->autoSolve(field, true, true, true);
                    this->getInputGameplayReturn.isAutoFlag = true;
                    break;
                }
                else if (inputKeyA == 'c' || inputKeyA == 'C')
                {
                    this->toggleEdgeJump == true ? this->toggleEdgeJump = false : this->toggleEdgeJump = true;
                }
                else if (inputKeyA == KEY_ENTER)
                {
                    std::cout << "\b" << std::flush;
                    if (field.getCoordsContent(currentArrayPosition) == symbols->symbolFlag || field.getCoordsContent(currentArrayPosition) == symbols->symbolZero)
                    {
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
                else if (inputKeyA == KEY_SPACE)
                {
                    std::cout << "\b" << std::flush;
                    if (field.isNumber(currentArrayPosition) || field.getCoordsContent(currentArrayPosition) == " ")
                    {
                        continue;
                    }
                    else
                    {
                        this->getInputGameplayReturn.isFlag = true;
                        break;
                    }
                }
                else
                {
                    continue;
                }
            }
            catch (...)
            {
                continue;
            }
        }
        disableNonCanonicalMode();
    #endif

    this->getInputGameplayReturn.Coords.col = currentArrayPosition.col;
    this->getInputGameplayReturn.Coords.row = currentArrayPosition.row;

    return this->getInputGameplayReturn;
}
