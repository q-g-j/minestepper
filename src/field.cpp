#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#endif

#include "../include/colors.hpp"
#include "../include/common.hpp"
#include "../include/debug.hpp"
#include "../include/field.hpp"
#include "../include/input.hpp"
#include "../include/print.hpp"
#include "../include/symbols.hpp"

// constructor
Field::Field(int const& cols_, int const& rows_, int const& fieldOffsetX_, int const& fieldOffsetY_, int const& fieldCellWidth_, int const& minesCount_, std::string const& difficultyString_)
:
    cols(cols_),
    rows(rows_),
    fieldOffsetX(fieldOffsetX_),
    fieldOffsetY(fieldOffsetY_),
    fieldCellWidth(fieldCellWidth_),
    minesCount(minesCount_),
    difficultyString(difficultyString_)
{
    this->minesLeft = minesCount_;
    this->countCovered = cols_ * rows_;
    this->flagsCount = 0;
    this->field2DVector = create2DVector();
    this->mines2DVector = create2DVector();
    clearField();
    clearMines();
}

// deconstructor:
Field::~Field()
{
}

// some getters:
int Field::getCols()
{
    return this->cols;
}

int Field::getRows()
{
    return this->rows;
}

int Field::getOffsetX()
{
    return this->fieldOffsetX;
}

int Field::getOffsetY()
{
    return this->fieldOffsetY;
}

int Field::getCellWidth()
{
    return this->fieldCellWidth;
}

int Field::getCovered()
{
    return this->countCovered;
}

int Field::getMinesCount()
{
    return this->minesCount;
}

int Field::getMinesLeft()
{
    return this->minesLeft;
}

std::string Field::getDifficultyString()
{
    return this->difficultyString;
}

stringconv Field::getCoordsContent(Common::CoordsStruct const& coords)
{
    return this->field2DVector[coords.col][coords.row];
}

// use 2D vectors for the game field:
std::vector<std::vector<stringconv>> Field::create2DVector()
{
    Symbols symbols;

    const int colsNum = this->cols + 1;
    const int rowsNum = this->rows + 1;
    std::vector<std::vector<stringconv>> temp2DVector;
        for(int i=0; i<colsNum; i++) {
            std::vector<stringconv> row;
            for(int j=0; j<rowsNum; j++) {
                    row.push_back(symbols.symbolZero);
            }
            temp2DVector.push_back(row);
    }
    return temp2DVector;
}

// fill this->field2DVector[][] with symbolCovered
void Field::clearField()
{
    Symbols symbols;

    for (int i=0; i <= this->cols; i++)
    {
        for (int j=0; j <= this->rows; j++)
        {
            this->field2DVector[i][j] = symbols.symbolCovered;
        }
    }
}

// fill this->mines2DVector[][] with symbolZero
void Field::clearMines()
{
    Symbols symbols;

    for (int i=0; i <= this->cols; i++)
    {
        for (int j=0; j <= this->rows; j++)
        {
            this->mines2DVector[i][j] = symbols.symbolZero;
        }
    }
}

// place mines at random positions of this->mines2DVector[][]:
void Field::fillMines(Common::CoordsStruct& userFirstInput)
{
    Common common;
    Symbols symbols;

    #if CUSTOM_FIELD == 1
    this->mines2DVector[1][9] = symbols.symbolMine;
    this->mines2DVector[2][9] = symbols.symbolMine;
    this->mines2DVector[4][9] = symbols.symbolMine;
    this->minesCount = 3;
    #else
    Common::CoordsStruct coords;
    size_t sizeofField2DVector = this->cols * this->rows;
    std::vector<int> tempVector;
    for (unsigned int i = 1; i <= sizeofField2DVector; i++)
    {
        if (i != common.coordsToInt(userFirstInput, this->cols))
            tempVector.push_back(i);
    }

    std::random_shuffle(tempVector.begin(), tempVector.end());
    for (int i = 0; i < this->minesCount; i++)
    {
        coords = common.intToCoords(tempVector.at(i), this->cols);
        this->mines2DVector[coords.col][coords.row] = symbols.symbolMine;
    }
    #endif
}

