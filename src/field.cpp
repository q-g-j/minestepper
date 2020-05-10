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
    // delete pointer (2D-)arrays to avoid memory leaks:
    for (int i=0; i <= cols; i++)
        delete[] this->fieldArray[i];
    delete[] this->fieldArray;
    
    for (int i=0; i <= cols; i++)
        delete[] this->minesArray[i];
    delete[] this->minesArray;
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

stringconv Field::getCoordsContent(Common::coordsStruct const& coords)
{
    return this->fieldArray[coords.col][coords.row];
}

// use pointers to pointers to be able to create dynamic 2D-arrays
// (could use vectors, but will leave this for now):
stringconv** Field::createArray()
{
    const int colsNum = this->cols + 1;
    const int rowsNum = this->rows + 1;
    stringconv** tempArray = nullptr;
    tempArray = new stringconv*[colsNum];
    for (int i=0; i < colsNum; i++)
        tempArray[i] = new stringconv[rowsNum];
    return tempArray;
}

// fill this->fieldArray[][] with coveredSymbol
void Field::clearFieldArray()
{
    for (int i=0; i <= this->cols; i++)
    {
        for (int j=0; j <= this->rows; j++)
        {
            this->fieldArray[i][j] = symbolCovered;
        }
    }
}

// fill this->minesArray[][] with " "
void Field::clearMinesArray()
{
    for (int i=0; i <= this->cols; i++)
    {
        for (int j=0; j <= this->rows; j++)
        {
            this->minesArray[i][j] = symbolUncovered;
        }
    }
}

// place mines at random positions of this->minesArray[][]:
void Field::fillMinesArray(coordsStruct& userFirstInput)
{
    Common common;
    coordsStruct coords;
    int sizeOfFieldArray = this->cols * this->rows;
    std::vector<int> tempVector;
    for (int i = 1; i <= sizeOfFieldArray; i++)
    {
        if (i != common.structToInt(userFirstInput, this->cols))
            tempVector.push_back(i);
    }

    std::random_shuffle(tempVector.begin(), tempVector.end());
    for (int i = 0; i < this->minesCount; i++)
    {
        coords = common.intToStruct(tempVector.at(i), this->cols);
        this->minesArray[coords.col][coords.row] = symbolMine;
    }
}

