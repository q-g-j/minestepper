#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "../include/colors.hpp"
#include "../include/common.hpp"
#include "../include/debug.hpp"
#include "../include/field.hpp"
#include "../include/input.hpp"
#include "../include/print.hpp"
#include "../include/symbols.hpp"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// constructor
Field::Field(int const& cols_, int const& rows_, int const& fieldOffsetX_, int const& fieldOffsetY_, int const& fieldCellWidth_, int const& minesTotal_, std::string const& difficultyString_)
:
    cols(cols_),
    rows(rows_),
    fieldOffsetX(fieldOffsetX_),
    fieldOffsetY(fieldOffsetY_),
    fieldCellWidth(fieldCellWidth_),
    minesTotal(minesTotal_),
    difficultyString(difficultyString_)
{
    flagsCount = 0;
    minesLeft = minesTotal_;
    coveredLeft = cols_ * rows_;
    field2DVector = create2DVector("field");
    mines2DVector = create2DVector("mines");
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

int Field::getCoveredLeft()
{
    return this->coveredLeft;
}

int Field::getMinesTotal()
{
    return this->minesTotal;
}

int Field::getMinesLeft()
{
    return this->minesLeft;
}

int Field::getFlagsCount()
{
    return this->flagsCount;
}

std::string Field::getDifficultyString()
{
    return this->difficultyString;
}

stringconv Field::getCoordsContent(Common::CoordsStruct const& coords)
{
    return this->field2DVector[coords.col][coords.row];
}

void Field::setCoveredLeft::operator--()
{
    --field_.coveredLeft;
}
void Field::setCoveredLeft::operator++()
{
    ++field_.coveredLeft;
}

void Field::setFlagsCount::operator--()
{
    --field_.flagsCount;
}
void Field::setFlagsCount::operator++()
{
    ++field_.flagsCount;
}

void Field::setMinesLeft::operator--()
{
    --field_.minesLeft;
}
void Field::setMinesLeft::operator++()
{
    ++field_.minesLeft;
}

#if DEBUG == 1
    void Field::debugPrintCountCovered(Common::CoordsStruct const& coordsOld)
    {
        Common common;
        Common::CoordsStruct tempCoords;
        tempCoords = common.coordsToCursorPosition(coordsOld, this->fieldOffsetX, this->fieldOffsetY, this->fieldCellWidth);
        gotoXY(getOffsetX() - 1 + 17, getOffsetY() - 2);
        std::cout << "Covered left: " << getCovered() <<  "     " << std::flush;
        gotoXY(tempCoords.col, tempCoords.row);
    }
#endif

// use 2D vectors for the visible game board and the internal mines field:
std::vector<std::vector<stringconv>> Field::create2DVector(std::string const& vectorType)
{
    Symbols symbols;

    std::vector<std::vector<stringconv>> temp2DVector;
    for(int i = 0; i <= this->cols; ++i)
    {
        std::vector<stringconv> row;
        for(int j = 0; j <= this->rows; ++j)
        {
            if (vectorType == "field")
            {
                row.push_back(symbols.symbolCovered);
            }
            else if (vectorType == "mines")
            {
                row.push_back(symbols.symbolZero);
            }
        }
        temp2DVector.push_back(row);
    }
    return temp2DVector;
}

// place mines at random positions of this->mines2DVector]:
void Field::fillMines(Common::CoordsStruct const& userFirstInput)
{
    Symbols symbols;

    #if STATIC_FIELD == 1
        this->mines2DVector[3][3] = symbols.symbolMine;
        this->mines2DVector[4][3] = symbols.symbolMine;
        this->minesTotal = 1;
    #else
        Common common;

        Common::CoordsStruct coords;
        size_t sizeofField2DVector = this->cols * this->rows;
        std::vector<int> tempVector;
        for (unsigned int i = 1; i <= sizeofField2DVector; ++i)
        {
            if (i != common.coordsToInt(userFirstInput, this->cols))
            {
                tempVector.push_back(i);
            }
        }

        std::random_device rng;
        std::mt19937 urng(rng());
        std::shuffle(tempVector.begin(), tempVector.end(), urng);
        
        for (int i = 0; i < this->minesTotal; ++i)
        {
            coords = common.intToCoords(tempVector.at(i), this->cols);
            this->mines2DVector[coords.col][coords.row] = symbols.symbolMine;
        }
    #endif
}

// draw the game board:
void Field::drawField()
{
    Symbols symbols;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        Common common;
        common.setUnicode(true);
    #endif

    for (int i = 0; i < this->fieldOffsetY - 4; ++i)
    {
        std::wcout << L"\n";
    }
    for (int i = 0; i < this->fieldOffsetX - 2; ++i)
    {
        std::wcout << L" ";
    }
    coutconv << symbols.symbolCornerTopLeft;
    for (int col = 1; col <= this->cols; ++col)
    {
        if (col < this->cols)
        {
            for (int width = 0; width < this->fieldCellWidth; ++width)
            {
                coutconv << symbols.symbolHorizontalLine;
            }
            coutconv << symbols.symbolDownT;
        }
        else
        {
            for (int width = 0; width < this->fieldCellWidth; ++width)
            {
                coutconv << symbols.symbolHorizontalLine;
            }
            coutconv << symbols.symbolCornerTopRight;
        }
    }
    std::wcout << L"\n";

    for (int row = 1; row <= this->rows; ++row)
    {
        for (int col = 1; col <= this->cols; ++col)
        {
            if (col == 1)
            {
                for (int i = 0; i < this->fieldOffsetX - 2; ++i)
                {
                    std::wcout << L" ";
                }
            }
            coutconv << symbols.symbolVerticalLine;

            for (int padding = 0; padding < (this->fieldCellWidth-1)/2; ++padding)
            {
                std::wcout << L" ";
            }

            Common::CoordsStruct Coords;
            Coords.col = col;
            Coords.row = row;

            printCoords(Coords, false);

            for (int padding = 0; padding < (this->fieldCellWidth-1)/2; ++padding)
            {
                std::wcout << L" ";
            }

            if (col == this->cols)
            {
                coutconv << symbols.symbolVerticalLine;
                std::wcout << L"\n";
                for (int i = 0; i < this->fieldOffsetX - 2; ++i)
                {
                    std::wcout << L" ";
                }
            }
        }

        if (row < this->rows)
        {
            coutconv << symbols.symbolRightT;
        }
        else
        {
            coutconv << symbols.symbolCornerBottomLeft;
        }

        for (int col = 1; col <= this->cols; ++col)
        {
            if (col < this->cols)
            {
                for (int width = 0; width < this->fieldCellWidth; ++width)
                {
                    coutconv << symbols.symbolHorizontalLine;
                }
                if (row < this->rows)
                {
                    coutconv << symbols.symbolPlus;
                }
                else
                {
                    coutconv << symbols.symbolUpT;
                }
            }
            else
            {
                for (int width = 0; width < this->fieldCellWidth; ++width)
                {
                    coutconv << symbols.symbolHorizontalLine;
                }
                if (row < this->rows)
                {
                    coutconv << symbols.symbolLeftT;
                }
                else
                {
                    coutconv << symbols.symbolCornerBottomRight;
                }
            }
        }
        std::wcout << L"\n";
    }
    coutconv << std::flush;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        common.setUnicode(false);
    #endif
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

// print the content of one particular cell at "coords":
void Field::printCoords(Common::CoordsStruct const& coords, bool isCursor)
{
    Colors colors;
    Common common;
    Symbols symbols;

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
                {
                    SetConsoleTextAttribute(hConsole, colors.fg_number_1);
                }
                else if (getCoordsContent(coords) == L"2")
                {
                    SetConsoleTextAttribute(hConsole, colors.fg_number_2);
                }
                else if (getCoordsContent(coords) == L"3")
                {
                    SetConsoleTextAttribute(hConsole, colors.fg_number_3);
                }
                else if (getCoordsContent(coords) == L"4")
                {
                    SetConsoleTextAttribute(hConsole, colors.fg_number_4);
                }
                else if (getCoordsContent(coords) == L"5")
                {
                    SetConsoleTextAttribute(hConsole, colors.fg_number_5);
                }
                else if (getCoordsContent(coords) == L"6")
                {
                    SetConsoleTextAttribute(hConsole, colors.fg_number_6);
                }
                else if (getCoordsContent(coords) == L"7")
                {
                    SetConsoleTextAttribute(hConsole, colors.fg_number_7);
                }
                else if (getCoordsContent(coords) == L"8")
                {
                    SetConsoleTextAttribute(hConsole, colors.fg_number_8);
                }
                else
                {
                    SetConsoleTextAttribute(hConsole, colors.color_default);
                }
            }
            else
            {
                if (getCoordsContent(coords) == L"1")
                {
                    SetConsoleTextAttribute(hConsole, colors.bg_number_1);
                }
                else if (getCoordsContent(coords) == L"2")
                {
                    SetConsoleTextAttribute(hConsole, colors.bg_number_2);
                }
                else if (getCoordsContent(coords) == L"3")
                {
                    SetConsoleTextAttribute(hConsole, colors.bg_number_3);
                }
                else if (getCoordsContent(coords) == L"4")
                {
                    SetConsoleTextAttribute(hConsole, colors.bg_number_4);
                }
                else if (getCoordsContent(coords) == L"5")
                {
                    SetConsoleTextAttribute(hConsole, colors.bg_number_5);
                }
                else if (getCoordsContent(coords) == L"6")
                {
                    SetConsoleTextAttribute(hConsole, colors.bg_number_6);
                }
                else if (getCoordsContent(coords) == L"7")
                {
                    SetConsoleTextAttribute(hConsole, colors.bg_number_7);
                }
                else if (getCoordsContent(coords) == L"8")
                {
                    SetConsoleTextAttribute(hConsole, colors.bg_number_8);
                }
                else
                {
                    SetConsoleTextAttribute(hConsole, colors.color_default);
                }
            }
        }
        else if (getCoordsContent(coords) == symbols.symbolFlag)
        {
            if (isCursor)
            {
                SetConsoleTextAttribute(hConsole, colors.bg_flag);
            }
            else
            {
                SetConsoleTextAttribute(hConsole, colors.fg_flag);
            }
        }
        else if (getCoordsContent(coords) == symbols.symbolMine)
        {
            SetConsoleTextAttribute(hConsole, colors.mine);
        }
        else if (getCoordsContent(coords) == symbols.symbolMineHit)
        {
            SetConsoleTextAttribute(hConsole, colors.minehit);
        }
        else if (getCoordsContent(coords) == symbols.symbolCovered)
        {
            if (isCursor)
            {
                SetConsoleTextAttribute(hConsole, colors.bg_covered);
            }
            else
            {
                SetConsoleTextAttribute(hConsole, colors.fg_covered);
            }
        }
        else if (getCoordsContent(coords) == symbols.symbolZero)
        {
            if (isCursor)
            {
                SetConsoleTextAttribute(hConsole, colors.bg_covered);
            }
        }
        std::wstring coordsString = this->field2DVector[coords.col][coords.row];
        WriteConsoleW(hConsole, coordsString.c_str(), static_cast<DWORD>(coordsString.size()), nullptr, nullptr);
        SetConsoleTextAttribute(hConsole, colors.color_default);
    #else
        std::string content;
        if (isNumber(coords))
        {
            if (isCursor == false)
            {
                if (getCoordsContent(coords) == "1")
                {
                    content = colors.fg_number_1 + "1";
                }
                else if (getCoordsContent(coords) == "2")
                {
                    content = colors.fg_number_2 + "2";
                }
                else if (getCoordsContent(coords) == "3")
                {
                    content = colors.fg_number_3 + "3";
                }
                else if (getCoordsContent(coords) == "4")
                {
                    content = colors.fg_number_4 + "4";
                }
                else if (getCoordsContent(coords) == "5")
                {
                    content = colors.fg_number_5 + "5";
                }
                else if (getCoordsContent(coords) == "6")
                {
                    content = colors.fg_number_6 + "6";
                }
                else if (getCoordsContent(coords) == "7")
                {
                    content = colors.fg_number_7 + "7";
                }
                else if (getCoordsContent(coords) == "8")
                {
                    content = colors.fg_number_8 + "8";
                }
                else
                {
                    content = colors.color_default + (this->field2DVector[coords.col][coords.row]);
                }
            }
            else
            {
                if (getCoordsContent(coords) == "1")
                {
                    content = colors.bg_number_1 + "1";
                }
                else if (getCoordsContent(coords) == "2")
                {
                    content = colors.bg_number_2 + "2";
                }
                else if (getCoordsContent(coords) == "3")
                {
                    content = colors.bg_number_3 + "3";
                }
                else if (getCoordsContent(coords) == "4")
                {
                    content = colors.bg_number_4 + "4";
                }
                else if (getCoordsContent(coords) == "5")
                {
                    content = colors.bg_number_5 + "5";
                }
                else if (getCoordsContent(coords) == "6")
                {
                    content = colors.bg_number_6 + "6";
                }
                else if (getCoordsContent(coords) == "7")
                {
                    content = colors.bg_number_7 + "7";
                }
                else if (getCoordsContent(coords) == "8")
                {
                    content = colors.bg_number_8 + "8";
                }
                else
                {
                    content = colors.color_default + (this->field2DVector[coords.col][coords.row]);
                }
            }
        }
        else if (getCoordsContent(coords) == symbols.symbolFlag)
        {
            if (isCursor)
            {
                content = colors.bg_flag + symbols.symbolFlag;
            }
            else
            {
                content = colors.fg_flag + symbols.symbolFlag;
            }
        }
        else if (getCoordsContent(coords) == symbols.symbolMine)
        {
            content = colors.mine + symbols.symbolMine;
        }
        else if (getCoordsContent(coords) == symbols.symbolMineHit)
        {
            content = colors.minehit + symbols.symbolMineHit;
        }
        else if (getCoordsContent(coords) == symbols.symbolCovered)
        {
            if (isCursor)
            {
                content = colors.bg_covered + symbols.symbolCovered;
            }
            else
            {
                content = colors.fg_covered + symbols.symbolCovered;
            }
        }
        else if (getCoordsContent(coords) == symbols.symbolZero)
        {
            if (isCursor)
            {
                content = colors.bg_covered + symbols.symbolCovered;
            }
            else
            {
                content = colors.bg_covered + symbols.symbolZero;
            }
        }
        else
        {
            content = colors.color_default + (this->field2DVector[coords.col][coords.row]);
        }

        coutconv << content;
        coutconv << colors.color_default << std::flush;
    #endif
}

