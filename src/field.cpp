#include <algorithm>
#include <iostream>
#include <random>

#include "common.hpp"
#include "debug.hpp"
#include "field.hpp"
#include "input.hpp"

// the constructor:
Field::Field(int cols, int rows, int offsetX, int offsetY, int minesCount, std::string difficultyString)
{
    this->cols = cols;
    this->rows = rows;
    this->offsetX = offsetX;
    this->offsetY = offsetY;
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
        delete this->fieldArray[i];
    delete[] this->fieldArray;
    
    for (int i=0; i <= cols; i++)
        delete this->minesArray[i];
    delete[] this->minesArray;
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
    return this->offsetX;
}

int Field::getOffsetY()
{
    return this->offsetY;
}

int Field::getMinesLeft()
{
    return this->minesLeft;
}

// use pointers to pointers to be able to create dynamic 2D-arrays
char** Field::createArray()
{
    const int colsNum = this->cols + 1;
    const int rowsNum = this->rows + 1;
    char** tempArray = new char*[colsNum];
    for (int i=0; i <= this->cols; i++)
        tempArray[i] = new char[rowsNum];
    return tempArray;
}

// fill this->fieldArray[][] with '-'
void Field::clearFieldArray()
{
    for (int i=0; i <= this->cols; i++)
    {
        for (int j=0; j <= this->rows; j++)
        {
            this->fieldArray[i][j] = '-';
        }
    }
}

// fill this->minesArray[][] with ' '
void Field::clearMinesArray()
{
    for (int i=0; i <= this->cols; i++)
    {
        for (int j=0; j <= this->rows; j++)
        {
            this->minesArray[i][j] = ' ';
        }
    }
}

// place mines at random positions of this->minesArray[][]:
void Field::fillMinesArray(coordsStruct userFirstInput)
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
        this->minesArray[coords.col][coords.row] = 'X';
    }
}

// draw this->fieldArray[][] or this->minesArray[][]:
void Field::drawField(char** array)
{
    std::cout << "    ";
    for (int colNum = 1; colNum <= this->cols; colNum++)
    {
        if (colNum < 10)
            std::cout << "  " << colNum << " ";
        else
            std::cout << " " << colNum << " ";
    }
    std::cout << nl;
    std::cout << "   ";
    for (int bar = 1; bar <= this->cols; bar++)
    {
        std::cout << " " << "---";
    }
    std::cout << nl;

    for (int rowNum = 1; rowNum <= this->rows; rowNum++)
    {
        for (int colNum = 1; colNum <= this->cols; colNum++)
        {
            if (colNum == 1)
            {
                if (rowNum < 10)
                    std::cout << " " << rowNum << " ";
                else
                    std::cout << rowNum << " ";
            }
            std::cout << "|" << " " << array[colNum][rowNum] << " ";
            if (colNum == this->cols)
            {
                std::cout << "|";
                if (rowNum < 10)
                    std::cout << " " << rowNum << " ";
                else
                    std::cout << rowNum << " ";        
                std::cout << nl;
                std::cout << "   ";
            }
        }
        for (int bar = 1; bar <= this->cols; bar++)
        {
            std::cout << " " << "---";
        }       
        std::cout << nl;
    }
    std::cout << "    ";
    for (int colNum = 1; colNum <= this->cols; colNum++)
    {
        if (colNum < 10)
            std::cout << "  " << colNum << " ";
        else
            std::cout << " " << colNum << " ";
    }
    std::cout << nl;
}

void Field::gotoXY(int x, int y)
{
    printf("%c[%d;%df",0x1B,y, x);
}

void Field::printCoords(coordsStruct coords)
{
    gotoXY(this->offsetX + coords.col * 4, this->offsetY + coords.row * 2);
    std::cout << this->fieldArray[coords.col][coords.row];
    std::cout << std::flush;
}

void Field::printMinesLeft()
{
    std::cout << "There are " << this->minesLeft << " mines left." << nl;
}

