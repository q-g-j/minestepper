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
#include <algorithm>
#include <array>
#include <atomic>
#include <iostream>
#include <vector>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// project headers:
#include <colors.h>
#include <common.h>
#include <debug.h>
#include <field.h>
#include <game.h>
#include <input.h>
#include <print.h>
#include <symbols.h>

// constructor
Field::Field(Common::GameModeReturnStruct& gameMode)
:
    cols(gameMode.cols),
    rows(gameMode.rows),
    fieldOffsetX(gameMode.offsetX),
    fieldOffsetY(gameMode.offsetY),
    fieldCellWidth(gameMode.cellWidth),
    minesTotal(gameMode.mines),
    minesLeft(gameMode.mines),
    coveredLeft(cols * rows),
    flagsCount(0),
    difficultyString(gameMode.difficultyString),
    urng(rng()),
    field2DVector(create2DVector("field")),
    mines2DVector(create2DVector("mines"))
{ }

Field::~Field() = default;

// some getters:
const int &Field::getCols() const { return this->cols; }
const int &Field::getRows() const { return this->rows; }
const int &Field::getOffsetX() const { return this->fieldOffsetX; }
const int &Field::getOffsetY() const { return this->fieldOffsetY; }
const int &Field::getCellWidth() const { return this->fieldCellWidth; }
const int &Field::getCoveredLeft() const { return this->coveredLeft; }
const int &Field::getMinesTotal() const { return this->minesTotal; }
const int &Field::getMinesLeft() const { return this->minesLeft; }
const int &Field::getFlagsCount() const { return this->flagsCount; }
const std::string &Field::getDifficultyString() const { return this->difficultyString; }
const stringconv &Field::getCoordsContent(Common::CoordsStruct const& coords) const { return this->field2DVector[coords.col][coords.row]; }

// setters:
void Field::setCoveredLeft::operator--() { --field_.coveredLeft; }
void Field::setCoveredLeft::operator++() { ++field_.coveredLeft; }
void Field::setFlagsCount::operator--() { --field_.flagsCount; }
void Field::setFlagsCount::operator++() { ++field_.flagsCount; }
void Field::setMinesLeft::operator--() { --field_.minesLeft; }
void Field::setMinesLeft::operator++() { ++field_.minesLeft; }

// use 2D vectors for the visible game board and the internal mines field:
std::vector<std::vector<stringconv>> Field::create2DVector(std::string const& vectorType)
{
    std::vector<std::vector<stringconv>> temp2DVector;
    for(int i = 0; i <= this->cols; ++i)
    {
        std::vector<stringconv> row;
        for(int j = 0; j <= this->rows; ++j)
        {
            if (vectorType == "field")
            {
                row.push_back(Symbols::getSymbol("symbolCovered"));
            }
            else if (vectorType == "mines")
            {
                row.push_back(Symbols::getSymbol("symbolZero"));
            }
        }
        temp2DVector.push_back(row);
    }
    return temp2DVector;
}

// place mines at random positions of this->mines2DVector]:
void Field::fillMines(Common::CoordsStruct const& userFirstInput)
{
    #if STATIC_FIELD == 1
        this->mines2DVector[3][3] = Symbols::getSymbol("symbolMine");
        this->mines2DVector[4][3] = Symbols::getSymbol("symbolMine");
        this->minesTotal = 2;
    #else
        Common::CoordsStruct coords;
        size_t sizeofField2DVector = this->cols * this->rows;
        std::vector<int> tempVector;
        for (unsigned int i = 1; i <= sizeofField2DVector; ++i)
        {
            if (i != Common::coordsToInt(userFirstInput, this->cols))
            {
                tempVector.push_back(i);
            }
        }

        std::shuffle(tempVector.begin(), tempVector.end(), urng);
        
        for (int i = 0; i < this->minesTotal; ++i)
        {
            coords = Common::intToCoords(tempVector.at(i), this->cols);
            this->mines2DVector[coords.col][coords.row] = Symbols::getSymbol("symbolMine");
        }
    #endif
}