// draw this->field2DVector[][]:
void Field::drawField()
{
    Common common;
    Symbols symbols;

    common.setUnicode(true);

    for (int i = 0; i < this->fieldOffsetY - 4; i++)
        std::wcout << L"\n";
    for (int i = 0; i < this->fieldOffsetX - 2; i++)
        std::wcout << L" ";
    coutconv << symbols.symbolCornerTopLeft;
    for (int col = 1; col <= this->cols; col++)
    {
        if (col < this->cols)
        {
            for (int width = 0; width < this->fieldCellWidth; width++)
                coutconv << symbols.symbolHorizontalLine;
            coutconv << symbols.symbolDownT;
        }
        else
        {
            for (int width = 0; width < this->fieldCellWidth; width++)
                coutconv << symbols.symbolHorizontalLine;
            coutconv << symbols.symbolCornerTopRight;
        }
    }
    std::wcout << L"\n";

    for (int row = 1; row <= this->rows; row++)
    {
        for (int col = 1; col <= this->cols; col++)
        {
            if (col == 1)
            {
                for (int i = 0; i < this->fieldOffsetX - 2; i++)
                    std::wcout << L" ";
            }
            coutconv << symbols.symbolVerticalLine;

            for (int padding = 0; padding < (this->fieldCellWidth-1)/2; padding++)
                std::wcout << L" ";

            Common::CoordsStruct Coords;
            Coords.col = col;
            Coords.row = row;
            printCoords(Coords, false);

            for (int padding = 0; padding < (this->fieldCellWidth-1)/2; padding++)
                std::wcout << L" ";

            if (col == this->cols)
            {
                coutconv << symbols.symbolVerticalLine;
                std::wcout << L"\n";
                for (int i = 0; i < this->fieldOffsetX - 2; i++)
                    std::wcout << L" ";
            }
        }

        if (row < this->rows)
            coutconv << symbols.symbolRightT;
        else
            coutconv << symbols.symbolCornerBottomLeft;

        for (int col = 1; col <= this->cols; col++)
        {
            if (col < this->cols)
            {
                for (int width = 0; width < this->fieldCellWidth; width++)
                    coutconv << symbols.symbolHorizontalLine;
                if (row < this->rows)
                    coutconv << symbols.symbolPlus;
                else
                    coutconv << symbols.symbolUpT;
            }
            else
            {
                for (int width = 0; width < this->fieldCellWidth; width++)
                    coutconv << symbols.symbolHorizontalLine;
                if (row < this->rows)
                    coutconv << symbols.symbolLeftT;
                else
                    coutconv << symbols.symbolCornerBottomRight;
            }
        }
        std::wcout << L"\n";
    }
    common.setUnicode(false);
}

void Field::gotoXY(int const& x, int const& y)
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coordsNew;

        coordsNew.X = x;
        coordsNew.Y = y;
        SetConsoleCursorPosition(hConsole, coordsNew);
    #else
        printf("%c[%d;%df",0x1B,y + 1,x + 1);
    #endif
}