// test coords if they contain a flag:
bool Field::isFlag(Common::CoordsStruct const& coords)
{
    Symbols symbols;

    if (this->field2DVector[coords.col][coords.row] == symbols.symbolFlag)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// test coords if they contain a number:
bool Field::isNumber(Common::CoordsStruct const& coords)
{
    Common common;
    for (int i = 1; i < 8; ++i)
    {
        if (this->field2DVector[coords.col][coords.row] == common.intToString(i))
        {
            return true;
        }
    }
    return false;
}

// find neighbors of a cell at "coords" that hold a given content (passed by variable symbol)
std::vector<Common::CoordsStruct> Field::findNeighbors(std::vector<std::vector<stringconv>> const& temp2DVector, Common::CoordsStruct const& coords, stringconv const& symbol)
{
    std::vector<Common::CoordsStruct> neighborsVector;

    // top left:
    if (coords.col-1 > 0 && coords.row-1 > 0)
    {
        if (temp2DVector[coords.col-1][coords.row-1] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col-1;
            tempCoords.row = coords.row-1;
            neighborsVector.push_back(tempCoords);
        }
    }

    // top middle:
    if (coords.row-1 > 0)
    {
        if (temp2DVector[coords.col][coords.row-1] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col;
            tempCoords.row = coords.row-1;
            neighborsVector.push_back(tempCoords);
        }
    }

    // top right:
    if (coords.col+1 <= this->cols && coords.row-1 > 0)
    {
        if (temp2DVector[coords.col+1][coords.row-1] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col+1;
            tempCoords.row = coords.row-1;
            neighborsVector.push_back(tempCoords);
        }
    }

    // center right:
    if (coords.col+1 <= this->cols)
    {
        if (temp2DVector[coords.col+1][coords.row] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col+1;
            tempCoords.row = coords.row;
            neighborsVector.push_back(tempCoords);
        }
    }

    // bottom right:
    if (coords.col+1 <= this->cols && coords.row+1 <= this->rows)
    {
        if (temp2DVector[coords.col+1][coords.row+1] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col+1;
            tempCoords.row = coords.row+1;
            neighborsVector.push_back(tempCoords);
        }
    }

    // bottom middle:
    if (coords.row+1 <= this->rows)
    {
        if (temp2DVector[coords.col][coords.row+1] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col;
            tempCoords.row = coords.row+1;
            neighborsVector.push_back(tempCoords);
        }
    }

    // bottom left:
    if (coords.col-1 > 0 && coords.row+1 <= this->rows)
    {
        if (temp2DVector[coords.col-1][coords.row+1] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col-1;
            tempCoords.row = coords.row+1;
            neighborsVector.push_back(tempCoords);
        }
    }

    // center left:
    if (coords.col-1 > 0)
    {
        if (temp2DVector[coords.col-1][coords.row] == symbol)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = coords.col-1;
            tempCoords.row = coords.row;
            neighborsVector.push_back(tempCoords);
        }
    }
    return neighborsVector;
}