// draw the game board:
void Field::drawField()
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        Common::setUnicode(true);
    #endif

    for (int i = 0; i < this->fieldOffsetY - 4; ++i)
    {
        std::wcout << L"\n";
    }
    for (int i = 0; i < this->fieldOffsetX - 2; ++i)
    {
        std::wcout << L" ";
    }
    coutconv << Symbols::getSymbol("symbolCornerTopLeft");
    for (int col = 1; col <= this->cols; ++col)
    {
        if (col < this->cols)
        {
            for (int width = 0; width < this->fieldCellWidth; ++width)
            {
                coutconv << Symbols::getSymbol("symbolHorizontalLine");
            }
            coutconv << Symbols::getSymbol("symbolDownT");
        }
        else
        {
            for (int width = 0; width < this->fieldCellWidth; ++width)
            {
                coutconv << Symbols::getSymbol("symbolHorizontalLine");
            }
            coutconv << Symbols::getSymbol("symbolCornerTopRight");
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
            coutconv << Symbols::getSymbol("symbolVerticalLine");

            for (int padding = 0; padding < (this->fieldCellWidth-1)/2; ++padding)
            {
                std::wcout << L" ";
            }

            Common::CoordsStruct coords;
            coords.col = col;
            coords.row = row;

            printCoords(coords, false);

            #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
                Common::setUnicode(true);
            #endif

            for (int padding = 0; padding < (this->fieldCellWidth-1)/2; ++padding)
            {
                std::wcout << L" ";
            }

            if (col == this->cols)
            {
                coutconv << Symbols::getSymbol("symbolVerticalLine");
                std::wcout << L"\n";
                for (int i = 0; i < this->fieldOffsetX - 2; ++i)
                {
                    std::wcout << L" ";
                }
            }
        }

        if (row < this->rows)
        {
            coutconv << Symbols::getSymbol("symbolRightT");
        }
        else
        {
            coutconv << Symbols::getSymbol("symbolCornerBottomLeft");
        }

        for (int col = 1; col <= this->cols; ++col)
        {
            if (col < this->cols)
            {
                for (int width = 0; width < this->fieldCellWidth; ++width)
                {
                    coutconv << Symbols::getSymbol("symbolHorizontalLine");
                }
                if (row < this->rows)
                {
                    coutconv << Symbols::getSymbol("symbolPlus");
                }
                else
                {
                    coutconv << Symbols::getSymbol("symbolUpT");
                }
            }
            else
            {
                for (int width = 0; width < this->fieldCellWidth; ++width)
                {
                    coutconv << Symbols::getSymbol("symbolHorizontalLine");
                }
                if (row < this->rows)
                {
                    coutconv << Symbols::getSymbol("symbolLeftT");
                }
                else
                {
                    coutconv << Symbols::getSymbol("symbolCornerBottomRight");
                }
            }
        }
        std::wcout << L"\n";
    }
    coutconv << std::flush;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        Common::setUnicode(false);
    #endif
}

void Field::printAll()
{
    Common::resizeConsole(this->fieldOffsetX + (this->cols * (((this->fieldCellWidth - 1) / 2) * 2 + 2)) + this->fieldOffsetX - 3, this->fieldOffsetY + (this->rows * 2) + 5);
    Common::clearScreen();

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        Common::centerWindow();
    #endif

    Common::gotoXY(this->fieldOffsetX - 1, 1);
    Print::printTitle(this->difficultyString, this->cols, this->rows, this->minesTotal);
    Common::gotoXY(1, 3);
    this->drawField();

    Common::gotoXY(this->fieldOffsetX - 1, this->fieldOffsetY + this->rows * 2);
    Colors::setTextColor("fg_white");
    std::cout << Print::getHelpText << newline << newline;
    Colors::setTextColor("color_default");
    
    Print::printMinesLeft(*this);

    #if DEBUG == 1
        Print::printDebugCoveredLeft(*this);
    #endif
}

