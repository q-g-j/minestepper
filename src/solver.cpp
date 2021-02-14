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

#include <algorithm>
#include <memory>
#include <vector>

#include "../include/field.hpp"
#include "../include/solver.hpp"
#include "../include/symbols.hpp"

Solver::Solver()
{
    common = std::make_unique<Common>();
    symbols = std::make_unique<Symbols>();
}

Solver::~Solver()
{
}

void Solver::autoPlaceFlagsRecursive(Field& field)
{
    std::vector<int> poolCoveredVector;

    // for each cell in field2DVector:
    for (int i = 1; i <= field.getCols(); ++i)
    {
        for (int j = 1; j <= field.getRows(); ++j)
        {
            Common::CoordsStruct tempCoords;
            tempCoords.col = i;
            tempCoords.row = j;

            // if it's a number:
            if (field.isNumber(tempCoords))
            {
                // create 2 vectors: 
                // one for covered neighbors, 
                // one for neighbor flags
                std::vector<Common::CoordsStruct> coveredVector;
                coveredVector = field.findNeighbors(field.field2DVector, tempCoords, symbols->symbolCovered);
                if (coveredVector.size() != 0)
                {
                    std::vector<Common::CoordsStruct> flagsVector;
                    flagsVector = field.findNeighbors(field.field2DVector, tempCoords, symbols->symbolFlag);
                    
                    // if the number of covered neighbors plus the number of neighbor flags matches the current cells number,
                    // add the covered cells to poolCoveredVector:
                    if ((flagsVector.size() + coveredVector.size()) == static_cast<size_t>(common->stringToInt(field.getCoordsContent(tempCoords))))
                    {
                        for (size_t k = 0; k < coveredVector.size(); ++k)
                        {
                            if (std::find(poolCoveredVector.begin(), poolCoveredVector.end(), common->coordsToInt(coveredVector.at(0), field.getCols())) == poolCoveredVector.end())
                            {
                                poolCoveredVector.push_back(common->coordsToInt(coveredVector.at(k), field.getCols()));
                            }
                        }
                    }
                }
            }
        }
    }

    // place a flag at the coords of each element of poolCoveredVector:
    if (poolCoveredVector.size() != 0)
    {
        for (size_t i = 0; i < poolCoveredVector.size(); ++i)
        {
            Common::CoordsStruct tempCoords;
            Field::setCoveredLeft coveredLeft(field);
            Field::setMinesLeft minesLeft(field);
            Field::setFlagsCount flagsCount(field);
            tempCoords = common->intToCoords(poolCoveredVector.at(i), field.getCols());
            field.field2DVector[tempCoords.col][tempCoords.row] = symbols->symbolFlag;
            --coveredLeft;
            --minesLeft;
            ++flagsCount;
            field.printCoords(tempCoords, false);
        }
    }

    // for each number on the game field, create a vector holding covered neighbors.
    // If there are any covered neighbors, create a vector holding neighbor flags.
    // Then, if the number of flags matches the number inside the current cell, run 
    // field.flagAutoUncover() on the current cell:
    bool ranFlagAutoUncover = false;
    for (int col = 1; col <= field.getCols(); ++col)
    {
        for (int row = 1; row <= field.getRows(); ++row)
        {
            Common::CoordsStruct tempNumberCoords;
            tempNumberCoords.col = col;
            tempNumberCoords.row = row;
            if (field.isNumber(tempNumberCoords))
            {
                std::vector<Common::CoordsStruct> neighborsCoveredVector;
                neighborsCoveredVector = field.findNeighbors(field.field2DVector, tempNumberCoords, symbols->symbolCovered);
                if (neighborsCoveredVector.size() != 0)
                {
                    std::vector<Common::CoordsStruct> neighborsFlagsVector;
                    neighborsFlagsVector = field.findNeighbors(field.field2DVector, tempNumberCoords, symbols->symbolFlag);
                    if (neighborsFlagsVector.size() == static_cast<size_t>(common->stringToInt(field.field2DVector[col][row])))
                    {
                        Common::PlaceUserInputReturnStruct tempPlaceUserInputReturnStruct;
                        field.flagAutoUncover(tempNumberCoords, tempPlaceUserInputReturnStruct);
                        ranFlagAutoUncover = true;
                    }
                }
            }
        }
    }

    // re-run the whole function, if field.flagAutoUncover() was triggered:
    if (ranFlagAutoUncover == true)
    {
        autoPlaceFlagsRecursive(field);
    }
}