void Field::gameWon()
{
    Input input;
    Print print;
    Symbols symbols;

    for (int i = 1; i <= this->cols; ++i)
    {
        for (int j = 1; j <= this->rows; ++j)
        {
            if (this->mines2DVector[i][j] == symbols.symbolMine)
            {
                this->field2DVector[i][j] = symbols.symbolMine;
            }
            Common::CoordsStruct tempCoords;
            tempCoords.col = i;
            tempCoords.row = j;
            printCoords(tempCoords, false);
        }
    }

    this->coveredLeft = 0;
    this->minesLeft = 0;

    #if DEBUG == 1
        Common::CoordsStruct dummyCoords;
        dummyCoords.col = 0;
        dummyCoords.row = 0;
        debugPrintCountCovered(dummyCoords);
    #endif

    print.printHasWon(*this);
    gotoXY(this->fieldOffsetX - 1, this->fieldOffsetY + this->rows*2 + 3);
    input.getInputEnterKey("Press ENTER to get back...");
}

void Field::gameLost()
{
    Input input;
    Print print;
    Symbols symbols;

    for (int i = 1; i <= this->cols; ++i)
    {
        for (int j = 1; j <= this->rows; ++j)
        {
            if (this->field2DVector[i][j] == symbols.symbolFlag && this->mines2DVector[i][j] \
            != symbols.symbolMine && this->mines2DVector[i][j] != symbols.symbolMineHit)
            {
                this->mines2DVector[i][j] = this->field2DVector[i][j];
            }
        }
    }
    for (int i = 1; i <= this->cols; ++i)
    {
        for (int j = 1; j <= this->rows; ++j)
        {
            this->field2DVector[i][j] = this->mines2DVector[i][j];
            Common::CoordsStruct tempCoords;
            tempCoords.col = i;
            tempCoords.row = j;
            printCoords(tempCoords, false);
        }
    }

    this->coveredLeft = 0;
    //this->minesLeft = 0;

    #if DEBUG == 1
        Common::CoordsStruct dummyCoords;
        dummyCoords.col = 0;
        dummyCoords.row = 0;
        debugPrintCountCovered(dummyCoords);
    #endif

    print.printHasLost(*this);
    gotoXY(this->fieldOffsetX - 1, this->fieldOffsetY + this->rows*2 + 3);
    input.getInputEnterKey("Press ENTER to get back...");
}