// draw this->fieldArray[][]:
void Field::drawField()
{
    Common common;
    common.setUnicode(true);

    for (int i = 0; i < this->fieldOffsetY - 4; i++)
        std::wcout << L"\n";
    for (int i = 0; i < this->fieldOffsetX - 2; i++)
        std::wcout << L" ";
    coutconv << symbolCornerTopLeft;
    for (int col = 1; col <= this->cols; col++)
    {
        if (col < this->cols)
        {
            for (int width = 0; width < this->fieldCellWidth; width++)
                coutconv << symbolHorizontalLine;
            coutconv << symbolDownT;
        }
        else
        {
            for (int width = 0; width < this->fieldCellWidth; width++)
                coutconv << symbolHorizontalLine;
            coutconv << symbolCornerTopRight;
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
            coutconv << symbolVerticalLine;
            
            for (int padding = 0; padding < (this->fieldCellWidth-1)/2; padding++)
                std::wcout << L" ";
            
            coordsStruct Coords;
            Coords.col = col;
            Coords.row = row;
            printCoords(Coords, false);
            
            for (int padding = 0; padding < (this->fieldCellWidth-1)/2; padding++)
                std::wcout << L" ";
            
            if (col == this->cols)
            {
                coutconv << symbolVerticalLine;      
                std::wcout << L"\n";
                for (int i = 0; i < this->fieldOffsetX - 2; i++)
                    std::wcout << L" ";
            }
        }

        if (row < this->rows)
            coutconv << symbolRightT;
        else
            coutconv << symbolCornerBottomLeft;
        
        for (int col = 1; col <= this->cols; col++)
        {
            if (col < this->cols)
            {
                for (int width = 0; width < this->fieldCellWidth; width++)
                    coutconv << symbolHorizontalLine;
                if (row < this->rows)
                    coutconv << symbolPlus;
                else
                    coutconv << symbolUpT;
            }
            else
            {
                for (int width = 0; width < this->fieldCellWidth; width++)
                    coutconv << symbolHorizontalLine;
                if (row < this->rows)
                    coutconv << symbolLeftT;
                else
                    coutconv << symbolCornerBottomRight;
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
void Field::printCoords(coordsStruct& coords, bool isCursor)
{
    Common common;
    
    std::string content;
    coordsStruct coordsTemp;
    coordsTemp = common.convCoordsToCursorPosition(coords, this->fieldOffsetX, this->fieldOffsetY, this->fieldCellWidth);
    gotoXY(coordsTemp.col, coordsTemp.row);
    
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (isNumber(coords))
        {
            if (isCursor == false)
            {
                if (getCoordsContent(coords) == L"1")
                    SetConsoleTextAttribute(hConsole, fg_light_blue);
                else if (getCoordsContent(coords) == L"2")
                    SetConsoleTextAttribute(hConsole, fg_light_green);
                else if (getCoordsContent(coords) == L"3")
                    SetConsoleTextAttribute(hConsole, fg_light_red);
                else if (getCoordsContent(coords) == L"4")
                    SetConsoleTextAttribute(hConsole, fg_magenta);
                else if (getCoordsContent(coords) == L"5")
                    SetConsoleTextAttribute(hConsole, fg_yellow);
                else if (getCoordsContent(coords) == L"6")
                    SetConsoleTextAttribute(hConsole, fg_green);
                else if (getCoordsContent(coords) == L"7")
                    SetConsoleTextAttribute(hConsole, fg_cyan);
                else if (getCoordsContent(coords) == L"8")
                    SetConsoleTextAttribute(hConsole, fg_white);
                else
                    SetConsoleTextAttribute(hConsole, color_default);
            }
            else
            {
                if (getCoordsContent(coords) == L"1")
                    SetConsoleTextAttribute(hConsole, bg_light_blue);
                else if (getCoordsContent(coords) == L"2")
                    SetConsoleTextAttribute(hConsole, bg_light_green);
                else if (getCoordsContent(coords) == L"3")
                    SetConsoleTextAttribute(hConsole, bg_light_red);
                else if (getCoordsContent(coords) == L"4")
                    SetConsoleTextAttribute(hConsole, bg_magenta);
                else if (getCoordsContent(coords) == L"5")
                    SetConsoleTextAttribute(hConsole, bg_yellow);
                else if (getCoordsContent(coords) == L"6")
                    SetConsoleTextAttribute(hConsole, bg_green);
                else if (getCoordsContent(coords) == L"7")
                    SetConsoleTextAttribute(hConsole, bg_cyan);
                else if (getCoordsContent(coords) == L"8")
                    SetConsoleTextAttribute(hConsole, bg_black);
                else
                    SetConsoleTextAttribute(hConsole, color_default);
            }
        }
        else if (getCoordsContent(coords) == symbolFlag)
        {
            if (isCursor)
                SetConsoleTextAttribute(hConsole, bg_red);
            else
                SetConsoleTextAttribute(hConsole, fg_red);
        }
        else if (getCoordsContent(coords) == symbolMineHit || getCoordsContent(coords) == symbolMine)
            SetConsoleTextAttribute(hConsole, fg_red);
        std::wstring coordsString = this->fieldArray[coords.col][coords.row];
        WriteConsoleW(hConsole, coordsString.c_str(), static_cast<DWORD>(coordsString.size()), nullptr, nullptr);
        SetConsoleTextAttribute(hConsole, color_default);
    #else
        if (isNumber(coords))
        {
            if (isCursor == false)
            {
                if (getCoordsContent(coords) == "1")
                    content = fg_light_blue + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "2")
                    content = fg_light_green + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "3")
                    content = fg_light_red + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "4")
                    content = fg_magenta + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "5")
                    content = fg_yellow + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "6")
                    content = fg_green + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "7")
                    content = fg_light_red + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "8")
                    content = fg_white + (this->fieldArray[coords.col][coords.row]);
                else
                    content = color_default + (this->fieldArray[coords.col][coords.row]);
            }
            else
            {
                if (getCoordsContent(coords) == "1")
                    content = bg_light_blue + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "2")
                    content = bg_light_green + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "3")
                    content = bg_light_red + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "4")
                    content = bg_magenta + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "5")
                    content = bg_yellow + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "6")
                    content = bg_green + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "7")
                    content = bg_light_red + (this->fieldArray[coords.col][coords.row]);
                else if (getCoordsContent(coords) == "8")
                    content = bg_black + (this->fieldArray[coords.col][coords.row]);
                else
                    content = color_default + (this->fieldArray[coords.col][coords.row]);
            }
        }
        else if (getCoordsContent(coords) == symbolFlag)
        {
            if (isCursor)
                content = bg_red + (this->fieldArray[coords.col][coords.row]);
            else
                content = fg_red + (this->fieldArray[coords.col][coords.row]);
        }
        else if (getCoordsContent(coords) == symbolMineHit || getCoordsContent(coords) == symbolMine)
            content = fg_red + (this->fieldArray[coords.col][coords.row]);
        else
            content = color_default + (this->fieldArray[coords.col][coords.row]);
            
        coutconv << content << std::flush;
        coutconv << color_default << std::flush;
    #endif
}

