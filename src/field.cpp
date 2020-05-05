#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
    #include <io.h>
    #include <fcntl.h>
#endif

#include "common.hpp"
#include "debug.hpp"
#include "field.hpp"
#include "input.hpp"

// the constructor:
Field::Field(int cols, int rows, int fieldOffsetX, int fieldOffsetY, int fieldCellWidth, int minesCount, std::string difficultyString)
{
    this->cols = cols;
    this->rows = rows;
    this->fieldOffsetX = fieldOffsetX;
    this->fieldOffsetY = fieldOffsetY;
    this->fieldCellWidth = fieldCellWidth;
    this->minesCount = minesCount;
    this->minesLeft = minesCount;
    this->countEmpty = cols * rows;
    this->difficultyString = difficultyString;
    this->fieldArray = createArray();
    this->minesArray = createArray();
    this->clearFieldArray();
    this->clearMinesArray();
}

// the deconstructor:
Field::~Field()
{    
    for (int i=0; i <= cols; i++)
        delete[] this->fieldArray[i];
    delete[] this->fieldArray;
    
    for (int i=0; i <= cols; i++)
        delete[] this->minesArray[i];
    delete[] this->minesArray;
}

void Field::setFrameSymbols()
{    
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        cornerTopLeftSymbol = L"\u250C";
        cornerTopRightSymbol = L"\u2510";
        cornerBottomLeftSymbol = L"\u2514";
        cornerBottomRightSymbol = L"\u2518";
        horizontalLineSymbol = L"\u2500";
        verticalLineSymbol = L"\u2502";
        downTSymbol = L"\u252C";
        upTSymbol = L"\u2534";
        rightTSymbol = L"\u251C";
        leftTSymbol = L"\u2524";
        plusSymbol = L"\u253C";
    #else
        cornerTopLeftSymbol = "\u250C";
        cornerTopRightSymbol = "\u2510";
        cornerBottomLeftSymbol = "\u2514";
        cornerBottomRightSymbol = "\u2518";
        horizontalLineSymbol = "\u2500";
        verticalLineSymbol = "\u2502";
        downTSymbol = "\u252C";
        upTSymbol = "\u2534";
        rightTSymbol = "\u251C";
        leftTSymbol = "\u2524";
        plusSymbol = "\u253C";
    #endif
}

void Field::setCellSymbols()
{    
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        flagSymbol = L"\u25ba";
        bombSymbol = L"\u263c";
        bombHitSymbol = L"#";
        uncoveredSymbol = L" ";
        coveredSymbol = L"\u2591";
    #else
        flagSymbol = "\u25ba";
        bombSymbol = "\u263c";
        bombHitSymbol = "#";
        uncoveredSymbol = " ";
        coveredSymbol = "\u2591";
    #endif    
}

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

int Field::getMinesLeft()
{
    return this->minesLeft;
}

// use pointers to pointers to be able to create dynamic 2D-arrays
stringsym** Field::createArray()
{
    const int colsNum = this->cols + 1;
    const int rowsNum = this->rows + 1;
    stringsym** tempArray = new stringsym*[colsNum];
    for (int i=0; i <= this->cols; i++)
        tempArray[i] = new stringsym[rowsNum];
    return tempArray;
}

// fill this->fieldArray[][] with coveredSymbol
void Field::clearFieldArray()
{
    setCellSymbols();
    for (int i=0; i <= this->cols; i++)
    {
        for (int j=0; j <= this->rows; j++)
        {
            this->fieldArray[i][j] = coveredSymbol;
        }
    }
}

// fill this->minesArray[][] with " "
void Field::clearMinesArray()
{
    setCellSymbols();
    for (int i=0; i <= this->cols; i++)
    {
        for (int j=0; j <= this->rows; j++)
        {
            this->minesArray[i][j] = uncoveredSymbol;
        }
    }
}

// place mines at random positions of this->minesArray[][]:
void Field::fillMinesArray(coordsStruct userFirstInput)
{
    setCellSymbols();
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
        this->minesArray[coords.col][coords.row] = bombSymbol;
    }
}