// automatically uncover all connected cells, as long as they have no neighbor mines:
void Field::autoUncoverRecursive(Common::CoordsStruct const& coords, std::vector<unsigned int> &poolVector)
{
    Common common;
    Symbols symbols;

    // create vector holding covered neighbors:
    std::vector<Common::CoordsStruct> neighborsCoveredVector;
    neighborsCoveredVector = findNeighbors(this->field2DVector, coords, symbols.symbolCovered);

    for (size_t i = 0; i < neighborsCoveredVector.size(); ++i)
    {
        std::vector<Common::CoordsStruct> neighborsMinesVector;
        neighborsMinesVector = findNeighbors(this->mines2DVector, neighborsCoveredVector.at(i), symbols.symbolMine);
        if (std::find(poolVector.begin(), poolVector.end(), common.coordsToInt(neighborsCoveredVector.at(i), this->cols)) == poolVector.end())
        {
            if (neighborsMinesVector.size() == 0)
            {
                if (this->mines2DVector[neighborsCoveredVector.at(i).col][neighborsCoveredVector.at(i).row] != symbols.symbolMine)
                {
                    this->field2DVector[neighborsCoveredVector.at(i).col][neighborsCoveredVector.at(i).row] = symbols.symbolZero;
                }
            }
            else
            {
                if (this->mines2DVector[neighborsCoveredVector.at(i).col][neighborsCoveredVector.at(i).row] != symbols.symbolMine)
                {
                    this->field2DVector[neighborsCoveredVector.at(i).col][neighborsCoveredVector.at(i).row] = common.intToString(neighborsMinesVector.size());
                }
            }
            poolVector.push_back(common.coordsToInt(neighborsCoveredVector.at(i), this->cols));
            printCoords(neighborsCoveredVector.at(i), false);
            --this->coveredLeft;

            #if DEBUG == 1
                #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
                    Sleep(50);
                #else
                    usleep(50*1000);
                #endif
                debugPrintCountCovered(coords);
            #endif
        }

        if (neighborsMinesVector.size() == 0)
        {
            autoUncoverRecursive(neighborsCoveredVector.at(i), poolVector);
        }
    }
}

