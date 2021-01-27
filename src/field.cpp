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
    this->difficultyString = difficultyString_;
    this->fieldArray = createArray();
    this->minesArray = createArray();
    clearFieldArray();
    clearMinesArray();
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
    return this->fieldArray[coords.col][coords.row];
}

// use 2D vectors for the game field:
std::vector<std::vector<stringconv>> Field::createArray()
{
    const int colsNum = this->cols + 1;
    const int rowsNum = this->rows + 1;
    std::vector<std::vector<stringconv>> tempArray;
    tempArray.resize(rowsNum, std::vector<stringconv>(colsNum));
    return tempArray;
}

// fill this->fieldArray[][] with symbolCovered
void Field::clearFieldArray()
{
    Symbols symbols;

    for (int i=0; i <= this->cols; i++)
    {
        for (int j=0; j <= this->rows; j++)
        {
            this->fieldArray[i][j] = symbols.symbolCovered;
        }
    }
}

// fill this->minesArray[][] with symbolZero
void Field::clearMinesArray()
{
    Symbols symbols;
    
    for (int i=0; i <= this->cols; i++)
    {
        for (int j=0; j <= this->rows; j++)
        {
            this->minesArray[i][j] = symbols.symbolZero;
        }
    }
}

// place mines at random positions of this->minesArray[][]:
void Field::fillMinesArray(CoordsStruct& userFirstInput)
{
    Common common;
    Symbols symbols;
    
    CoordsStruct coords;
    size_t sizeOfFieldArray = this->cols * this->rows;
    std::vector<int> tempVector;
    for (int i = 1; i <= sizeOfFieldArray; i++)
    {
        if (i != common.convCoordsToInt(userFirstInput, this->cols))
            tempVector.push_back(i);
    }

    std::random_shuffle(tempVector.begin(), tempVector.end());
    for (int i = 0; i < this->minesCount; i++)
    {
        coords = common.convIntToCoords(tempVector.at(i), this->cols);
        this->minesArray[coords.col][coords.row] = symbols.symbolMine;
    }
}

// draw this->fieldArray[][]:
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
            
            CoordsStruct Coords;
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
void Field::printCoords(CoordsStruct& coords, bool isCursor)
{
    Colors colors;
    Common common;
    Symbols symbols;
    
    std::string content;
    CoordsStruct coordsTemp;

    coordsTemp = common.convCoordsToCursorPosition(coords, this->fieldOffsetX, this->fieldOffsetY, this->fieldCellWidth);
    gotoXY(coordsTemp.col, coordsTemp.row);
    
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
                SetConsoleTextAttribute(hConsole, colors.bg_red);
            else
                SetConsoleTextAttribute(hConsole, colors.fg_red);
        }
        else if (getCoordsContent(coords) == symbols.symbolMineHit || getCoordsContent(coords) == symbols.symbolMine)
            SetConsoleTextAttribute(hConsole, colors.fg_red);
        std::wstring coordsString = this->fieldArray[coords.col][coords.row];
        WriteConsoleW(hConsole, coordsString.c_str(), static_cast<DWORD>(coordsString.size()), nullptr, nullptr);
        SetConsoleTextAttribute(hConsole, colors.color_default);
    #else
        if (isNumber(coords))
        {
            if (isCursor == false)
            {
                if (getCoordsContent(coords) == "1")
                    content = colors.fg_light_blue + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "2")
                    content = colors.fg_light_green + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "3")
                    content = colors.fg_light_red + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "4")
                    content = colors.fg_magenta + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "5")
                    content = colors.fg_yellow + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "6")
                    content = colors.fg_green + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "7")
                    content = colors.fg_light_red + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "8")
                    content = colors.fg_white + (this->fieldArray[coords.col][coords.row]);
                else
                    content = colors.color_default + (this->fieldArray[coords.col][coords.row]);
            }
            else
            {
                if (getCoordsContent(coords) == "1")
                    content = colors.bg_light_blue + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "2")
                    content = colors.bg_light_green + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "3")
                    content = colors.bg_light_red + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "4")
                    content = colors.bg_magenta + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "5")
                    content = colors.bg_yellow + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "6")
                    content = colors.bg_green + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "7")
                    content = colors.bg_light_red + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "8")
                    content = colors.bg_black + (this->fieldArray[coords.col][coords.row]);
                else
                    content = colors.color_default + (this->fieldArray[coords.col][coords.row]);
            }
        }
        else if (getCoordsContent(coords) == symbols.symbolFlag)
        {
            if (isCursor)
                content = colors.bg_red + (this->fieldArray[coords.col][coords.row]);
            else
                content = colors.fg_red + (this->fieldArray[coords.col][coords.row]);
        }
        else if (getCoordsContent(coords) == symbols.symbolMineHit || getCoordsContent(coords) == symbols.symbolMine)
            content = colors.fg_red + (this->fieldArray[coords.col][coords.row]);
        else
            content = colors.color_default + (this->fieldArray[coords.col][coords.row]);
            
        coutconv << content << std::flush;
        coutconv << colors.color_default << std::flush;
    #endif
}

