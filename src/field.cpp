#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "common.hpp"
#include "debug.hpp"
#include "field.hpp"
#include "input.hpp"

// the constructor:
Field::Field(int cols, int rows, int minesCount, std::string difficultyString)
{
    this->cols = cols;
    this->rows = rows;
    this->minesCount = minesCount;
    this->minesLeft = minesCount;
    this->countEmpty = cols * rows;
    this->difficultyString = difficultyString;
    this->fieldArray = createArray();
    this->minesArray = createArray();
    clearArray(this->fieldArray);
    clearArray(this->minesArray);
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

// user pointers to pointers to be able to create dynamic 2D-arrays
char** Field::createArray()
{
    std::vector<std::vector<char>> tempVector(this->rows, std::vector<char>(this->cols));
    const int colsNum = this->cols + 1;
    const int rowsNum = this->rows + 1;
    char** tempArray = new char*[colsNum];
    for (int i=0; i <= this->cols; i++)
        tempArray[i] = new char[rowsNum];

    return tempArray;
}

// fill an array with empty space (' ') - for this->fieldArray[][] and this->minesArray[][]:
void Field::clearArray(char** array)
{
    for (int i=0; i <= this->cols; i++)
    {
        for (int j=0; j <= this->rows; j++)
        {
            array[i][j] = ' ';
        }
    }
}

// place mines at random positions of this->minesArray[][]:
void Field::fillMinesArray()
{    
    Common common;
    Common::coordsStruct coords;
    int sizeOfFieldArray = this->cols * this->rows;
    std::vector<int> tempVector;
    for (int i = 0; i <= sizeOfFieldArray; i++)
        tempVector.push_back(i);
    std::random_shuffle(tempVector.begin()+1, tempVector.end());
    for (int i = 1; i <= this->minesCount; i++)
    {
        coords = common.intToStruct(tempVector.at(i), this->cols);
        this->minesArray[coords.col][coords.row] = 'X';
    }
}

// draw this->fieldArray[][] or this->minesArray[][] with current content:
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
    std::cout << "' '   = not uncovered yet" << nl;
    std::cout << "'0-8' = uncovered (0-8: number of neighbour mines)" << nl;
    std::cout << "'F'   = flag" << nl;
    std::cout << "'X'   = mine :-(" << nl;
    std::cout << nl << nl;
    std::cout << "Press ENTER to go back...";
}

void Field::printAll()
{
    Common common;
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->minesCount << " mines; " << this->minesLeft << " mines left..." << nl << nl;
    
    #if DEBUG == 1
        drawField(minesArray);
        std::cout << nl;
    #endif

    drawField(this->fieldArray);
    std::cout << nl;
}

// test coords if they contain a flag:
bool Field::isFlagOn(Common::coordsStruct coords)
{
    if (this->fieldArray[coords.col][coords.row] == 'F')
        return true;
    else 
        return false;
}

// test coords if they contain a number:
bool Field::isNumber(Common::coordsStruct coords)
{
    for (int i = 48; i < 56; i++)
    {
        if (this->fieldArray[coords.col][coords.row] == i)
            return true;
    }
    return false;
}