// draw this->fieldArray[][] or this->minesArray[][]:
void Field::drawField(stringsym** array)
{
    setFrameSymbols();
    setCellSymbols();

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        _setmode(_fileno(stdout), _O_U16TEXT);
    #endif

    std::wcout << L"    ";
    for (int col = 1; col <= this->cols; col++)
    {
        if (col < 10)
            std::wcout << L"  " << col << L" ";
        else
            std::wcout << L" " << col << L" ";
    }
    std::wcout << L"\n";
    std::wcout << L"   ";
    coutsym << cornerTopLeftSymbol;
    for (int col = 1; col <= this->cols; col++)
    {
        if (col < this->cols)
        {
            for (int width = 0; width < this->fieldCellWidth; width++) coutsym << horizontalLineSymbol;
            coutsym << downTSymbol;
        }
        else
        {
            for (int width = 0; width < this->fieldCellWidth; width++) coutsym << horizontalLineSymbol;
            coutsym << cornerTopRightSymbol;
        }
    }
    std::wcout << L"\n";

    for (int row = 1; row <= this->rows; row++)
    {
        for (int col = 1; col <= this->cols; col++)
        {
            if (col == 1)
            {
                if (row < 10)
                    std::wcout << L" " << row << L" ";
                else
                    std::wcout << row << L" ";
            }
            coutsym << verticalLineSymbol;
            
            for (int padding = 0; padding < (this->fieldCellWidth-1)/2; padding++)
                std::wcout << L" ";
            
            coutsym << array[col][row]; // <----------------------------------------------
            
            for (int padding = 0; padding < (this->fieldCellWidth-1)/2; padding++)
                std::wcout << L" ";
            
            if (col == this->cols)
            {
                coutsym << verticalLineSymbol;
                if (row < 10)
                    std::wcout << L" " << row << L" ";
                else
                    std::wcout << row << L" ";        
                std::wcout << L"\n";
                std::wcout << L"   ";
            }
        }

        if (row < this->rows)
            coutsym << rightTSymbol;
        else
            coutsym << cornerBottomLeftSymbol;
        
        for (int col = 1; col <= this->cols; col++)
        {
            if (col < this->cols)
            {
                for (int width = 0; width < this->fieldCellWidth; width++) coutsym << horizontalLineSymbol;
                if (row < this->rows)
                    coutsym << plusSymbol;
                else
                    coutsym << upTSymbol;
            }
            else
            {
                for (int width = 0; width < this->fieldCellWidth; width++) coutsym << horizontalLineSymbol;
                if (row < this->rows)
                    coutsym << leftTSymbol;
                else
                    coutsym << cornerBottomRightSymbol;
            }
        }

        std::wcout << L"\n";
    }
    std::wcout << L"    ";
    for (int col = 1; col <= this->cols; col++)
    {
        if (col < 10)
            std::wcout << L"  " << col << L" ";
        else
            std::wcout << L" " << col << L" ";
    }
    std::wcout << std::endl;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        _setmode(_fileno(stdout), _O_TEXT);
    #endif
}

void Field::gotoXY(int x, int y)
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

void Field::printCoords(coordsStruct coords)
{
    gotoXY(this->fieldOffsetX + coords.col * 4, this->fieldOffsetY + coords.row * 2);
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        std::wstring coordsString = this->fieldArray[coords.col][coords.row];
        WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), coordsString.c_str(), coordsString.size(), nullptr, nullptr);
    #else
        coutsym << (this->fieldArray[coords.col][coords.row]);
        coutsym << std::flush;
    #endif
}

void Field::printExplanation()
{
    setCellSymbols();
    std::cout << "In this game your task is to find all hidden mines by uncovering all safe positions." << nl << nl;
    std::cout << "You can guess and sometimes combine where the next mine is." << nl;
    std::cout << "The number on each uncovered square shows how many neighbours contain a mine." << nl << nl;
    std::cout << "If you're sure that you have found a mine, place a flag on it's position by preceding an 'f' to your input." << nl;
    std::cout << "To remove the flag, just repeat your input. You may place or remove as many flags in a row as you wish." << nl << nl;
    std::cout << "Choose a position in this format: 'column,row' - e.g. 5,4" << nl;
    std::cout << "To place or remove a flag: f5,4" << nl << nl;
    std::cout << "You can reselect a numbered square to automatically uncover all remaining neighbours," << nl;
    std::cout << "as long as you put all flags right." << nl << nl;
    std::cout << "Press ENTER to go back...";
}