void Field::flagAutoUncover(Common::UserInputReturnStruct const& userInput, Common::PlaceUserInputReturnStruct &returnStruct)
{
    Common common;
    Symbols symbols;

    // create a new vector of surrounding mines:
    std::vector<Common::CoordsStruct> flagUncoverNeighborsMinesVector;
    flagUncoverNeighborsMinesVector = findNeighbors(this->mines2DVector, userInput.Coords, symbols.symbolMine);

    // create a new vector of surrounding flags:
    std::vector<Common::CoordsStruct> flagUncoverNeighborsFlagsVector;
    flagUncoverNeighborsFlagsVector = findNeighbors(this->field2DVector, userInput.Coords, symbols.symbolFlag);

    // create a new vector of surrounding covered squares:
    std::vector<Common::CoordsStruct> flagUncoverNeighborsCoveredVector;
    flagUncoverNeighborsCoveredVector = findNeighbors(this->field2DVector, userInput.Coords, symbols.symbolCovered);

    // create a new empty vector for missed mines:
    std::vector<Common::CoordsStruct> flagUncoverMissedMinesVector;

    // if player has placed some flags around UserInput.Coords:
    if (flagUncoverNeighborsFlagsVector.size() != 0)
    {
        // only proceed if the flag number matches the number of actual surrounding mines:
        if (flagUncoverNeighborsMinesVector.size() == flagUncoverNeighborsFlagsVector.size())
        {
            // for each covered neighbor of userInput.Coords check if the player has missed a mine
            // and add this mines position to flagUncoverMissedMinesVector:
            for (size_t i = 0; i < flagUncoverNeighborsCoveredVector.size(); ++i)
            {
                if (this->mines2DVector[flagUncoverNeighborsCoveredVector.at(i).col][flagUncoverNeighborsCoveredVector.at(i).row] == symbols.symbolMine)
                {
                    flagUncoverMissedMinesVector.push_back(flagUncoverNeighborsCoveredVector.at(i));
                }
            }
            // if there are missed mines, reveal the mines2DVector - player has lost:
            if (flagUncoverMissedMinesVector.size() != 0)
            {
                for (size_t i = 0; i < flagUncoverMissedMinesVector.size(); ++i)
                {
                    this->mines2DVector[flagUncoverMissedMinesVector.at(i).col][flagUncoverMissedMinesVector.at(i).row] = symbols.symbolMineHit;
                }
                returnStruct.hasLost = true;
            }
            // else if all flags are placed correctly:
            else
            {
                if (flagUncoverNeighborsMinesVector.size() == flagUncoverNeighborsFlagsVector.size())
                {
                    // create a pool of already uncovered cells, to avoid double checks within autoUncoverRecursive():
                    std::vector<unsigned int> poolVector;

                    // for each covered neighbor of userInput.Coords, print the number of surrounding mines:
                    for (size_t i = 0; i < flagUncoverNeighborsCoveredVector.size(); ++i)
                    {
                        Common::CoordsStruct tempCoords;
                        tempCoords.col = flagUncoverNeighborsCoveredVector.at(i).col;
                        tempCoords.row = flagUncoverNeighborsCoveredVector.at(i).row;
                        std::vector<Common::CoordsStruct> flagUncoverNeighborsCoveredMinesVector;
                        flagUncoverNeighborsCoveredMinesVector = findNeighbors(this->mines2DVector, tempCoords, symbols.symbolMine);

                        if (flagUncoverNeighborsCoveredMinesVector.size() == 0)
                        {
                            if (std::find(poolVector.begin(), poolVector.end(), common.coordsToInt(tempCoords, this->cols)) == poolVector.end())
                            {
                                this->field2DVector[tempCoords.col][tempCoords.row] = symbols.symbolZero;
                                printCoords(tempCoords, false);
                                poolVector.push_back(common.coordsToInt(tempCoords, this->cols));
                                --this->coveredLeft;
                                autoUncoverRecursive(tempCoords, poolVector);
                            }
                        }
                        else
                        {
                            if (std::find(poolVector.begin(), poolVector.end(), common.coordsToInt(tempCoords, this->cols)) == poolVector.end())
                            {
                                this->field2DVector[tempCoords.col][tempCoords.row] = common.intToString(static_cast<int>(flagUncoverNeighborsCoveredMinesVector.size()));
                                printCoords(tempCoords, false);
                                poolVector.push_back(common.coordsToInt(tempCoords, this->cols));
                                --this->coveredLeft;
                            }
                        }
                    }
                }
            }
        }
    }
}