// the main function of class Field which will alter the fieldArray:
Common::placeUserInputStruct Field::placeUserInput(Common::userInputStruct userInput, int turn)
{
    Common::placeUserInputStruct returnStruct;
    if (userInput.isFlag == true)
    {
        if (isFlagOn(userInput.coords) == true)
        {
            this->fieldArray[userInput.coords.col][userInput.coords.row] = ' ';
            this->minesLeft++;
            returnStruct.isTurn = false;
            return returnStruct;
        }
        else
        {
            this->fieldArray[userInput.coords.col][userInput.coords.row] = 'F';
            this->minesLeft--;
            returnStruct.isTurn = false;
            return returnStruct;
        }
    }
    else
    {
        // fill the Field with random mine AFTER the player has placed his first guess:
        if (turn == 1)
        {
            fillMinesArray();
            while (this->minesArray[userInput.coords.col][userInput.coords.row] == 'X')
            {
                clearArray(minesArray);
                fillMinesArray();
            }
        }
        
        // check if the player hit a mine which ends the game:
        if (this->minesArray[userInput.coords.col][userInput.coords.row] == 'X')
        {
            this->minesArray[userInput.coords.col][userInput.coords.row] = '#';
            hasLost();
            returnStruct.hasLost = true;
            return returnStruct;
        }
        else
        {
            // if the player has typed the coords of an already uncovered position and has placed all flags right,
            // uncover all sourrounding safe positions:
            if (isNumber(userInput.coords))
            {
                // create a new vector of surrounding mines:
                std::vector<Common::coordsStruct> autoUncoverNeighboursMinesVector;
                autoUncoverNeighboursMinesVector = findNeighbours(this->minesArray, userInput.coords, 'X');
                
                // create a new vector of surrounding flags:
                std::vector<Common::coordsStruct> autoUncoverNeighboursFlagsVector;
                autoUncoverNeighboursFlagsVector = findNeighbours(this->fieldArray, userInput.coords, 'F');
                
                // create a new vector of surrounding covered squares:
                std::vector<Common::coordsStruct> autoUncoverNeighboursCoveredVector;
                autoUncoverNeighboursCoveredVector = findNeighbours(this->fieldArray, userInput.coords, ' ');
                
                // create a new empty vector for missed mines:
                std::vector<Common::coordsStruct> autoUncoverMissedMinesVector;
                
                // if player has placed some flags around userInput.coords:
                if (autoUncoverNeighboursFlagsVector.size() != 0)
                {
                    // check if the flags number matches the number of actual surrounding mines:
                    if (autoUncoverNeighboursMinesVector.size() == autoUncoverNeighboursFlagsVector.size())
                    {
                        // for each not uncovered neighbour of userInput.coords check if the player has missed a mine
                        // and add this mines position to autoUncoverMissedMinesVector:
                        for (int i = 0; i < static_cast<int>(autoUncoverNeighboursCoveredVector.size()); i++)
                        {
                            if (this->minesArray[autoUncoverNeighboursCoveredVector.at(i).col][autoUncoverNeighboursCoveredVector.at(i).row] == 'X')
                                autoUncoverMissedMinesVector.push_back(autoUncoverNeighboursCoveredVector.at(i));
                        }
                    }
                    // if there are missed mines, reveal the minesArray - player has lost:
                    if (autoUncoverMissedMinesVector.size() != 0)
                    {
                        for (int i = 0; i < static_cast<int>(autoUncoverMissedMinesVector.size()); i++)
                        {
                            this->minesArray[autoUncoverMissedMinesVector.at(i).col][autoUncoverMissedMinesVector.at(i).row] = '#';
                        }
                        returnStruct.hasLost = 1;
                        hasLost();
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
                                Common::coordsStruct coordsTemp;
                                coordsTemp.col = autoUncoverNeighboursCoveredVector.at(i).col;
                                coordsTemp.row = autoUncoverNeighboursCoveredVector.at(i).row;
                                std::vector<Common::coordsStruct> autoUncoverNeighboursCoveredMinesVector;
                                autoUncoverNeighboursCoveredMinesVector = findNeighbours(this->minesArray, coordsTemp, 'X');
                                this->fieldArray[coordsTemp.col][coordsTemp.row] = static_cast<char>(autoUncoverNeighboursCoveredMinesVector.size() + 48);
                            }
                            returnStruct.isTurn = false;
                            return returnStruct;
                        }
                    }
                }
                returnStruct.isTurn = false;
                return returnStruct;
            }
        
            // uncover the players choice and place the number of surrounding mines in it:
            std::vector<Common::coordsStruct> neighboursMinesVector;
            neighboursMinesVector = findNeighbours(this->minesArray, userInput.coords, 'X');
            
            // place neighboursMinesVector.size() in fieldArray (convert int to char by adding 48):
            this->fieldArray[userInput.coords.col][userInput.coords.row] = static_cast<char>(neighboursMinesVector.size() + 48);
            this->countEmpty--;
            
            // automatically uncover all neighbour squares of squares containing a '0' (repeat if new '0's appeared):
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
                            if (this->fieldArray[i][j] == ' ')
                            {
                                // create a new vector of neighbours containing '0':
                                Common::coordsStruct coordsBase;
                                coordsBase.col = i;
                                coordsBase.row = j;
                                std::vector<Common::coordsStruct> neighboursZerosVector;
                                neighboursZerosVector = findNeighbours(this->fieldArray, coordsBase, 48);
                                

                                // if there is a neighbour containing a '0' create a new vector of neighbours containing mines:
                                if (neighboursZerosVector.size() != 0)
                                {
                                    std::vector<Common::coordsStruct> neighboursMinesVectorNew;
                                    neighboursMinesVectorNew = findNeighbours(this->minesArray, coordsBase, 'X');
            
                                    // place neighboursMinesVectorNew.size() in fieldArray (convert int to char by adding 48):
                                    if (this->fieldArray[i][j] == ' ')
                                    {
                                        this->fieldArray[i][j] = static_cast<char>(neighboursMinesVectorNew.size() + 48);
                                        this->countEmpty--;
                                    }
                                }
                            }
                        }
                    }
                    run = false;
                    for (int a = 1; a <= this->cols; a++)
                    {
                        for (int b = 1; b <= this->rows; b++)
                        {
                            if (this->fieldArray[a][b] == ' ')
                            {
                                // create a new vector of neighbours containing '0':
                                Common::coordsStruct coordsBaseNew;
                                coordsBaseNew.col = a;
                                coordsBaseNew.row = b;
                                std::vector<Common::coordsStruct> neighboursZerosVectorNew;
                                neighboursZerosVectorNew = findNeighbours(this->fieldArray, coordsBaseNew, 48);
                                if (neighboursZerosVectorNew.size() != 0)
                                    run = true;
                            }
                        }
                    }
                }
            }
        }
        
        // check if player has won:
        if (this->countEmpty == minesCount)
        {
            hasWon();
            returnStruct.hasWon = true;
            return returnStruct;
        }        
        return returnStruct;
    }
}

