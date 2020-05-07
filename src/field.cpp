#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#endif

#include "common.hpp"
#include "debug.hpp"
#include "field.hpp"
#include "input.hpp"

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
    this->cursorArray = createArray();
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
    
    for (int i=0; i <= cols; i++)
        delete[] this->cursorArray[i];
    delete[] this->cursorArray;
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

int Field::getMinesLeft()
{
    return this->minesLeft;
}

stringconv Field::getCoordsContent(Common::coordsStruct const& coords)
{
    return this->fieldArray[coords.col][coords.row];
}

// use pointers to pointers to be able to create dynamic 2D-arrays
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

// draw this->fieldArray[][] or this->minesArray[][]:
void Field::drawField(stringconv** array)
{
    Common common;
    common.setUnicode(true);

    std::wcout << L"\n";
    std::wcout << L"   ";
    coutconv << symbolCornerTopLeft;
    for (int col = 1; col <= this->cols; col++)
    {
        if (col < this->cols)
        {
            for (int width = 0; width < this->fieldCellWidth; width++) coutconv << symbolHorizontalLine;
            coutconv << symbolDownT;
        }
        else
        {
            for (int width = 0; width < this->fieldCellWidth; width++) coutconv << symbolHorizontalLine;
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
                std::wcout << L"   ";
            }
            coutconv << symbolVerticalLine;
            
            for (int padding = 0; padding < (this->fieldCellWidth-1)/2; padding++)
                std::wcout << L" ";
            
            coutconv << array[col][row]; // <----------------------------------------------
            
            for (int padding = 0; padding < (this->fieldCellWidth-1)/2; padding++)
                std::wcout << L" ";
            
            if (col == this->cols)
            {
                coutconv << symbolVerticalLine;      
                std::wcout << L"\n";
                std::wcout << L"   ";
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
                for (int width = 0; width < this->fieldCellWidth; width++) coutconv << symbolHorizontalLine;
                if (row < this->rows)
                    coutconv << symbolPlus;
                else
                    coutconv << symbolUpT;
            }
            else
            {
                for (int width = 0; width < this->fieldCellWidth; width++) coutconv << symbolHorizontalLine;
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

        coordsNew.X = x-1;
        coordsNew.Y = y-1;
        SetConsoleCursorPosition(hConsole, coordsNew);
    #else
        printf("%c[%d;%df",0x1B,y,x);
    #endif
}

void Field::printCoords(coordsStruct& coords)
{
    coordsStruct coordsTemp;
    coordsTemp.col = getOffsetX();
    for (int i = 1; i < coords.col; i ++)
        coordsTemp.col = coordsTemp.col + (getCellWidth() + 1);
    
    coordsTemp.row = getOffsetY();
    for (int i = 1; i < coords.row; i ++)
        coordsTemp.row = coordsTemp.row + 2;
    
    gotoXY(coordsTemp.col, coordsTemp.row);
    
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        std::wstring coordsString = this->fieldArray[coords.col][coords.row];
        WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), coordsString.c_str(), coordsString.size(), nullptr, nullptr);
    #else
        coutconv << (this->fieldArray[coords.col][coords.row]) << std::flush;
    #endif
}

void Field::printHasWon()
{
    Common common;
    Input input;
    
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->minesCount << " mines" << newline << newline;
    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
            if (this->minesArray[i][j] == symbolMine)
                this->fieldArray[i][j] = symbolMine;
    }
    
    std::cout << newline;
    
    drawField(this->fieldArray);
    std::cout << newline;
    std::cout << "Congratulation, you have won!" << newline;
    std::string goBackString =  "Press ENTER to go back...";
    input.getEnterKey(goBackString);
    
}

void Field::printHasLost()
{
    Common common;
    Input input;
    
    std::string wrongInputText = "Press ENTER to go back...";
    
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->minesCount << " mines" << newline << newline;
    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            if (this->fieldArray[i][j] == symbolFlag && this->minesArray[i][j] != symbolMine && this->minesArray[i][j] != symbolMineHit) // && this->minesArray[i][j] == coveredSymbol)
                 this->minesArray[i][j] = this->fieldArray[i][j];
        }
    }
    std::cout << newline;
    drawField(this->minesArray);
    
    std::cout << newline;
    std::cout << "Sry, you have lost!" << newline;
    std::cout << wrongInputText;
    input.getEnterKey("");
}

void Field::printExplanation()
{
    std::cout << "Minestepper - A Minesweeper Clone" << newline << newline;
    std::cout << "In this game your task is to find all hidden mines by uncovering all safe positions." << newline << newline;
    std::cout << "You can guess and sometimes combine where the next mine is." << newline;
    std::cout << "The number on each uncovered square shows how many neighbours contain a mine." << newline << newline;
    std::cout << "If you're sure that you have found a mine, place a flag on it's position by pressing SPACE" << newline;
    std::cout << "To remove the flag, just repeat your input. You may place or remove as many flags in a row as you wish." << newline << newline;
    std::cout << "You can reselect a numbered cell with ENTER to automatically uncover all remaining neighbours," << newline;
    std::cout << "as long as you put all flags right! Otherwise you might lose" << newline << newline;
    std::cout << "Explanations:" << newline << newline;
    std::cout << "Arrow Keys:    navigate" << newline;
    std::cout << "ENTER:         sweep" << newline;
    std::cout << "SPACE:         place or remove a flag" << newline;
    std::cout << "q or Q:        quit" << newline << newline;
    std::cout << "Press ENTER to go back...";
}