// Print the content of one particular cell at "coords".
// This method is used both to reveal a cell and to display the cursor.
void Field::printCoords(Common::CoordsStruct const& coords, bool isCursor)
{
    Common::CoordsStruct tempCoords;
    extern std::atomic<bool> isTimerPrinting;

    while (isTimerPrinting)
    {
        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
                Sleep(1);
        #else
                usleep(1 * 1000);
        #endif
    }

    tempCoords = Common::coordsToCursorPosition(coords, this->fieldOffsetX, this->fieldOffsetY, this->fieldCellWidth);
    Common::gotoXY(tempCoords.col, tempCoords.row);

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        Common::setUnicode(true);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (isNumber(coords))
        {
            if (isCursor == false)
            {
                if      (getCoordsContent(coords) == L"1") Colors::setTextColor("fg_number_1");
                else if (getCoordsContent(coords) == L"2") Colors::setTextColor("fg_number_2");
                else if (getCoordsContent(coords) == L"3") Colors::setTextColor("fg_number_3");
                else if (getCoordsContent(coords) == L"4") Colors::setTextColor("fg_number_4");
                else if (getCoordsContent(coords) == L"5") Colors::setTextColor("fg_number_5");
                else if (getCoordsContent(coords) == L"6") Colors::setTextColor("fg_number_6");
                else if (getCoordsContent(coords) == L"7") Colors::setTextColor("fg_number_7");
                else if (getCoordsContent(coords) == L"8") Colors::setTextColor("fg_number_8");
                else Colors::setTextColor("color_default");
            }
            else
            {
                if      (getCoordsContent(coords) == L"1") Colors::setTextColor("bg_number_1");
                else if (getCoordsContent(coords) == L"2") Colors::setTextColor("bg_number_2");
                else if (getCoordsContent(coords) == L"3") Colors::setTextColor("bg_number_3");
                else if (getCoordsContent(coords) == L"4") Colors::setTextColor("bg_number_4");
                else if (getCoordsContent(coords) == L"5") Colors::setTextColor("bg_number_5");
                else if (getCoordsContent(coords) == L"6") Colors::setTextColor("bg_number_6");
                else if (getCoordsContent(coords) == L"7") Colors::setTextColor("bg_number_7");
                else if (getCoordsContent(coords) == L"8") Colors::setTextColor("bg_number_8");
                else Colors::setTextColor("color_default");
            }
        }
        else if (getCoordsContent(coords) == Symbols::getSymbol("symbolFlag"))
        {
            if (isCursor) Colors::setTextColor("bg_flag");
            else Colors::setTextColor("fg_flag");
        }
        else if (getCoordsContent(coords) == Symbols::getSymbol("symbolMine")) Colors::setTextColor("mine");
        else if (getCoordsContent(coords) == Symbols::getSymbol("symbolMineHit")) Colors::setTextColor("minehit");
        else if (getCoordsContent(coords) == Symbols::getSymbol("symbolCovered"))
        {
            if (isCursor) Colors::setTextColor("bg_covered");
            else Colors::setTextColor("fg_covered");
        }
        else if (getCoordsContent(coords) == Symbols::getSymbol("symbolZero"))
        {
            if (isCursor) Colors::setTextColor("bg_covered");
        }
        std::wstring coordsString = this->field2DVector[coords.col][coords.row];
        WriteConsoleW(hConsole, coordsString.c_str(), static_cast<DWORD>(coordsString.size()), nullptr, nullptr);
        Colors::setTextColor("color_default");
        Common::setUnicode(false);
    #else
        std::string content;
        if (isNumber(coords))
        {
            if (isCursor == false)
            {
                if      (getCoordsContent(coords) == "1")
                {
                    Colors::setTextColor("fg_number_1");
                    content = "1";
                }
                else if (getCoordsContent(coords) == "2")
                {
                    Colors::setTextColor("fg_number_2");
                    content = "2";
                }
                else if (getCoordsContent(coords) == "3")
                {
                    Colors::setTextColor("fg_number_3");
                    content = "3";
                }
                else if (getCoordsContent(coords) == "4")
                {
                    Colors::setTextColor("fg_number_4");
                    content = "4";
                }
                else if (getCoordsContent(coords) == "5")
                {
                    Colors::setTextColor("fg_number_5");
                    content = "5";
                }
                else if (getCoordsContent(coords) == "6")
                {
                    Colors::setTextColor("fg_number_6");
                    content = "6";
                }
                else if (getCoordsContent(coords) == "7")
                {
                    Colors::setTextColor("fg_number_7");
                    content = "7";
                }
                else if (getCoordsContent(coords) == "8")
                {
                    Colors::setTextColor("fg_number_8");
                    content = "8";
                }
                else
                {
                    Colors::setTextColor("color_default");
                    content = (this->field2DVector[coords.col][coords.row]);
                }
            }
            else
            {
                if      (getCoordsContent(coords) == "1")
                {
                    Colors::setTextColor("bg_number_1");
                    content = "1";
                }
                else if (getCoordsContent(coords) == "2")
                {
                    Colors::setTextColor("bg_number_2");
                    content = "2";
                }
                else if (getCoordsContent(coords) == "3")
                {
                    Colors::setTextColor("bg_number_3");
                    content = "3";
                }
                else if (getCoordsContent(coords) == "4")
                {
                    Colors::setTextColor("bg_number_4");
                    content = "4";
                }
                else if (getCoordsContent(coords) == "5")
                {
                    Colors::setTextColor("bg_number_5");
                    content = "5";
                }
                else if (getCoordsContent(coords) == "6")
                {
                    Colors::setTextColor("bg_number_6");
                    content = "6";
                }
                else if (getCoordsContent(coords) == "7")
                {
                    Colors::setTextColor("bg_number_7");
                    content = "7";
                }
                else if (getCoordsContent(coords) == "8")
                {
                    Colors::setTextColor("bg_number_8");
                    content = "8";
                }
                else
                {
                    Colors::setTextColor("bg_default");
                    content = (this->field2DVector[coords.col][coords.row]);
                }
            }
        }
        else if (getCoordsContent(coords) == Symbols::getSymbol("symbolFlag"))
        {
            if (isCursor)
            {
                Colors::setTextColor("bg_flag");
                content = Symbols::getSymbol("symbolFlag");
            }
            else
            {
                Colors::setTextColor("fg_flag");
                content = Symbols::getSymbol("symbolFlag");
            }
        }
        else if (getCoordsContent(coords) == Symbols::getSymbol("symbolMine"))
        {
            Colors::setTextColor("mine");
            content = Symbols::getSymbol("symbolMine");
        }
        else if (getCoordsContent(coords) == Symbols::getSymbol("symbolMineHit"))
        {
            Colors::setTextColor("minehit");
            content = Symbols::getSymbol("symbolMineHit");
        }
        else if (getCoordsContent(coords) == Symbols::getSymbol("symbolCovered"))
        {
            if (isCursor)
            {
                Colors::setTextColor("bg_covered");
                content = Symbols::getSymbol("symbolCovered");
            }
            else
            {
                Colors::setTextColor("fg_covered");
                content = Symbols::getSymbol("symbolCovered");
            }
        }
        else if (getCoordsContent(coords) == Symbols::getSymbol("symbolZero"))
        {
            if (isCursor)
            {
                Colors::setTextColor("bg_covered");
                content = Symbols::getSymbol("symbolCovered");
            }
            else
            {
                Colors::setTextColor("bg_covered");
                content = Symbols::getSymbol("symbolZero");
            }
        }
        else
        {
            Colors::setTextColor("color_default");
            content = (this->field2DVector[coords.col][coords.row]);
        }

        coutconv << content << std::flush;
        Colors::setTextColor("color_default");
    #endif
}