// test coords if they contain a flag:
bool Field::isFlagSet(coordsStruct& coords)
{
    if (this->fieldArray[coords.col][coords.row] == symbolFlag)
        return true;
    else 
        return false;
}

// test coords if they contain a number:
bool Field::isNumber(coordsStruct& coords)
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
std::vector<coordsStruct> Field::findNeighbours(stringconv **tempArray, coordsStruct const& coords, stringconv const& symbol)
{
    std::vector<coordsStruct> neighboursVector;
    
    // up left:
    if (coords.col-1 > 0 && coords.row-1 > 0)
    {
        if (tempArray[coords.col-1][coords.row-1] == symbol)
        {
            coordsStruct coordsTemp;
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
            coordsStruct coordsTemp;
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
            coordsStruct coordsTemp;
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
            coordsStruct coordsTemp;
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
            coordsStruct coordsTemp;
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
            coordsStruct coordsTemp;
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
            coordsStruct coordsTemp;
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
            coordsStruct coordsTemp;
            coordsTemp.col = coords.col+1;
            coordsTemp.row = coords.row+1;
            neighboursVector.push_back(coordsTemp);
        }
    }
    
    return neighboursVector;
}

// the main method of class Field which will alter the fieldArray:
placeUserInputReturnStruct Field::placeUserInput(userInputReturnStruct& UserInput, int& turn)
{
    Common common;
    Print print;
    placeUserInputReturnStruct returnStruct;
    std::vector<coordsStruct> neighboursMinesVector;
    
    // set or remove flag if requested
    if (UserInput.isFlag == true)
    {
        if (isFlagSet(UserInput.Coords) == true)
        {
            this->fieldArray[UserInput.Coords.col][UserInput.Coords.row] = symbolCovered;
            printCoords(UserInput.Coords, false);
            this->flagsCount--;
            this->minesLeft++;
            this->countCovered++;
        }
        else
        {
            this->fieldArray[UserInput.Coords.col][UserInput.Coords.row] = symbolFlag;
            printCoords(UserInput.Coords, false);
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
            fillMinesArray(UserInput.Coords);
        }
        
        // check if the player hit a mine which ends the game:
        if (this->minesArray[UserInput.Coords.col][UserInput.Coords.row] == symbolMine)
        {
            this->minesArray[UserInput.Coords.col][UserInput.Coords.row] = symbolMineHit;
    
            common.clearScreen();
            print.printTitle(this->difficultyString, this->cols, this->rows, this->minesCount);
            for (int i = 1; i <= this->cols; i++)
            {
                for (int j = 1; j <= this->rows; j++)
                {
                    if (this->fieldArray[i][j] == symbolFlag && this->minesArray[i][j] != symbolMine && this->minesArray[i][j] != symbolMineHit) // && this->minesArray[i][j] == coveredSymbol)
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
        if (isNumber(UserInput.Coords))
        {
            // create a new vector of surrounding mines:
            std::vector<coordsStruct> autoUncoverNeighboursMinesVector;
            autoUncoverNeighboursMinesVector = findNeighbours(this->minesArray, UserInput.Coords, symbolMine);
            
            // create a new vector of surrounding flags:
            std::vector<coordsStruct> autoUncoverNeighboursFlagsVector;
            autoUncoverNeighboursFlagsVector = findNeighbours(this->fieldArray, UserInput.Coords, symbolFlag);
            
            // create a new vector of surrounding covered squares:
            std::vector<coordsStruct> autoUncoverNeighboursCoveredVector;
            autoUncoverNeighboursCoveredVector = findNeighbours(this->fieldArray, UserInput.Coords, symbolCovered);
            
            // create a new empty vector for missed mines:
            std::vector<coordsStruct> autoUncoverMissedMinesVector;
            
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
                        if (this->minesArray[autoUncoverNeighboursCoveredVector.at(i).col][autoUncoverNeighboursCoveredVector.at(i).row] == symbolMine)
                            autoUncoverMissedMinesVector.push_back(autoUncoverNeighboursCoveredVector.at(i));
                    }
                    // if there are missed mines, reveal the minesArray - player has lost:
                    if (autoUncoverMissedMinesVector.size() != 0)
                    {
                        for (int i = 0; i < static_cast<int>(autoUncoverMissedMinesVector.size()); i++)
                        {
                            this->minesArray[autoUncoverMissedMinesVector.at(i).col][autoUncoverMissedMinesVector.at(i).row] = symbolMineHit;
                        }
                        returnStruct.hasLost = 1;
                        common.clearScreen();
                        print.printTitle(this->difficultyString, this->cols, this->rows, this->minesCount);
                        for (int i = 1; i <= this->cols; i++)
                        {
                            for (int j = 1; j <= this->rows; j++)
                            {
                                if (this->fieldArray[i][j] == symbolFlag && this->minesArray[i][j] != symbolMine && this->minesArray[i][j] != symbolMineHit) // && this->minesArray[i][j] == coveredSymbol)
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
                                coordsStruct coordsTemp;
                                coordsTemp.col = autoUncoverNeighboursCoveredVector.at(i).col;
                                coordsTemp.row = autoUncoverNeighboursCoveredVector.at(i).row;
                                std::vector<coordsStruct> autoUncoverNeighboursCoveredMinesVector;
                                autoUncoverNeighboursCoveredMinesVector = findNeighbours(this->minesArray, coordsTemp, symbolMine);
                                if (autoUncoverNeighboursCoveredMinesVector.size() == 0)
                                    this->fieldArray[coordsTemp.col][coordsTemp.row] = symbolUncovered;
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
            neighboursMinesVector = findNeighbours(this->minesArray, UserInput.Coords, symbolMine);
            if (neighboursMinesVector.size() == 0)
                this->fieldArray[UserInput.Coords.col][UserInput.Coords.row] = symbolUncovered;
            else
                this->fieldArray[UserInput.Coords.col][UserInput.Coords.row] = common.intToString(static_cast<int>(neighboursMinesVector.size()));
            printCoords(UserInput.Coords, false);
            this->countCovered--;
            returnStruct.isTurn = true;
        }
        
        // automatically uncover all neighbour squares of squares containing a " " (repeat if new " "s appeared):
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
                        if (this->fieldArray[i][j] == symbolCovered)
                        {
                            // create a new vector of neighbours containing " ":
                            coordsStruct coordsBase;
                            coordsBase.col = i;
                            coordsBase.row = j;
                            std::vector<coordsStruct> neighboursZerosVector;
                            neighboursZerosVector = findNeighbours(this->fieldArray, coordsBase, symbolUncovered);
                            

                            // if there is a neighbour containing a " " create a new vector of neighbours containing mines:
                            if (neighboursZerosVector.size() != 0)
                            {
                                std::vector<coordsStruct> neighboursMinesVectorNew;
                                neighboursMinesVectorNew = findNeighbours(this->minesArray, coordsBase, symbolMine);
        
                                // place neighboursMinesVectorNew.size() in fieldArray:
                                if (this->fieldArray[i][j] == symbolCovered)
                                {
                                    coordsStruct coordsTemp;
                                    coordsTemp.col = i;
                                    coordsTemp.row = j;
                                    if (neighboursMinesVectorNew.size() == 0)
                                        this->fieldArray[i][j] = symbolUncovered;
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
                        if (this->fieldArray[a][b] == symbolCovered)
                        {
                            // create a new vector of neighbours containing " ":
                            coordsStruct coordsBaseNew;
                            coordsBaseNew.col = a;
                            coordsBaseNew.row = b;
                            std::vector<coordsStruct> neighboursZerosVectorNew;
                            neighboursZerosVectorNew = findNeighbours(this->fieldArray, coordsBaseNew, symbolUncovered);
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
                if (this->minesArray[i][j] == symbolMine)
                    this->fieldArray[i][j] = symbolMine;
        }
        
        drawField();
        print.printHasWon();
        returnStruct.hasWon = true;
    }
    return returnStruct;
}