void Field::printAll()
{
    Common common;
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->minesCount << " mines" << newline << newline;
    std::cout << newline;
    
    /*#if DEBUG == 1
        drawField(this->minesArray);
        std::cout << newline;
    #endif*/
    
    drawField(this->fieldArray);
    std::cout << newline;
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

std::vector<coordsStruct> Field::findNeighbours(stringconv **tempArray, coordsStruct const& coords, stringconv const& mark)
{
    std::vector<coordsStruct> neighboursVector;
    
    // up left:
    if (coords.col-1 > 0 && coords.row-1 > 0)
    {
        if (tempArray[coords.col-1][coords.row-1] == mark)
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
        if (tempArray[coords.col][coords.row-1] == mark)
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
        if (tempArray[coords.col+1][coords.row-1] == mark)
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
        if (tempArray[coords.col-1][coords.row] == mark)
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
        if (tempArray[coords.col+1][coords.row] == mark)
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
        if (tempArray[coords.col-1][coords.row+1] == mark)
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
        if (tempArray[coords.col][coords.row+1] == mark)
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
        if (tempArray[coords.col+1][coords.row+1] == mark)
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
placeUserInputReturnStruct Field::placeUserInput(userInputReturnStruct& userInput, int& turn)
{
    Common common;
    
    placeUserInputReturnStruct returnStruct;
    std::vector<coordsStruct> neighboursMinesVector;
    
    // set or remove flag if requested
    if (userInput.isFlag == true)
    {
        if (isFlagSet(userInput.coords) == true)
        {
            this->fieldArray[userInput.coords.col][userInput.coords.row] = symbolCovered;
            printCoords(userInput.coords);
            this->flagsCount--;
            this->minesLeft++;
            this->countCovered++;
        }
        else
        {
            this->fieldArray[userInput.coords.col][userInput.coords.row] = symbolFlag;
            printCoords(userInput.coords);
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
            fillMinesArray(userInput.coords);
        }
        
        // check if the player hit a mine which ends the game:
        if (this->minesArray[userInput.coords.col][userInput.coords.row] == symbolMine)
        {
            this->minesArray[userInput.coords.col][userInput.coords.row] = symbolMineHit;
            printHasLost();
            returnStruct.hasLost = true;
            return returnStruct;
        }
        
        // if the player has typed the coords of an already uncovered position and has placed all flags right,
        // uncover all surrounding safe positions:
        if (isNumber(userInput.coords))
        {
            // create a new vector of surrounding mines:
            std::vector<coordsStruct> autoUncoverNeighboursMinesVector;
            autoUncoverNeighboursMinesVector = findNeighbours(this->minesArray, userInput.coords, symbolMine);
            
            // create a new vector of surrounding flags:
            std::vector<coordsStruct> autoUncoverNeighboursFlagsVector;
            autoUncoverNeighboursFlagsVector = findNeighbours(this->fieldArray, userInput.coords, symbolFlag);
            
            // create a new vector of surrounding covered squares:
            std::vector<coordsStruct> autoUncoverNeighboursCoveredVector;
            autoUncoverNeighboursCoveredVector = findNeighbours(this->fieldArray, userInput.coords, symbolCovered);
            
            // create a new empty vector for missed mines:
            std::vector<coordsStruct> autoUncoverMissedMinesVector;
            
            // if player has placed some flags around userInput.coords:
            if (autoUncoverNeighboursFlagsVector.size() != 0)
            {
                // only proceed if the flag number matches the number of actual surrounding mines:
                if (autoUncoverNeighboursMinesVector.size() == autoUncoverNeighboursFlagsVector.size())
                {
                    // for each not uncovered neighbour of userInput.coords check if the player has missed a mine
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
                        printHasLost();
                        return returnStruct;
                    }
                    // else if all flags are placed correctly:
                    else
                    {
                        if (autoUncoverNeighboursMinesVector.size() == autoUncoverNeighboursFlagsVector.size())
                        {
                            // for each not uncovered neighbour of userInput.coords, print the number of surrounding mines:
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
                                printCoords(coordsTemp);
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
            neighboursMinesVector = findNeighbours(this->minesArray, userInput.coords, symbolMine);
            if (neighboursMinesVector.size() == 0)
                this->fieldArray[userInput.coords.col][userInput.coords.row] = symbolUncovered;
            else
                this->fieldArray[userInput.coords.col][userInput.coords.row] = common.intToString(static_cast<int>(neighboursMinesVector.size()));
            printCoords(userInput.coords);
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
                                    printCoords(coordsTemp);
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
        printHasWon();
        returnStruct.hasWon = true;
    }
    return returnStruct;
}