// test coords if they contain a flag:
bool Field::isFlag(Common::CoordsStruct const& coords)
{
    if (this->field2DVector[coords.col][coords.row] == Symbols::getSymbol("symbolFlag"))
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
    for (int i = 1; i <= 8; ++i)
    {
        if (this->field2DVector[coords.col][coords.row] == Common::intToStringConv(i))
        {
            return true;
        }
    }
    return false;
}

// find neighbors of a cell at "coords" that hold a given content (passed by variable symbol)
std::vector<Common::CoordsStruct> Field::findNeighbors(std::vector<std::vector<stringconv>> const& temp2DVector, Common::CoordsStruct const& coords, stringconv const& symbol)
{
    std::vector<Common::CoordsStruct> findNeighborsReturn;
    std::array<std::array<int, 2>, 8> neighborsArray;
    neighborsArray.at(0) = { -1, -1 };
    neighborsArray.at(1) = {  0, -1 };
    neighborsArray.at(2) = {  1, -1 };
    neighborsArray.at(3) = {  1,  0 };
    neighborsArray.at(4) = {  1,  1 };
    neighborsArray.at(5) = {  0,  1 };
    neighborsArray.at(6) = { -1,  1 };
    neighborsArray.at(7) = { -1,  0 };

    for (size_t x = 0; x < sizeof(neighborsArray) / sizeof(neighborsArray[0]); ++x)
    {
        if (
            !(coords.col == 1 && neighborsArray[x][0] == -1) &&
            !(coords.row == 1 && neighborsArray[x][1] == -1) &&
            !(coords.col == this->cols && neighborsArray[x][0] == 1) &&
            !(coords.row == this->rows && neighborsArray[x][1] == 1)
            )
        {
            if (symbol == Symbols::getSymbol("symbolNumber"))
            {
                for (int i = 1; i < 8; ++i)
                {
                    if (temp2DVector[coords.col + neighborsArray[x][0]][coords.row + neighborsArray[x][1]] == Symbols::getSymbol("symbolNumbersArray[i]"))
                    {
                        Common::CoordsStruct tempCoords;
                        tempCoords.col = coords.col + neighborsArray[x][0];
                        tempCoords.row = coords.row + neighborsArray[x][1];
                        findNeighborsReturn.push_back(tempCoords);
                    }
                }
            }
            else if (temp2DVector[coords.col + neighborsArray[x][0]][coords.row + neighborsArray[x][1]] == symbol)
            {
                Common::CoordsStruct tempCoords;
                tempCoords.col = coords.col + neighborsArray[x][0];
                tempCoords.row = coords.row + neighborsArray[x][1];
                findNeighborsReturn.push_back(tempCoords);
            }
        }
    }
    return findNeighborsReturn;
}