// the main method of class Field which will alter this->field2DVector.
// Takes single coords from Input::UserInput method.
Common::PlaceUserInputReturnStruct Field::placeUserInput(Common::UserInputReturnStruct& userInput, int& turn)
{
    Common common;
    Symbols symbols;

    Common::PlaceUserInputReturnStruct returnStruct;

    // set or remove flag if requested
    if (userInput.isFlag == true)
    {
        if (isFlag(userInput.Coords) == true)
        {
            this->field2DVector[userInput.Coords.col][userInput.Coords.row] = symbols.symbolCovered;
            printCoords(userInput.Coords, false);
            --this->flagsCount;
            ++this->minesLeft;
            ++this->coveredLeft;
        }
        else
        {
            this->field2DVector[userInput.Coords.col][userInput.Coords.row] = symbols.symbolFlag;
            printCoords(userInput.Coords, false);
            ++this->flagsCount;
            --this->minesLeft;
            --this->coveredLeft;
        }
    }
    else if (! userInput.isAutoFlag)
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
            gameLost();
            returnStruct.hasLost = true;
            return returnStruct;
        }

        // if the player has pressed ENTER on a number and has placed all flags right,
        // uncover all surrounding safe positions:
        if (isNumber(userInput.Coords))
        {
            flagAutoUncover(userInput, returnStruct);
            if (returnStruct.hasLost == true)
            {
                gameLost();
                return returnStruct;
            }
        }
        else
        {
            // uncover the players choice and place the number of surrounding mines in it:
            std::vector<Common::CoordsStruct> neighborsMinesVector = findNeighbors(this->mines2DVector, userInput.Coords, symbols.symbolMine);
            if (neighborsMinesVector.size() == 0)
            {
                this->field2DVector[userInput.Coords.col][userInput.Coords.row] = symbols.symbolZero;
            }
            else
            {
                this->field2DVector[userInput.Coords.col][userInput.Coords.row] = common.intToString(static_cast<int>(neighborsMinesVector.size()));
            }
            printCoords(userInput.Coords, false);
            returnStruct.isTurn = true;
            --this->coveredLeft;

            // Call recursive method autoUncoverRecursive() to automatically uncover all connected cells, as long as
            // they have no neighbor mines:
            std::vector<Common::CoordsStruct> autoUncoverNeighborsMinesVector = findNeighbors(this->mines2DVector, userInput.Coords, symbols.symbolMine);
            if (autoUncoverNeighborsMinesVector.size() == 0)
            {
                // create a pool of already uncovered cells, to avoid double checks within autoUncoverRecursive():
                std::vector<unsigned int> poolVector;
                autoUncoverRecursive(userInput.Coords, poolVector);
            }
        }
    }

    // check if player has won:
    if (this->flagsCount + this->coveredLeft == this->minesTotal)
    {
        returnStruct.hasWon = true;
        gameWon();
    }

    return returnStruct;
}