// test coords if they contain a flag:
bool Field::isFlag(CoordsStruct& coords)
{
    Symbols symbols;
    
    if (this->fieldArray[coords.col][coords.row] == symbols.symbolFlag)
        return true;
    else 
        return false;
}

// test coords if they contain a number:
bool Field::isNumber(CoordsStruct& coords)
{
    Common common;
    for (int i = 1; i < 8; i++)
    {
        if (this->fieldArray[coords.col][coords.row] == common.intToString(i))
            return true;
    }
    return false;
}

// find neighbors of a cell at "coords" that hold a given content (passed by variable symbol)
std::vector<CoordsStruct> Field::findNeighbours(std::vector<std::vector<stringconv>> const& tempArray, CoordsStruct const& coords, stringconv const& symbol)
{
    std::vector<CoordsStruct> neighboursVector;
    
    // up left:
    if (coords.col-1 > 0 && coords.row-1 > 0)
    {
        if (tempArray[coords.col-1][coords.row-1] == symbol)
        {
            CoordsStruct coordsTemp;
            coordsTemp.col = coords.col-1;
            coordsTemp.row = coords.row-1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // up middle:
    if (coords.row-1 > 0)
    {
        if (tempArray[coords.col][coords.row-1] == symbol)
        {
            CoordsStruct coordsTemp;
            coordsTemp.col = coords.col;
            coordsTemp.row = coords.row-1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // up right:
    if (coords.col+1 <= this->cols && coords.row-1 > 0)
    {
        if (tempArray[coords.col+1][coords.row-1] == symbol)
        {
            CoordsStruct coordsTemp;
            coordsTemp.col = coords.col+1;
            coordsTemp.row = coords.row-1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // middle left:
    if (coords.col-1 > 0)
    {
        if (tempArray[coords.col-1][coords.row] == symbol)
        {
            CoordsStruct coordsTemp;
            coordsTemp.col = coords.col-1;
            coordsTemp.row = coords.row;
            neighboursVector.push_back(coordsTemp);
        }
    }
    
    // middle right:
    if (coords.col+1 <= this->cols)
    {
        if (tempArray[coords.col+1][coords.row] == symbol)
        {
            CoordsStruct coordsTemp;
            coordsTemp.col = coords.col+1;
            coordsTemp.row = coords.row;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // down left:
    if (coords.col-1 > 0 && coords.row+1 <= this->rows)
    {
        if (tempArray[coords.col-1][coords.row+1] == symbol)
        {
            CoordsStruct coordsTemp;
            coordsTemp.col = coords.col-1;
            coordsTemp.row = coords.row+1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // down middle:
    if (coords.row+1 <= this->rows)
    {
        if (tempArray[coords.col][coords.row+1] == symbol)
        {
            CoordsStruct coordsTemp;
            coordsTemp.col = coords.col;
            coordsTemp.row = coords.row+1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // down right:
    if (coords.col+1 <= this->cols && coords.row+1 <= this->rows)
    {
        if (tempArray[coords.col+1][coords.row+1] == symbol)
        {
            CoordsStruct coordsTemp;
            coordsTemp.col = coords.col+1;
            coordsTemp.row = coords.row+1;
            neighboursVector.push_back(coordsTemp);
        }
    }
    
    return neighboursVector;
}

// the main method of class Field which will alter the fieldArray:
PlaceUserInputReturnStruct Field::placeUserInput(UserInputReturnStruct& userInput, int& turn)
{
    Common common;
    Print print;
    Symbols symbols;
    
    PlaceUserInputReturnStruct returnStruct;
    std::vector<CoordsStruct> neighboursMinesVector;
    
    // set or remove flag if requested
    if (userInput.isFlag == true)
    {
        if (isFlag(userInput.Coords) == true)
        {
            this->fieldArray[userInput.Coords.col][userInput.Coords.row] = symbols.symbolCovered;
            printCoords(userInput.Coords, false);
            this->flagsCount--;
            this->minesLeft++;
            this->countCovered++;
        }
        else
        {
            this->fieldArray[userInput.Coords.col][userInput.Coords.row] = symbols.symbolFlag;
            printCoords(userInput.Coords, false);
            this->flagsCount++;
            this->minesLeft--;
            this->countCovered--;
        }
    }
    else
    {
        // fill the Field with random mines AFTER the player has placed his first guess:
        if (turn == 1)
        {
            fillMinesArray(userInput.Coords);
        }
        
        // check if the player hit a mine which ends the game:
        if (this->minesArray[userInput.Coords.col][userInput.Coords.row] == symbols.symbolMine)
        {
            this->minesArray[userInput.Coords.col][userInput.Coords.row] = symbols.symbolMineHit;
    
            common.clearScreen();
            print.printTitle(this->difficultyString, this->cols, this->rows, this->minesCount);
            for (int i = 1; i <= this->cols; i++)
            {
                for (int j = 1; j <= this->rows; j++)
                {
                    if (this->fieldArray[i][j] == symbols.symbolFlag && this->minesArray[i][j] != symbols.symbolMine && this->minesArray[i][j] != symbols.symbolMineHit) // && this->minesArray[i][j] == coveredSymbol)
                        this->minesArray[i][j] = this->fieldArray[i][j];
                }
            }
            for (int i = 1; i <= this->cols; i++)
            {
                for (int j = 1; j <= this->rows; j++)
                {
                    this->fieldArray[i][j] = this->minesArray[i][j];
                }
            }
            drawField();
    
            print.printHasLost();
            returnStruct.hasLost = true;
            return returnStruct;
        }
        
        // if the player has typed the coords of an already uncovered position and has placed all flags right,
        // uncover all surrounding safe positions:
        if (isNumber(userInput.Coords))
        {
            // create a new vector of surrounding mines:
            std::vector<CoordsStruct> autoUncoverNeighboursMinesVector;
            autoUncoverNeighboursMinesVector = findNeighbours(this->minesArray, userInput.Coords, symbols.symbolMine);
            
            // create a new vector of surrounding flags:
            std::vector<CoordsStruct> autoUncoverNeighboursFlagsVector;
            autoUncoverNeighboursFlagsVector = findNeighbours(this->fieldArray, userInput.Coords, symbols.symbolFlag);
            
            // create a new vector of surrounding covered squares:
            std::vector<CoordsStruct> autoUncoverNeighboursCoveredVector;
            autoUncoverNeighboursCoveredVector = findNeighbours(this->fieldArray, userInput.Coords, symbols.symbolCovered);
            
            // create a new empty vector for missed mines:
            std::vector<CoordsStruct> autoUncoverMissedMinesVector;
            
            // if player has placed some flags around UserInput.Coords:
            if (autoUncoverNeighboursFlagsVector.size() != 0)
            {
                // only proceed if the flag number matches the number of actual surrounding mines:
                if (autoUncoverNeighboursMinesVector.size() == autoUncoverNeighboursFlagsVector.size())
                {
                    // for each not uncovered neighbour of UserInput.Coords check if the player has missed a mine
                    // and add this mines position to autoUncoverMissedMinesVector:
                    for (int i = 0; i < static_cast<int>(autoUncoverNeighboursCoveredVector.size()); i++)
                    {
                        if (this->minesArray[autoUncoverNeighboursCoveredVector.at(i).col][autoUncoverNeighboursCoveredVector.at(i).row] == symbols.symbolMine)
                            autoUncoverMissedMinesVector.push_back(autoUncoverNeighboursCoveredVector.at(i));
                    }
                    // if there are missed mines, reveal the minesArray - player has lost:
                    if (autoUncoverMissedMinesVector.size() != 0)
                    {
                        for (int i = 0; i < static_cast<int>(autoUncoverMissedMinesVector.size()); i++)
                        {
                            this->minesArray[autoUncoverMissedMinesVector.at(i).col][autoUncoverMissedMinesVector.at(i).row] = symbols.symbolMineHit;
                        }
                        returnStruct.hasLost = 1;
                        common.clearScreen();
                        print.printTitle(this->difficultyString, this->cols, this->rows, this->minesCount);
                        for (int i = 1; i <= this->cols; i++)
                        {
                            for (int j = 1; j <= this->rows; j++)
                            {
                                if (this->fieldArray[i][j] == symbols.symbolFlag && this->minesArray[i][j] != symbols.symbolMine && this->minesArray[i][j] != symbols.symbolMineHit) // && this->minesArray[i][j] == coveredSymbol)
                                    this->minesArray[i][j] = this->fieldArray[i][j];
                            }
                        }
                        for (int i = 1; i <= this->cols; i++)
                        {
                            for (int j = 1; j <= this->rows; j++)
                            {
                                this->fieldArray[i][j] = this->minesArray[i][j];
                            }
                        }
                        std::cout << newline;
                        drawField();
                
                        print.printHasLost();
                        return returnStruct;
                    }
                    // else if all flags are placed correctly:
                    else
                    {
                        if (autoUncoverNeighboursMinesVector.size() == autoUncoverNeighboursFlagsVector.size())
                        {
                            // for each not uncovered neighbour of UserInput.Coords, print the number of surrounding mines:
                            for (int i = 0; i < static_cast<int>(autoUncoverNeighboursCoveredVector.size()); i++)
                            {
                                CoordsStruct coordsTemp;
                                coordsTemp.col = autoUncoverNeighboursCoveredVector.at(i).col;
                                coordsTemp.row = autoUncoverNeighboursCoveredVector.at(i).row;
                                std::vector<CoordsStruct> autoUncoverNeighboursCoveredMinesVector;
                                autoUncoverNeighboursCoveredMinesVector = findNeighbours(this->minesArray, coordsTemp, symbols.symbolMine);
                                if (autoUncoverNeighboursCoveredMinesVector.size() == 0)
                                    this->fieldArray[coordsTemp.col][coordsTemp.row] = symbols.symbolZero;
                                else
                                    this->fieldArray[coordsTemp.col][coordsTemp.row] = common.intToString(static_cast<int>(autoUncoverNeighboursCoveredMinesVector.size()));
                                printCoords(coordsTemp, false);
                                this->countCovered--;
                            }
                        }
                    
                    }
                }
            }
        }
        else
        {
            // uncover the players choice and place the number of surrounding mines in it:
            neighboursMinesVector = findNeighbours(this->minesArray, userInput.Coords, symbols.symbolMine);
            if (neighboursMinesVector.size() == 0)
                this->fieldArray[userInput.Coords.col][userInput.Coords.row] = symbols.symbolZero;
            else
                this->fieldArray[userInput.Coords.col][userInput.Coords.row] = common.intToString(static_cast<int>(neighboursMinesVector.size()));
            printCoords(userInput.Coords, false);
            this->countCovered--;
            returnStruct.isTurn = true;
        }
        
        // automatically uncover all neighbour squares of squares containing symbolZero
        bool run = true;
        if (neighboursMinesVector.size() == 0)
        {
            while (run == true)
            {
                // for each free position do:
                for (int i = 1; i <= this->cols; i++)
                {
                    for (int j = 1; j <= this->rows; j++)
                    {
                        if (this->fieldArray[i][j] == symbols.symbolCovered)
                        {
                            // create a new vector of neighbours containing symbolZero:
                            CoordsStruct coordsBase;
                            coordsBase.col = i;
                            coordsBase.row = j;
                            std::vector<CoordsStruct> neighboursZerosVector;
                            neighboursZerosVector = findNeighbours(this->fieldArray, coordsBase, symbols.symbolZero);

                            // if there is a neighbour containing symbolZero create a new vector of neighbours containing mines:
                            if (neighboursZerosVector.size() != 0)
                            {
                                std::vector<CoordsStruct> neighboursMinesVectorNew;
                                neighboursMinesVectorNew = findNeighbours(this->minesArray, coordsBase, symbols.symbolMine);
        
                                // place neighboursMinesVectorNew.size() in fieldArray:
                                if (this->fieldArray[i][j] == symbols.symbolCovered)
                                {
                                    CoordsStruct coordsTemp;
                                    coordsTemp.col = i;
                                    coordsTemp.row = j;
                                    if (neighboursMinesVectorNew.size() == 0)
                                        this->fieldArray[i][j] = symbols.symbolZero;
                                    else
                                        this->fieldArray[i][j] = common.intToString(static_cast<int>(neighboursMinesVectorNew.size()));
                                    printCoords(coordsTemp, false);
                                    this->countCovered--;
                                }
                            }
                        }
                    }
                }
                run = false;
                
                // repeat if necessary:
                for (int a = 1; a <= this->cols; a++)
                {
                    for (int b = 1; b <= this->rows; b++)
                    {
                        if (this->fieldArray[a][b] == symbols.symbolCovered)
                        {
                            // create a new vector of neighbours containing symbolZero:
                            CoordsStruct coordsBaseNew;
                            coordsBaseNew.col = a;
                            coordsBaseNew.row = b;
                            std::vector<CoordsStruct> neighboursZerosVectorNew;
                            neighboursZerosVectorNew = findNeighbours(this->fieldArray, coordsBaseNew, symbols.symbolZero);
                            if (neighboursZerosVectorNew.size() != 0)
                                run = true;
                        }
                    }
                }
            }
        }
    }
    // check if player has won:
    if (this->flagsCount + this->countCovered == this->minesCount)
    {
        common.clearScreen();
        print.printTitle(this->difficultyString, this->cols, this->rows, this->minesCount);
        for (int i = 1; i <= this->cols; i++)
        {
            for (int j = 1; j <= this->rows; j++)
                if (this->minesArray[i][j] == symbols.symbolMine)
                    this->fieldArray[i][j] = symbols.symbolMine;
        }
        
        drawField();
        print.printHasWon();
        returnStruct.hasWon = true;
    }
    return returnStruct;
}