void Field::printAll()
{
    Common common;
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->minesCount << " mines" << nl << nl;
    std::cout << nl << nl;
    
    /*#if DEBUG == 1
        drawField(this->minesArray);
        std::cout << nl;
    #endif*/
    
    drawField(this->fieldArray);
    std::cout << nl;
}

// test coords if they contain a flag:
bool Field::isFlagSet(coordsStruct coords)
{
    setCellSymbols();
    if (this->fieldArray[coords.col][coords.row] == flagSymbol)
        return true;
    else 
        return false;
}

// test coords if they contain a number:
bool Field::isNumber(coordsStruct coords)
{
    Common common;
    for (int i = 1; i < 8; i++)
    {
        if (this->fieldArray[coords.col][coords.row] == common.intToString(i))
            return true;
    }
    return false;
}

// the main method of class Field which will alter the fieldArray:
placeUserInputReturnStruct Field::placeUserInput(userInputReturnStruct userInput, int turn)
{
    setCellSymbols();
    Common common;
    placeUserInputReturnStruct returnStruct;
    std::vector<coordsStruct> neighboursMinesVector;
    
    // set or remove flag if requested
    if (userInput.isFlag == true)
    {
        if (isFlagSet(userInput.coords) == true)
        {
            this->fieldArray[userInput.coords.col][userInput.coords.row] = coveredSymbol;
            printCoords(userInput.coords);
            this->minesLeft++;
            this->countEmpty++;
            return returnStruct;
        }
        else
        {
            this->fieldArray[userInput.coords.col][userInput.coords.row] = flagSymbol;
            printCoords(userInput.coords);
            this->minesLeft--;
            this->countEmpty--;
            return returnStruct;
        }
    }
    
    // fill the Field with random mines AFTER the player has placed his first guess:
    if (turn == 1)
    {
        fillMinesArray(userInput.coords);
    }
    
    // check if the player hit a mine which ends the game:
    if (this->minesArray[userInput.coords.col][userInput.coords.row] == bombSymbol)
    {
        this->minesArray[userInput.coords.col][userInput.coords.row] = bombHitSymbol;
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
        autoUncoverNeighboursMinesVector = findNeighbours(this->minesArray, userInput.coords, bombSymbol);
        
        // create a new vector of surrounding flags:
        std::vector<coordsStruct> autoUncoverNeighboursFlagsVector;
        autoUncoverNeighboursFlagsVector = findNeighbours(this->fieldArray, userInput.coords, flagSymbol);
        
        // create a new vector of surrounding covered squares:
        std::vector<coordsStruct> autoUncoverNeighboursCoveredVector;
        autoUncoverNeighboursCoveredVector = findNeighbours(this->fieldArray, userInput.coords, coveredSymbol);
        
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
                    if (this->minesArray[autoUncoverNeighboursCoveredVector.at(i).col][autoUncoverNeighboursCoveredVector.at(i).row] == bombSymbol)
                        autoUncoverMissedMinesVector.push_back(autoUncoverNeighboursCoveredVector.at(i));
                }
                // if there are missed mines, reveal the minesArray - player has lost:
                if (autoUncoverMissedMinesVector.size() != 0)
                {
                    for (int i = 0; i < static_cast<int>(autoUncoverMissedMinesVector.size()); i++)
                    {
                        this->minesArray[autoUncoverMissedMinesVector.at(i).col][autoUncoverMissedMinesVector.at(i).row] = bombHitSymbol;
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
                            autoUncoverNeighboursCoveredMinesVector = findNeighbours(this->minesArray, coordsTemp, bombSymbol);
                            if (autoUncoverNeighboursCoveredMinesVector.size() == 0)
                                this->fieldArray[coordsTemp.col][coordsTemp.row] = uncoveredSymbol;
                            else
                                this->fieldArray[coordsTemp.col][coordsTemp.row] = common.intToString(static_cast<int>(autoUncoverNeighboursCoveredMinesVector.size()));
                            printCoords(coordsTemp);
                            this->countEmpty--;
                        }
                    }
                
                }
            }
        }
    }
    else
    {
        // uncover the players choice and place the number of surrounding mines in it:
        neighboursMinesVector = findNeighbours(this->minesArray, userInput.coords, bombSymbol);
        if (neighboursMinesVector.size() == 0)
            this->fieldArray[userInput.coords.col][userInput.coords.row] = uncoveredSymbol;
        else
            this->fieldArray[userInput.coords.col][userInput.coords.row] = common.intToString(static_cast<int>(neighboursMinesVector.size()));
        printCoords(userInput.coords);
        this->countEmpty--;
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
                    if (this->fieldArray[i][j] == coveredSymbol)
                    {
                        // create a new vector of neighbours containing " ":
                        coordsStruct coordsBase;
                        coordsBase.col = i;
                        coordsBase.row = j;
                        std::vector<coordsStruct> neighboursZerosVector;
                        neighboursZerosVector = findNeighbours(this->fieldArray, coordsBase, uncoveredSymbol);
                        

                        // if there is a neighbour containing a " " create a new vector of neighbours containing mines:
                        if (neighboursZerosVector.size() != 0)
                        {
                            std::vector<coordsStruct> neighboursMinesVectorNew;
                            neighboursMinesVectorNew = findNeighbours(this->minesArray, coordsBase, bombSymbol);
    
                            // place neighboursMinesVectorNew.size() in fieldArray:
                            if (this->fieldArray[i][j] == coveredSymbol)
                            {
                                coordsStruct coordsTemp;
                                coordsTemp.col = i;
                                coordsTemp.row = j;
                                if (neighboursMinesVectorNew.size() == 0)
                                    this->fieldArray[i][j] = uncoveredSymbol;
                                else
                                    this->fieldArray[i][j] = common.intToString(static_cast<int>(neighboursMinesVectorNew.size()));
                                printCoords(coordsTemp);
                                this->countEmpty--;
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
                    if (this->fieldArray[a][b] == coveredSymbol)
                    {
                        // create a new vector of neighbours containing " ":
                        coordsStruct coordsBaseNew;
                        coordsBaseNew.col = a;
                        coordsBaseNew.row = b;
                        std::vector<coordsStruct> neighboursZerosVectorNew;
                        neighboursZerosVectorNew = findNeighbours(this->fieldArray, coordsBaseNew, uncoveredSymbol);
                        if (neighboursZerosVectorNew.size() != 0)
                            run = true;
                    }
                }
            }
        }
    }
    
    // check if player has won:
    if (this->countEmpty == this->minesCount)
    {
        printHasWon();
        returnStruct.hasWon = true;
    }        
    return returnStruct;
}