void Field::printExplanation()
{   std::cout << "In this game your task is to find all hidden mines by uncovering all safe positions." << nl << nl;
    std::cout << "You can guess and sometimes combine where the next mine is." << nl;
    std::cout << "The number on each uncovered square shows how many neighbours contain a mine." << nl << nl;
    std::cout << "If you're sure that you have found a mine, place a flag on it's position by preceding an 'f' to your input." << nl;
    std::cout << "To remove the flag, just repeat your input. You may place or remove as many flags in a row as you wish." << nl << nl;
    std::cout << "Choose a position in this format: 'column,row' - e.g. 5,4" << nl;
    std::cout << "To place or remove a flag: f5,4" << nl << nl;
    std::cout << "You can reselect a numbered square to automatically uncover all remaining neighbours," << nl;
    std::cout << "as long as you put all flags right." << nl << nl;
    
    std::cout << "Explanations:" << nl;
    std::cout << "'-'   = not uncovered yet" << nl;
    std::cout << "' '   = uncovered and zero neighbour mines" << nl;
    std::cout << "'1-8' = uncovered (1-8: number of neighbour mines)" << nl;
    std::cout << "'F'   = flag" << nl;
    std::cout << "'X'   = mine :-(" << nl;
    std::cout << nl << nl;
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
    #endif
    */
    drawField(this->fieldArray);
    std::cout << nl;
}

// test coords if they contain a flag:
bool Field::isFlagSet(coordsStruct coords)
{
    if (this->fieldArray[coords.col][coords.row] == 'F')
        return true;
    else 
        return false;
}

// test coords if they contain a number:
bool Field::isNumber(coordsStruct coords)
{
    for (int i = 49; i < 56; i++)
    {
        if (this->fieldArray[coords.col][coords.row] == i)
            return true;
    }
    return false;
}