void Field::hasWon()
{
    Common common;
    Input input;
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->minesCount << " mines" << nl << nl;
    for (int i = 1; i <= this->cols; i++)
        for (int j = 1; j <= this->rows; j++)
            if (minesArray[i][j] == 'X')
                fieldArray[i][j] = 'X';
    
    drawField(this->fieldArray);
    std::cout << nl;
    std::cout << "Congratulation, you have won!" << nl;
    std::cout << "Press ENTER to go back...";
    input.getAnyKey();
    
}

void Field::hasLost()
{
    Common common;
    Input input;
    common.clearScreen();
    std::cout << "Minestepper" << " - " << this->difficultyString << " (" << this->cols << "x" << this->rows << ") - " << this->minesCount << " mines" << nl << nl;
    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            if (this->fieldArray[i][j] == 'F' && this->minesArray[i][j] == ' ')
                 this->minesArray[i][j] = this->fieldArray[i][j];
        }
    }
    drawField(this->minesArray);
    
    std::cout << nl;
    std::cout << "Sry, you have lost!" << nl;
    std::cout << "Press ENTER to go back...";
    input.getAnyKey();
}

std::vector<Common::coordsStruct> Field::findNeighbours(char **tempArray, Common::coordsStruct coords, char mark)
{
    std::vector<Common::coordsStruct> neighboursVector;
    
    // up left:
    if (coords.col-1 > 0 && coords.row-1 > 0)
    {
        if (tempArray[coords.col-1][coords.row-1] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col-1;
            tempCoords.row = coords.row-1;
            neighboursVector.push_back(tempCoords);
        }
    }

    // up middle:
    if (coords.row-1 > 0)
    {
        if (tempArray[coords.col][coords.row-1] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col;
            tempCoords.row = coords.row-1;
            neighboursVector.push_back(tempCoords);
        }
    }

    // up right:
    if (coords.col+1 <= this->cols && coords.row-1 > 0)
    {
        if (tempArray[coords.col+1][coords.row-1] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col+1;
            tempCoords.row = coords.row-1;
            neighboursVector.push_back(tempCoords);
        }
    }

    // middle left:
    if (coords.col-1 > 0)
    {
        if (tempArray[coords.col-1][coords.row] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col-1;
            tempCoords.row = coords.row;
            neighboursVector.push_back(tempCoords);
        }
    }
    
    // middle right:
    if (coords.col+1 <= this->cols)
    {
        if (tempArray[coords.col+1][coords.row] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col+1;
            tempCoords.row = coords.row;
            neighboursVector.push_back(tempCoords);
        }
    }

    // down left:
    if (coords.col-1 > 0 && coords.row+1 <= this->rows)
    {
        if (tempArray[coords.col-1][coords.row+1] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col-1;
            tempCoords.row = coords.row+1;
            neighboursVector.push_back(tempCoords);
        }
    }

    // down middle:
    if (coords.row+1 <= this->rows)
    {
        if (tempArray[coords.col][coords.row+1] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col;
            tempCoords.row = coords.row+1;
            neighboursVector.push_back(tempCoords);
        }
    }

    // down right:
    if (coords.col+1 <= this->cols && coords.row+1 <= this->rows)
    {
        if (tempArray[coords.col+1][coords.row+1] == mark)
        {
            Common::coordsStruct tempCoords;
            tempCoords.col = coords.col+1;
            tempCoords.row = coords.row+1;
            neighboursVector.push_back(tempCoords);
        }
    }
    
    return neighboursVector;
}