// the main method to print the content of a particular cell:
void Field::printCoords(Common::CoordsStruct& coords, bool isCursor)
{
    Colors colors;
    Common common;
    Symbols symbols;

    std::string content;
    Common::CoordsStruct tempCoords;

    tempCoords = common.coordsToCursorPosition(coords, this->fieldOffsetX, this->fieldOffsetY, this->fieldCellWidth);
    gotoXY(tempCoords.col, tempCoords.row);

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (isNumber(coords))
        {
            if (isCursor == false)
            {
                if (getCoordsContent(coords) == L"1")
                    SetConsoleTextAttribute(hConsole, colors.fg_light_blue);
                else if (getCoordsContent(coords) == L"2")
                    SetConsoleTextAttribute(hConsole, colors.fg_light_green);
                else if (getCoordsContent(coords) == L"3")
                    SetConsoleTextAttribute(hConsole, colors.fg_light_red);
                else if (getCoordsContent(coords) == L"4")
                    SetConsoleTextAttribute(hConsole, colors.fg_magenta);
                else if (getCoordsContent(coords) == L"5")
                    SetConsoleTextAttribute(hConsole, colors.fg_yellow);
                else if (getCoordsContent(coords) == L"6")
                    SetConsoleTextAttribute(hConsole, colors.fg_green);
                else if (getCoordsContent(coords) == L"7")
                    SetConsoleTextAttribute(hConsole, colors.fg_cyan);
                else if (getCoordsContent(coords) == L"8")
                    SetConsoleTextAttribute(hConsole, colors.fg_white);
                else
                    SetConsoleTextAttribute(hConsole, colors.color_default);
            }
            else
            {
                if (getCoordsContent(coords) == L"1")
                    SetConsoleTextAttribute(hConsole, colors.bg_light_blue);
                else if (getCoordsContent(coords) == L"2")
                    SetConsoleTextAttribute(hConsole, colors.bg_light_green);
                else if (getCoordsContent(coords) == L"3")
                    SetConsoleTextAttribute(hConsole, colors.bg_light_red);
                else if (getCoordsContent(coords) == L"4")
                    SetConsoleTextAttribute(hConsole, colors.bg_magenta);
                else if (getCoordsContent(coords) == L"5")
                    SetConsoleTextAttribute(hConsole, colors.bg_yellow);
                else if (getCoordsContent(coords) == L"6")
                    SetConsoleTextAttribute(hConsole, colors.bg_green);
                else if (getCoordsContent(coords) == L"7")
                    SetConsoleTextAttribute(hConsole, colors.bg_cyan);
                else if (getCoordsContent(coords) == L"8")
                    SetConsoleTextAttribute(hConsole, colors.bg_black);
                else
                    SetConsoleTextAttribute(hConsole, colors.color_default);
            }
        }
        else if (getCoordsContent(coords) == symbols.symbolFlag)
        {
            if (isCursor)
                SetConsoleTextAttribute(hConsole, colors.bg_cyan);
            else
                SetConsoleTextAttribute(hConsole, colors.fg_cyan);
        }
        else if (getCoordsContent(coords) == symbols.symbolMine)
            SetConsoleTextAttribute(hConsole, colors.fg_red);
        else if (getCoordsContent(coords) == symbols.symbolMineHit)
            SetConsoleTextAttribute(hConsole, colors.fg_yellow);
        std::wstring coordsString = this->field2DVector[coords.col][coords.row];
        WriteConsoleW(hConsole, coordsString.c_str(), static_cast<DWORD>(coordsString.size()), nullptr, nullptr);
        SetConsoleTextAttribute(hConsole, colors.color_default);
    #else
        if (isNumber(coords))
        {
            if (isCursor == false)
            {
                if (getCoordsContent(coords) == "1")
                    content = colors.fg_light_blue + "1";
                else if (getCoordsContent(coords) == "2")
                    content = colors.fg_light_green + "2";
                else if (getCoordsContent(coords) == "3")
                    content = colors.fg_light_red + "3";
                else if (getCoordsContent(coords) == "4")
                    content = colors.fg_magenta + "4";
                else if (getCoordsContent(coords) == "5")
                    content = colors.fg_yellow + "5";
                else if (getCoordsContent(coords) == "6")
                    content = colors.fg_green + "6";
                else if (getCoordsContent(coords) == "7")
                    content = colors.fg_light_red + "7";
                else if (getCoordsContent(coords) == "8")
                    content = colors.fg_white + "8";
                else
                    content = colors.color_default + (this->field2DVector[coords.col][coords.row]);
            }
            else
            {
                if (getCoordsContent(coords) == "1")
                    content = colors.bg_light_blue + "1";
                else if (getCoordsContent(coords) == "2")
                    content = colors.bg_light_green + "2";
                else if (getCoordsContent(coords) == "3")
                    content = colors.bg_light_red + "3";
                else if (getCoordsContent(coords) == "4")
                    content = colors.bg_magenta + "4";
                else if (getCoordsContent(coords) == "5")
                    content = colors.bg_yellow + "5";
                else if (getCoordsContent(coords) == "6")
                    content = colors.bg_green + "6";
                else if (getCoordsContent(coords) == "7")
                    content = colors.bg_light_red + "7";
                else if (getCoordsContent(coords) == "8")
                    content = colors.bg_black + "8";
                else
                    content = colors.color_default + (this->field2DVector[coords.col][coords.row]);
            }
        }
        else if (getCoordsContent(coords) == symbols.symbolFlag)
        {
            if (isCursor)
                content = colors.bg_cyan + symbols.symbolFlag;
            else
                content = colors.fg_cyan + symbols.symbolFlag;
        }
        else if (getCoordsContent(coords) == symbols.symbolMine)
            content = colors.fg_red + symbols.symbolMine;
        else if (getCoordsContent(coords) == symbols.symbolMineHit)
            content = colors.fg_yellow + symbols.symbolMineHit;
        else
            content = colors.color_default + (this->field2DVector[coords.col][coords.row]);

        coutconv << content << std::flush;
        coutconv << colors.color_default << std::flush;
    #endif
}

// test coords if they contain a flag:
bool Field::isFlag(Common::CoordsStruct& coords)
{
    Symbols symbols;

    if (this->field2DVector[coords.col][coords.row] == symbols.symbolFlag)
        return true;
    else
        return false;
}