void Field::gameWon()
{
    Input input;

    extern std::atomic<bool> isGameRunning;
    isGameRunning = false;

    for (int i = 1; i <= this->cols; ++i)
    {
        for (int j = 1; j <= this->rows; ++j)
        {
            if (this->mines2DVector[i][j] == Symbols::getSymbol("symbolMine"))
            {
                this->field2DVector[i][j] = Symbols::getSymbol("symbolMine");
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
        Print::printDebugCoveredLeft(*this);
    #endif

    Print::printHasWon(*this);
    Common::gotoXY(this->fieldOffsetX - 1, this->fieldOffsetY + this->rows*2 + 3);
    input.getInputEnterKey("Press ENTER to get back...");
}

void Field::gameLost()
{
    Input input;

    extern std::atomic<bool> isGameRunning;
    isGameRunning = false;

    for (int i = 1; i <= this->cols; ++i)
    {
        for (int j = 1; j <= this->rows; ++j)
        {
            if (this->field2DVector[i][j] == Symbols::getSymbol("symbolFlag") && this->mines2DVector[i][j] \
            != Symbols::getSymbol("symbolMine") && this->mines2DVector[i][j] != Symbols::getSymbol("symbolMineHit"))
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

    #if DEBUG == 1
        Print::printDebugCoveredLeft(*this);
    #endif

    Print::printHasLost(*this);
    Common::gotoXY(this->fieldOffsetX - 1, this->fieldOffsetY + this->rows*2 + 3);
    input.getInputEnterKey("Press ENTER to get back...");
}

// automatically uncover all connected cells, as long as they have no neighbor mines:
void Field::autoUncoverRecursive(Common::CoordsStruct const& coords, std::vector<unsigned int> &poolVector)
{
    extern std::atomic<bool> doPauseTimer;
    bool wasPaused = false;
    (doPauseTimer == false) ? doPauseTimer = true : wasPaused = true;

    // create vector holding covered neighbors:
    std::vector<Common::CoordsStruct> neighborsCoveredVector;
    neighborsCoveredVector = findNeighbors(this->field2DVector, coords, Symbols::getSymbol("symbolCovered"));

    for (size_t i = 0; i < neighborsCoveredVector.size(); ++i)
    {
        std::vector<Common::CoordsStruct> neighborsMinesVector;
        neighborsMinesVector = findNeighbors(this->mines2DVector, neighborsCoveredVector.at(i), Symbols::getSymbol("symbolMine"));
        if (std::find(poolVector.begin(), poolVector.end(), Common::coordsToInt(neighborsCoveredVector.at(i), this->cols)) == poolVector.end())
        {
            if (neighborsMinesVector.size() == 0)
            {
                if (this->mines2DVector[neighborsCoveredVector.at(i).col][neighborsCoveredVector.at(i).row] != Symbols::getSymbol("symbolMine"))
                {
                    this->field2DVector[neighborsCoveredVector.at(i).col][neighborsCoveredVector.at(i).row] = Symbols::getSymbol("symbolZero");
                }
            }
            else
            {
                if (this->mines2DVector[neighborsCoveredVector.at(i).col][neighborsCoveredVector.at(i).row] != Symbols::getSymbol("symbolMine"))
                {
                    this->field2DVector[neighborsCoveredVector.at(i).col][neighborsCoveredVector.at(i).row] = Common::intToStringConv(static_cast<int>(neighborsMinesVector.size()));
                }
            }
            poolVector.push_back(Common::coordsToInt(neighborsCoveredVector.at(i), this->cols));
            printCoords(neighborsCoveredVector.at(i), false);
            --this->coveredLeft;

            #if DEBUG == 1
                #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
                    Sleep(20);
                #else
                    usleep(20*1000);
                #endif
                Print::printDebugCoveredLeft(*this);
            #endif
        }

        if (neighborsMinesVector.size() == 0)
        {
            autoUncoverRecursive(neighborsCoveredVector.at(i), poolVector);
        }
    }
    if (wasPaused != true) doPauseTimer = false;
}

void Field::flagAutoUncover(Common::CoordsStruct const& coords, Common::PlaceUserInputReturnStruct &returnStruct, bool hasCheated_)
{
    extern std::atomic<bool> doPauseTimer;
    extern std::atomic<bool> hasCheated;
    bool wasPaused = false;
    (doPauseTimer == false) ? doPauseTimer = true : wasPaused = true;

    // create a new vector of surrounding flags:
    std::vector<Common::CoordsStruct> flagUncoverNeighborsFlagsVector;
    flagUncoverNeighborsFlagsVector = findNeighbors(this->field2DVector, coords, Symbols::getSymbol("symbolFlag"));

    // if player has placed some flags around UserInput.coords:
    if (flagUncoverNeighborsFlagsVector.size() != 0)
    {
        // create a new vector of surrounding mines:
        std::vector<Common::CoordsStruct> flagUncoverNeighborsMinesVector;
        flagUncoverNeighborsMinesVector = findNeighbors(this->mines2DVector, coords, Symbols::getSymbol("symbolMine"));

        // only proceed if the flag number matches the number of actual surrounding mines:
        if (flagUncoverNeighborsMinesVector.size() == flagUncoverNeighborsFlagsVector.size())
        {
            // create a new vector of surrounding covered squares:
            std::vector<Common::CoordsStruct> flagUncoverNeighborsCoveredVector;
            flagUncoverNeighborsCoveredVector = findNeighbors(this->field2DVector, coords, Symbols::getSymbol("symbolCovered"));

            // create a new empty vector for missed mines:
            std::vector<Common::CoordsStruct> flagUncoverMissedMinesVector;

            // for each covered neighbor of userInput.coords check if the player has missed a mine
            // and add this mines position to flagUncoverMissedMinesVector:
            for (size_t i = 0; i < flagUncoverNeighborsCoveredVector.size(); ++i)
            {
                if (this->mines2DVector[flagUncoverNeighborsCoveredVector.at(i).col][flagUncoverNeighborsCoveredVector.at(i).row] == Symbols::getSymbol("symbolMine"))
                {
                    flagUncoverMissedMinesVector.push_back(flagUncoverNeighborsCoveredVector.at(i));
                }
            }
            // if there are missed mines, reveal the mines2DVector - player has lost:
            if (flagUncoverMissedMinesVector.size() != 0)
            {
                for (size_t i = 0; i < flagUncoverMissedMinesVector.size(); ++i)
                {
                    this->mines2DVector[flagUncoverMissedMinesVector.at(i).col][flagUncoverMissedMinesVector.at(i).row] = Symbols::getSymbol("symbolMineHit");
                }
                returnStruct.hasLost = true;
            }
            // else if all flags are placed correctly:
            else
            {
                if (flagUncoverNeighborsMinesVector.size() == flagUncoverNeighborsFlagsVector.size())
                {
                    if (hasCheated_) hasCheated = true;
                    // create a pool of already uncovered cells, to avoid double checks within autoUncoverRecursive():
                    std::vector<unsigned int> poolVector;

                    // for each covered neighbor of userInput.coords, print the number of surrounding mines:
                    for (size_t i = 0; i < flagUncoverNeighborsCoveredVector.size(); ++i)
                    {
                        Common::CoordsStruct tempCoords;
                        tempCoords.col = flagUncoverNeighborsCoveredVector.at(i).col;
                        tempCoords.row = flagUncoverNeighborsCoveredVector.at(i).row;
                        std::vector<Common::CoordsStruct> flagUncoverNeighborsCoveredMinesVector;
                        flagUncoverNeighborsCoveredMinesVector = findNeighbors(this->mines2DVector, tempCoords, Symbols::getSymbol("symbolMine"));

                        if (flagUncoverNeighborsCoveredMinesVector.size() == 0)
                        {
                            if (std::find(poolVector.begin(), poolVector.end(), Common::coordsToInt(tempCoords, this->cols)) == poolVector.end())
                            {
                                this->field2DVector[tempCoords.col][tempCoords.row] = Symbols::getSymbol("symbolZero");
                                printCoords(tempCoords, false);
                                poolVector.push_back(Common::coordsToInt(tempCoords, this->cols));
                                --this->coveredLeft;
                                autoUncoverRecursive(tempCoords, poolVector);
                            }
                        }
                        else
                        {
                            if (std::find(poolVector.begin(), poolVector.end(), Common::coordsToInt(tempCoords, this->cols)) == poolVector.end())
                            {
                                this->field2DVector[tempCoords.col][tempCoords.row] = Common::intToStringConv(static_cast<int>(flagUncoverNeighborsCoveredMinesVector.size()));
                                printCoords(tempCoords, false);
                                poolVector.push_back(Common::coordsToInt(tempCoords, this->cols));
                                --this->coveredLeft;
                            }
                        }
                    }
                }
            }
        }
    }
    if (wasPaused != true) doPauseTimer = false;
}

// the main method of class Field which will alter this->field2DVector.
// Takes single coords from Input::UserInput method.
const Common::PlaceUserInputReturnStruct &Field::placeUserInput(Common::UserInputReturnStruct& userInput, int& turn)
{
    // set or remove flag if requested
    if (userInput.isFlag == true)
    {
        if (isFlag(userInput.coords) == true)
        {
            this->field2DVector[userInput.coords.col][userInput.coords.row] = Symbols::getSymbol("symbolCovered");
            printCoords(userInput.coords, false);
            --this->flagsCount;
            ++this->minesLeft;
            ++this->coveredLeft;
        }
        else
        {
            this->field2DVector[userInput.coords.col][userInput.coords.row] = Symbols::getSymbol("symbolFlag");
            printCoords(userInput.coords, false);
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
            fillMines(userInput.coords);
        }

        // check if the player hit a mine which ends the game:
        if (this->mines2DVector[userInput.coords.col][userInput.coords.row] == Symbols::getSymbol("symbolMine"))
        {
            this->mines2DVector[userInput.coords.col][userInput.coords.row] = Symbols::getSymbol("symbolMineHit");
            gameLost();
            this->placeUserInputReturn.hasLost = true;
            return this->placeUserInputReturn;
        }

        // if the player has pressed ENTER on a number and has placed all flags right,
        // uncover all surrounding safe positions:
        if (isNumber(userInput.coords))
        {
            flagAutoUncover(userInput.coords, this->placeUserInputReturn, false);
            if (this->placeUserInputReturn.hasLost == true)
            {
                gameLost();
                return this->placeUserInputReturn;
            }
        }
        else
        {
            // uncover the players choice and place the number of surrounding mines in it:
            std::vector<Common::CoordsStruct> neighborsMinesVector = findNeighbors(this->mines2DVector, userInput.coords, Symbols::getSymbol("symbolMine"));
            if (neighborsMinesVector.size() == 0)
            {
                this->field2DVector[userInput.coords.col][userInput.coords.row] = Symbols::getSymbol("symbolZero");
            }
            else
            {
                this->field2DVector[userInput.coords.col][userInput.coords.row] = Common::intToStringConv(static_cast<int>(neighborsMinesVector.size()));
            }
            printCoords(userInput.coords, false);
            this->placeUserInputReturn.isTurn = true;
            --this->coveredLeft;

            // Call recursive method autoUncoverRecursive() to automatically uncover all connected cells, as long as
            // they have no neighbor mines:
            std::vector<Common::CoordsStruct> autoUncoverNeighborsMinesVector = findNeighbors(this->mines2DVector, userInput.coords, Symbols::getSymbol("symbolMine"));
            if (autoUncoverNeighborsMinesVector.size() == 0)
            {
                // create a pool of already uncovered cells, to avoid double checks within autoUncoverRecursive():
                std::vector<unsigned int> poolVector;
                autoUncoverRecursive(userInput.coords, poolVector);
            }
        }
    }

    // check if player has won:
    if (this->flagsCount + this->coveredLeft == this->minesTotal)
    {
        this->placeUserInputReturn.hasWon = true;
        gameWon();
    }
    
    #if MEM_LEAK_TEST_LOOP == 1
        this->placeUserInputReturn.hasLost = true;
    #endif

    return this->placeUserInputReturn;
}
