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
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

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
    colors(std::make_unique<Colors>()),
    common(std::make_unique<Common>()),
    print(std::make_unique<Print>()),
    solver(std::make_unique<Solver>()),
    symbols(std::make_unique<Symbols>())
{
}

Input::~Input()
{
}

void Input::getInputEnterKey(std::string const& text)
{
    std::cout << colors->setTextColor(colors->fg_white);
    std::cout << text << std::flush;
    std::cout << colors->setTextColor(colors->color_default);

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

// custom mode: ask user for the game mode (difficulty):
int Input::getInputDifficulty()
{
    int difficulty = 0;

    print->printMenu();

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        while (true)
        {
            int inputKey = 0;
            if ((inputKey = _getch()) == 'q' || inputKey == 'Q')
            {
                common->clearScreen();
                common->exitProgram(0);
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
                common->exitProgram(0);
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

int Input::getInputCustomCellWidth()
{
    std::string line = "";
    int cellWidth = 3;
    bool isValidInput = false;

    common->clearScreen();
    print->printCustomGetCellWidth();

    while (true)
    {
        std::cout << colors->setTextColor(colors->fg_white);
        std::cout << print->inputText;
        std::cout << colors->setTextColor(colors->color_default);
        getline(std::cin, line);
        if (line == "q" || line == "Q")
        {
            common->exitProgram(0);
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
                cellWidth = stoi(line);
            }
            catch (...)
            {
                isValidInput = false;
            }
            if  (cellWidth <= 0 || cellWidth > 3 || cellWidth % 2 == 0)
            {
                isValidInput = false;
            }
        }
        if (isValidInput == true)
        {
            return cellWidth;
        }
        else
        {
            getInputEnterKey(print->wrongInputText);
            print->deleteLastLine(print->wrongInputText.length());
            print->deleteLastLine(print->inputText.length() + line.length());
        }
    }    
}

// custom mode: ask user for the size of the field:
Common::CoordsStruct Input::getInputCustomDimensions(int const& fieldCellWidth)
{
    Common::CoordsStruct dimensions;
    std::string line = "";
    int beforeX = 0;
    int afterX = 0;
    bool isValidInput = false;

    common->clearScreen();
    print->printCustomGetDimensions(fieldCellWidth);

    while (true)
    {
        std::cout << colors->setTextColor(colors->fg_white);
        std::cout << print->inputText;
        std::cout << colors->setTextColor(colors->color_default);
        getline(std::cin, line);
        if (line == "q" || line == "Q")
        {
            common->exitProgram(0);
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
                if (beforeX < 8 || afterX < 8 || beforeX > 80 || afterX > 20)
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
        if (isValidInput == true)
        {
            dimensions.col = beforeX;
            dimensions.row = afterX;
            return dimensions;
        }
        else
        {
            getInputEnterKey(print->wrongInputText);
            print->deleteLastLine(print->wrongInputText.length());
            print->deleteLastLine(print->inputText.length() + line.length());
        }
    }
}

// custom mode: ask user for the number of mines:
int Input::getInputCustomMinesCount(int const& fieldSize)
{
    std::string line = "";
    int minesTotal = 0;
    bool isValidInput = false;

    common->clearScreen();
    print->printCustomGetMinesCount();

    while (true)
    {
        std::cout << colors->setTextColor(colors->fg_white);
        std::cout << print->inputText;
        std::cout << colors->setTextColor(colors->color_default);
        getline(std::cin, line);
        if (line == "q" || line == "Q")
        {
            common->exitProgram(0);
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
                minesTotal = stoi(line);
            }
            catch (...)
            {
                isValidInput = false;
            }
            if  (minesTotal <= 0 || minesTotal >= fieldSize)
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
            getInputEnterKey(print->wrongInputText);
            print->deleteLastLine(print->wrongInputText.length());
            print->deleteLastLine(print->inputText.length() + line.length());
        }
    }
}

void Input::helpToggle(Field &field, Common::CoordsStruct const& currentArrayPosition)
{
    extern bool doPauseTimer;
    doPauseTimer = true;

    common->resizeConsole(107, 27);

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common->centerWindow();
        print->showBlinkingCursor(false);
    #endif

    Common::CoordsStruct currentCursorPosition;
    common->clearScreen();
    print->printExplanation();
    getInputEnterKey("");
    common->resizeConsole(field.getOffsetX() + (field.getCols() * (((field.getCellWidth() - 1) / 2) * 2 + 2)) + field.getOffsetX() - 3, field.getOffsetY() + (field.getRows() * 2) + 5);

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common->centerWindow();
    #endif

    common->clearScreen();
    common->gotoXY(field.getOffsetX() - 1, 1);
    print->printTitle(field.getDifficultyString(), field.getCols(), field.getRows(), field.getMinesTotal());
    common->gotoXY(1, 3);
    field.drawField();
    std::cout << newline;
    print->printMinesLeft(field);

    #if DEBUG == 1
        print->printDebugCoveredLeft(field);
    #endif

    common->gotoXY(field.getOffsetX() - 1, field.getOffsetY() + field.getRows()*2);
    std::cout << colors->setTextColor(colors->fg_white);
    std::cout << print->getHelpText << newline << newline;
    std::cout << colors->setTextColor(colors->color_default);
    currentCursorPosition = common->coordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
    common->gotoXY(currentCursorPosition.col, currentCursorPosition.row);

    field.printCoords(currentArrayPosition, true);

    doPauseTimer = false;
}

// move the players cursor in 4 directions with the arrow keys:
void Input::moveCursor(Field &field, Common::CoordsStruct& currentArrayPosition, Direction &direction, bool *toggleEdgeJump)
{
    extern bool isTimerPrinting;
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        while (isTimerPrinting) Sleep(5);
    #else
        while (isTimerPrinting) usleep(5 * 1000);
    #endif

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
Common::UserInputReturnStruct Input::getUserInput(Field &field, int firstrun)
{
    static Common::CoordsStruct currentArrayPosition;
    Common::CoordsStruct currentCursorPosition;
    Common::UserInputReturnStruct returnStruct;
    bool toggleEdgeJump = false;
    bool *toogleEdgeJumpP;
    toogleEdgeJumpP = &toggleEdgeJump;

    common->gotoXY(field.getOffsetX() - 1, field.getOffsetY() + field.getRows() * 2);
    std::cout << colors->setTextColor(colors->fg_white);
    std::cout << print->getHelpText << newline << newline;
    std::cout << colors->setTextColor(colors->color_default);

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

    currentCursorPosition = common->coordsToCursorPosition(currentArrayPosition, field.getOffsetX(), field.getOffsetY(), field.getCellWidth());
    common->gotoXY(currentCursorPosition.col, currentCursorPosition.row);

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        field.printCoords(currentArrayPosition, true);

        int inputKeyA;
        char inputKeyB;

        while(1)
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
                common->clearScreen();
                common->exitProgram(0);
            }
            else if (inputKeyA == 'h' || inputKeyA == 'H')
            {
                helpToggle(field, currentArrayPosition);
                continue;
            }
            else if (inputKeyA == 'f' || inputKeyA == 'F')
            {
                solver->autoSolve(field, true, false, false);
                returnStruct.isAutoFlag = true;
                break;
            }
            else if (inputKeyA == 'r' || inputKeyA == 'R')
            {
                solver->autoSolve(field, false, true, false);
                returnStruct.isAutoFlag = true;
                break;
            }
            else if (inputKeyA == 's' || inputKeyA == 'S')
            {
                solver->autoSolve(field, true, true, true);
                returnStruct.isAutoFlag = true;
                break;
            }
            else if (inputKeyA == 'c' || inputKeyA == 'C')
            {                
                toggleEdgeJump == true ? toggleEdgeJump = false : toggleEdgeJump = true;
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
            else if  (inputKeyA == KEY_SPACE)
            {
                coutconv << L"\b" << std::flush;
                if (field.isNumber(currentArrayPosition) || field.getCoordsContent(currentArrayPosition) == L" ")
                {
                    continue;
                }
                else
                {
                    returnStruct.isFlag = true;
                }
                break;
            }
            else
            {
                continue;
            }
        }

    #else
        enableNonCanonicalMode();
        field.printCoords(currentArrayPosition, true);

        char inputKeyA;
        char inputKeyB;
        char inputKeyC;

        while (true)
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
                common->exitProgram(0);
            }
            else if (inputKeyA == 'h' || inputKeyA == 'H')
            {
                helpToggle(field, currentArrayPosition);
                continue;
            }
            else if (inputKeyA == 'f' || inputKeyA == 'F')
            {
                solver->autoSolve(field, true, false, false);
                returnStruct.isAutoFlag = true;
                break;
            }
            else if (inputKeyA == 'r' || inputKeyA == 'R')
            {
                solver->autoSolve(field, false, true, false);
                returnStruct.isAutoFlag = true;
                break;
            }
            else if (inputKeyA == 's' || inputKeyA == 'S')
            {
                solver->autoSolve(field, true, true, true);
                returnStruct.isAutoFlag = true;
                break;
            }
            else if (inputKeyA == 'c' || inputKeyA == 'C')
            {
                toggleEdgeJump == true ? toggleEdgeJump = false : toggleEdgeJump = true;
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
                    returnStruct.isFlag = true;
                    break;
                }
            }
            else
            {
                continue;
            }
        }

        disableNonCanonicalMode();
    #endif

    returnStruct.Coords.col = currentArrayPosition.col;
    returnStruct.Coords.row = currentArrayPosition.row;

    return returnStruct;
}