// the main method of class Field which will alter the fieldArray:
placeUserInputReturnStruct Field::placeUserInput(userInputReturnStruct userInput, int turn)
{
    placeUserInputReturnStruct returnStruct;
    std::vector<coordsStruct> neighboursMinesVector;
    
    // set or remove flag if requested
    if (userInput.isFlag == true)
    {
        if (isFlagSet(userInput.coords) == true)
        {
            this->fieldArray[userInput.coords.col][userInput.coords.row] = '-';
            printCoords(userInput.coords);
            this->minesLeft++;
            this->countEmpty++;
            return returnStruct;
        }
        else
        {
            this->fieldArray[userInput.coords.col][userInput.coords.row] = 'F';
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
    if (this->minesArray[userInput.coords.col][userInput.coords.row] == 'X')
    {
        this->minesArray[userInput.coords.col][userInput.coords.row] = '#';
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
        autoUncoverNeighboursMinesVector = findNeighbours(this->minesArray, userInput.coords, 'X');
        
        // create a new vector of surrounding flags:
        std::vector<coordsStruct> autoUncoverNeighboursFlagsVector;
        autoUncoverNeighboursFlagsVector = findNeighbours(this->fieldArray, userInput.coords, 'F');
        
        // create a new vector of surrounding covered squares:
        std::vector<coordsStruct> autoUncoverNeighboursCoveredVector;
        autoUncoverNeighboursCoveredVector = findNeighbours(this->fieldArray, userInput.coords, '-');
        
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
                    if (this->minesArray[autoUncoverNeighboursCoveredVector.at(i).col][autoUncoverNeighboursCoveredVector.at(i).row] == 'X')
                        autoUncoverMissedMinesVector.push_back(autoUncoverNeighboursCoveredVector.at(i));
                }
                // if there are missed mines, reveal the minesArray - player has lost:
                if (autoUncoverMissedMinesVector.size() != 0)
                {
                    for (int i = 0; i < static_cast<int>(autoUncoverMissedMinesVector.size()); i++)
                    {
                        this->minesArray[autoUncoverMissedMinesVector.at(i).col][autoUncoverMissedMinesVector.at(i).row] = '#';
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
                            autoUncoverNeighboursCoveredMinesVector = findNeighbours(this->minesArray, coordsTemp, 'X');
                            if (autoUncoverNeighboursCoveredMinesVector.size() == 0)
                                this->fieldArray[coordsTemp.col][coordsTemp.row] = ' ';
                            else
                                this->fieldArray[coordsTemp.col][coordsTemp.row] = static_cast<char>(autoUncoverNeighboursCoveredMinesVector.size() + 48);                 
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
        neighboursMinesVector = findNeighbours(this->minesArray, userInput.coords, 'X');
        if (neighboursMinesVector.size() == 0)
            this->fieldArray[userInput.coords.col][userInput.coords.row] = ' ';
        else
            this->fieldArray[userInput.coords.col][userInput.coords.row] = static_cast<char>(neighboursMinesVector.size() + 48);
        printCoords(userInput.coords);
        this->countEmpty--;
        returnStruct.isTurn = true;
    }
    
    // automatically uncover all neighbour squares of squares containing a ' ' (repeat if new ' 's appeared):
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
                    if (this->fieldArray[i][j] == '-')
                    {
                        // create a new vector of neighbours containing ' ':
                        coordsStruct coordsBase;
                        coordsBase.col = i;
                        coordsBase.row = j;
                        std::vector<coordsStruct> neighboursZerosVector;
                        neighboursZerosVector = findNeighbours(this->fieldArray, coordsBase, ' ');
                        

                        // if there is a neighbour containing a ' ' create a new vector of neighbours containing mines:
                        if (neighboursZerosVector.size() != 0)
                        {
                            std::vector<coordsStruct> neighboursMinesVectorNew;
                            neighboursMinesVectorNew = findNeighbours(this->minesArray, coordsBase, 'X');
    
                            // place neighboursMinesVectorNew.size() in fieldArray (convert int to char by adding 48):
                            if (this->fieldArray[i][j] == '-')
                            {
                                coordsStruct coordsTemp;
                                coordsTemp.col = i;
                                coordsTemp.row = j;
                                if (neighboursMinesVectorNew.size() == 0)
                                    this->fieldArray[i][j] = ' ';
                                else
                                    this->fieldArray[i][j] = static_cast<char>(neighboursMinesVectorNew.size() + 48);
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
                    if (this->fieldArray[a][b] == '-')
                    {
                        // create a new vector of neighbours containing ' ':
                        coordsStruct coordsBaseNew;
                        coordsBaseNew.col = a;
                        coordsBaseNew.row = b;
                        std::vector<coordsStruct> neighboursZerosVectorNew;
                        neighboursZerosVectorNew = findNeighbours(this->fieldArray, coordsBaseNew, ' ');
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
    Common common;
    Input input;
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->minesCount << " mines" << nl << nl;
    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
            if (this->minesArray[i][j] == 'X')
                this->fieldArray[i][j] = 'X';
    }
    
    std::cout << nl << nl;
    
    drawField(this->fieldArray);
    std::cout << nl;
    std::cout << "Congratulation, you have won!" << nl;
    std::cout << "Press ENTER to go back...";
    input.getAnyKey();
    
}

void Field::printHasLost()
{
    Common common;
    Input input;
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->minesCount << " mines" << nl << nl;
    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            if (this->fieldArray[i][j] == 'F') // && this->minesArray[i][j] == '-')
                 this->minesArray[i][j] = this->fieldArray[i][j];
        }
    }
    std::cout << nl << nl;
    drawField(this->minesArray);
    
    std::cout << nl;
    std::cout << "Sry, you have lost!" << nl;
    std::cout << "Press ENTER to go back...";
    input.getAnyKey();
}

std::vector<coordsStruct> Field::findNeighbours(char **tempArray, coordsStruct coords, char mark)
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