// test coords if they contain a number:
bool Field::isNumber(Common::CoordsStruct& coords)
{
    Common common;
    for (int i = 1; i < 8; i++)
    {
        if (this->field2DVector[coords.col][coords.row] == common.intToString(i))
            return true;
    }
    return false;
}

// find neighbours of a cell at "coords" that hold a given content (passed by variable symbol)
std::vector<Common::CoordsStruct> Field::findNeighbours(std::vector<std::vector<stringconv>> const& temp2DVector, Common::CoordsStruct const& coords, stringconv const& symbol)
{
    std::vector<Common::CoordsStruct> neighboursVector;

    // up left:
    if (coords.col-1 > 0 && coords.row-1 > 0)
    {
        if (temp2DVector[coords.col-1][coords.row-1] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col-1;
            tempCoords.row = coords.row-1;
            neighboursVector.push_back(tempCoords);
        }
    }

    // up middle:
    if (coords.row-1 > 0)
    {
        if (temp2DVector[coords.col][coords.row-1] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col;
            tempCoords.row = coords.row-1;
            neighboursVector.push_back(tempCoords);
        }
    }

    // up right:
    if (coords.col+1 <= this->cols && coords.row-1 > 0)
    {
        if (temp2DVector[coords.col+1][coords.row-1] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col+1;
            tempCoords.row = coords.row-1;
            neighboursVector.push_back(tempCoords);
        }
    }

    // middle left:
    if (coords.col-1 > 0)
    {
        if (temp2DVector[coords.col-1][coords.row] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col-1;
            tempCoords.row = coords.row;
            neighboursVector.push_back(tempCoords);
        }
    }

    // middle right:
    if (coords.col+1 <= this->cols)
    {
        if (temp2DVector[coords.col+1][coords.row] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col+1;
            tempCoords.row = coords.row;
            neighboursVector.push_back(tempCoords);
        }
    }

    // down left:
    if (coords.col-1 > 0 && coords.row+1 <= this->rows)
    {
        if (temp2DVector[coords.col-1][coords.row+1] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col-1;
            tempCoords.row = coords.row+1;
            neighboursVector.push_back(tempCoords);
        }
    }

    // down middle:
    if (coords.row+1 <= this->rows)
    {
        if (temp2DVector[coords.col][coords.row+1] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col;
            tempCoords.row = coords.row+1;
            neighboursVector.push_back(tempCoords);
        }
    }

    // down right:
    if (coords.col+1 <= this->cols && coords.row+1 <= this->rows)
    {
        if (temp2DVector[coords.col+1][coords.row+1] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col+1;
            tempCoords.row = coords.row+1;
            neighboursVector.push_back(tempCoords);
        }
    }
    return neighboursVector;
}

// automatically uncover all connected cells, as long as they have no neighbour mines:
void Field::autoUncoverRecursive(Common::CoordsStruct const& coords, std::vector<unsigned int> &poolVector)
{
    Common common;
    Symbols symbols;

    // create vector holding covered neighbours:
    std::vector<Common::CoordsStruct> neighboursCoveredVector;
    neighboursCoveredVector = findNeighbours(this->field2DVector, coords, symbols.symbolCovered);

    for (size_t i = 0; i < neighboursCoveredVector.size(); i++)
    {
        std::vector<Common::CoordsStruct> neighboursMinesVector;
        neighboursMinesVector = findNeighbours(this->mines2DVector, neighboursCoveredVector.at(i), symbols.symbolMine);
        if (std::find(poolVector.begin(), poolVector.end(), common.coordsToInt(neighboursCoveredVector.at(i), this->cols)) == poolVector.end())
        {
            if (neighboursMinesVector.size() == 0)
            {
                if (this->mines2DVector[neighboursCoveredVector.at(i).col][neighboursCoveredVector.at(i).row] != symbols.symbolMine)
                {
                    this->field2DVector[neighboursCoveredVector.at(i).col][neighboursCoveredVector.at(i).row] = symbols.symbolZero;
                    poolVector.push_back(common.coordsToInt(neighboursCoveredVector.at(i), this->cols));
                    printCoords(neighboursCoveredVector.at(i), false);
                }
            }
            else
            {
                if (this->mines2DVector[neighboursCoveredVector.at(i).col][neighboursCoveredVector.at(i).row] != symbols.symbolMine)
                {
                    this->field2DVector[neighboursCoveredVector.at(i).col][neighboursCoveredVector.at(i).row] = common.intToString(neighboursMinesVector.size());
                    poolVector.push_back(common.coordsToInt(neighboursCoveredVector.at(i), this->cols));
                    printCoords(neighboursCoveredVector.at(i), false);
                }
            }            
        }
        if (neighboursMinesVector.size() == 0)
            autoUncoverRecursive(neighboursCoveredVector.at(i), poolVector);
    }
}