void Field::printHasWon()
{
    setCellSymbols();
    Common common;
    Input input;
    std::string wrongInputText = "Press ENTER to go back...";
    
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->minesCount << " mines" << nl << nl;
    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
            if (this->minesArray[i][j] == bombSymbol)
                this->fieldArray[i][j] = bombSymbol;
    }
    
    std::cout << nl << nl;
    
    drawField(this->fieldArray);
    std::cout << nl;
    std::cout << "Congratulation, you have won!" << nl;
    std::cout << wrongInputText;
    input.getEnterKey(wrongInputText);
    
}

void Field::printHasLost()
{
    setCellSymbols();
    Common common;
    Input input;
    
    setCellSymbols();
    
    std::string wrongInputText = "Press ENTER to go back...";
    
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->minesCount << " mines" << nl << nl;
    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            if (this->fieldArray[i][j] == flagSymbol) // && this->minesArray[i][j] == coveredSymbol)
                 this->minesArray[i][j] = this->fieldArray[i][j];
        }
    }
    std::cout << nl << nl;
    drawField(this->minesArray);
    
    std::cout << nl;
    std::cout << "Sry, you have lost!" << nl;
    std::cout << wrongInputText;
    input.getEnterKey(wrongInputText);
}

std::vector<coordsStruct> Field::findNeighbours(stringsym **tempArray, coordsStruct coords, stringsym mark)
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