Common::PlaceUserInputReturnStruct Field::gameWon()
{
    Common common;
    Print print;
    Symbols symbols;

    Common::PlaceUserInputReturnStruct returnStruct;

    common.clearScreen();
    print.printTitle(this->difficultyString, this->cols, this->rows, this->minesCount);
    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
            if (this->mines2DVector[i][j] == symbols.symbolMine)
                this->field2DVector[i][j] = symbols.symbolMine;
    }

    drawField();
    print.printHasWon();
    returnStruct.hasWon = true;
    return returnStruct;
}

Common::PlaceUserInputReturnStruct Field::gameLost()
{
    Common common;
    Print print;
    Symbols symbols;

    common.clearScreen();
    print.printTitle(this->difficultyString, this->cols, this->rows, this->minesCount);
    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            if (this->field2DVector[i][j] == symbols.symbolFlag && this->mines2DVector[i][j] != symbols.symbolMine && this->mines2DVector[i][j] != symbols.symbolMineHit) // && this->mines2DVector[i][j] == coveredSymbol)
                this->mines2DVector[i][j] = this->field2DVector[i][j];
        }
    }
    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            this->field2DVector[i][j] = this->mines2DVector[i][j];
        }
    }
    drawField();

    print.printHasLost();
    Common::PlaceUserInputReturnStruct returnStruct;
    returnStruct.hasLost = true;
    return returnStruct;
}

// the main method of class Field which will alter this->field2DVector.
// Takes single coords from Input::UserInput method.
Common::PlaceUserInputReturnStruct Field::placeUserInput(Common::UserInputReturnStruct& userInput, int& turn)
{
    Common common;
    Print print;
    Symbols symbols;

    Common::PlaceUserInputReturnStruct returnStruct;

    // set or remove flag if requested
    if (userInput.isFlag == true)
    {
        if (isFlag(userInput.Coords) == true)
        {
            this->field2DVector[userInput.Coords.col][userInput.Coords.row] = symbols.symbolCovered;
            printCoords(userInput.Coords, false);
            this->flagsCount--;
            this->minesLeft++;
        }
        else
        {
            this->field2DVector[userInput.Coords.col][userInput.Coords.row] = symbols.symbolFlag;
            printCoords(userInput.Coords, false);
            this->flagsCount++;
            this->minesLeft--;
        }
    }
    else
    {
        // fill the Field with random mines AFTER the player has placed his first guess:
        if (turn == 1)
        {
            fillMines(userInput.Coords);
        }

        // check if the player hit a mine which ends the game:
        if (this->mines2DVector[userInput.Coords.col][userInput.Coords.row] == symbols.symbolMine)
        {
            this->mines2DVector[userInput.Coords.col][userInput.Coords.row] = symbols.symbolMineHit;
            returnStruct = gameLost();
            return returnStruct;
        }

        // if the player has typed the coords of an already uncovered position and has placed all flags right,
        // uncover all surrounding safe positions:
        if (isNumber(userInput.Coords))
        {
            // create a new vector of surrounding mines:
            std::vector<Common::CoordsStruct> autoUncoverNeighboursMinesVector;
            autoUncoverNeighboursMinesVector = findNeighbours(this->mines2DVector, userInput.Coords, symbols.symbolMine);

            // create a new vector of surrounding flags:
            std::vector<Common::CoordsStruct> autoUncoverNeighboursFlagsVector;
            autoUncoverNeighboursFlagsVector = findNeighbours(this->field2DVector, userInput.Coords, symbols.symbolFlag);

            // create a new vector of surrounding covered squares:
            std::vector<Common::CoordsStruct> autoUncoverNeighboursCoveredVector;
            autoUncoverNeighboursCoveredVector = findNeighbours(this->field2DVector, userInput.Coords, symbols.symbolCovered);

            // create a new empty vector for missed mines:
            std::vector<Common::CoordsStruct> autoUncoverMissedMinesVector;

            // if player has placed some flags around UserInput.Coords:
            if (autoUncoverNeighboursFlagsVector.size() != 0)
            {
                // only proceed if the flag number matches the number of actual surrounding mines:
                if (autoUncoverNeighboursMinesVector.size() == autoUncoverNeighboursFlagsVector.size())
                {
                    // for each covered neighbour of userInput.Coords check if the player has missed a mine
                    // and add this mines position to autoUncoverMissedMinesVector:
                    for (size_t i = 0; i < autoUncoverNeighboursCoveredVector.size(); i++)
                    {
                        if (this->mines2DVector[autoUncoverNeighboursCoveredVector.at(i).col][autoUncoverNeighboursCoveredVector.at(i).row] == symbols.symbolMine)
                            autoUncoverMissedMinesVector.push_back(autoUncoverNeighboursCoveredVector.at(i));
                    }
                    // if there are missed mines, reveal the mines2DVector - player has lost:
                    if (autoUncoverMissedMinesVector.size() != 0)
                    {
                        for (size_t i = 0; i < autoUncoverMissedMinesVector.size(); i++)
                        {
                            this->mines2DVector[autoUncoverMissedMinesVector.at(i).col][autoUncoverMissedMinesVector.at(i).row] = symbols.symbolMineHit;
                        }
                        returnStruct = gameLost();
                        return returnStruct;
                    }
                    // else if all flags are placed correctly:
                    else
                    {
                        if (autoUncoverNeighboursMinesVector.size() == autoUncoverNeighboursFlagsVector.size())
                        {
                            // for each covered neighbour of userInput.Coords, print the number of surrounding mines:
                            for (size_t i = 0; i < autoUncoverNeighboursCoveredVector.size(); i++)
                            {
                                Common::CoordsStruct tempCoords;
                                tempCoords.col = autoUncoverNeighboursCoveredVector.at(i).col;
                                tempCoords.row = autoUncoverNeighboursCoveredVector.at(i).row;
                                std::vector<Common::CoordsStruct> autoUncoverNeighboursCoveredMinesVector;
                                autoUncoverNeighboursCoveredMinesVector = findNeighbours(this->mines2DVector, tempCoords, symbols.symbolMine);
                                std::vector<unsigned int> poolVector;
                                if (autoUncoverNeighboursCoveredMinesVector.size() == 0)
                                {                                    
                                    if (std::find(poolVector.begin(), poolVector.end(), common.coordsToInt(tempCoords, this->cols)) == poolVector.end())
                                    {
                                        this->field2DVector[tempCoords.col][tempCoords.row] = symbols.symbolZero;
                                        printCoords(tempCoords, false);
                                    }
                                    autoUncoverRecursive(tempCoords, poolVector);
                                }
                                else
                                {                                    
                                    if (std::find(poolVector.begin(), poolVector.end(), common.coordsToInt(tempCoords, this->cols)) == poolVector.end())
                                    {
                                        this->field2DVector[tempCoords.col][tempCoords.row] = common.intToString(static_cast<int>(autoUncoverNeighboursCoveredMinesVector.size()));
                                        printCoords(tempCoords, false);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            // uncover the players choice and place the number of surrounding mines in it:
            std::vector<Common::CoordsStruct> neighboursMinesVector = findNeighbours(this->mines2DVector, userInput.Coords, symbols.symbolMine);
            if (neighboursMinesVector.size() == 0)
                this->field2DVector[userInput.Coords.col][userInput.Coords.row] = symbols.symbolZero;
            else
                this->field2DVector[userInput.Coords.col][userInput.Coords.row] = common.intToString(static_cast<int>(neighboursMinesVector.size()));
            printCoords(userInput.Coords, false);
            returnStruct.isTurn = true;
        }

        // Call recursive method autoUncoverRecursive() to automatically uncover all connected cells, as long as
        // they have no neighbour mines:
        std::vector<Common::CoordsStruct> autouncoverNeighboursMinesVector = findNeighbours(this->mines2DVector, userInput.Coords, symbols.symbolMine);
        if (autouncoverNeighboursMinesVector.size() == 0)
        {
            std::vector<unsigned int> poolVector;
            autoUncoverRecursive(userInput.Coords, poolVector);
        }
    }

    unsigned int countCoveredTemp = 0;
    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            if (this->field2DVector[i][j] == symbols.symbolCovered)
                countCoveredTemp++;
        }
    }
    this->countCovered = countCoveredTemp;

    // check if player has won:
    if (this->flagsCount + this->countCovered == this->minesCount)
        returnStruct = gameWon();

    return returnStruct;
}
