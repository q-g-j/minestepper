#include <algorithm>
#include <vector>

#include "../include/field.hpp"
#include "../include/solver.hpp"
#include "../include/symbols.hpp"

void Solver::solveGame(Field& field)
{
    Common common;
    Symbols symbols;

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
                // create 2 vectors: 1 for covered neighbors, 1 for flag neighbors:
                std::vector<Common::CoordsStruct> coveredVector;
                std::vector<Common::CoordsStruct> flagsVector;
                std::vector<Common::CoordsStruct> flagsNewVector;
                coveredVector = field.findNeighbors(field.field2DVector, tempCoords, symbols.symbolCovered);
                flagsVector = field.findNeighbors(field.field2DVector, tempCoords, symbols.symbolFlag);

                for (size_t f = 0; f < flagsVector.size(); ++f)
                {
                    flagsNewVector.push_back(flagsVector.at(f));
                }

                // if the number of covered neighbos plus the number of flag neighbors matches the current cells number,
                // add the covered cells to poolCoveredVector:
                if ((flagsVector.size() + coveredVector.size()) == common.stringToInt(field.getCoordsContent(tempCoords)))
                {
                    for (size_t k = 0; k < coveredVector.size(); ++k)
                    {
                        {
                            if (std::find(poolCoveredVector.begin(), poolCoveredVector.end(), common.coordsToInt(coveredVector.at(0), field.getCols())) == poolCoveredVector.end())
                            {
                                poolCoveredVector.push_back(common.coordsToInt(coveredVector.at(k), field.getCols()));
                            }
                        }
                    }
                }
            }
        }
    }

    if (poolCoveredVector.size() != 0)
    {
        for (size_t i = 0; i < poolCoveredVector.size(); ++i)
        {
            Common::CoordsStruct tempCoords;
            tempCoords = common.intToCoords(poolCoveredVector.at(i), field.getCols());
            field.field2DVector[tempCoords.col][tempCoords.row] = symbols.symbolFlag;
            --field.minesLeft;
            field.printCoords(tempCoords, false);
        }
    }

    bool ranFlagAutoUncover = false;
    // for each number on the game field, create a vector holding the neighbor flags and
    // create a vector of covered neighbors.
    // Then, if the number of found flags matches the number inside the current cell and
    // if there are any covered neighbors, run field.flagAutoUncover on the current cell:
    for (int col = 1; col <= field.getCols(); ++col)
    {
        for (int row = 1; row <= field.getRows(); ++row)
        {
            Common::CoordsStruct tempNumberCoords;
            tempNumberCoords.col = col;
            tempNumberCoords.row = row;
            if (field.isNumber(tempNumberCoords))
            {
                std::vector<Common::CoordsStruct> neighborsFlagsVector;
                std::vector<Common::CoordsStruct> neighborsCoveredVector;
                neighborsFlagsVector = field.findNeighbors(field.field2DVector, tempNumberCoords, symbols.symbolFlag);
                neighborsCoveredVector = field.findNeighbors(field.field2DVector, tempNumberCoords, symbols.symbolCovered);

                if (neighborsFlagsVector.size() == common.stringToInt(field.field2DVector[col][row]) && neighborsCoveredVector.size() != 0)
                {
                    Common::UserInputReturnStruct tempUserInputReturnStruct;
                    Common::PlaceUserInputReturnStruct tempPlaceUserInputReturnStruct;
                    tempUserInputReturnStruct.Coords = tempNumberCoords;
                    field.flagAutoUncover(tempUserInputReturnStruct, tempPlaceUserInputReturnStruct);
                    ranFlagAutoUncover = true;
                }
            }
        }
    }

    if (poolCoveredVector.size() != 0 || ranFlagAutoUncover)
    {
        solveGame(field);
    }
}

        //            // next step: run field.flagAutoUncover() on each neighbor of each new flag, that holds
        //            // a number that equals the flag count:
        //            //
        //            // create a vector that will hold all these numbers and avoid duplicates:
        //            std::vector<unsigned int> flagsNeighborsNumbersVector;
        //            
        //            // for each flag cell:
        //            for (size_t f = 0; f < flagsNewVector.size(); ++f)
        //            {
        //                // create a temporary vector that holds all numbered neighbors of the flag cell that match the number of flags:
        //                std::vector<Common::CoordsStruct> tempVector;
        //                tempVector = field.findNeighbors(field.field2DVector, flagsNewVector.at(f), common.intToString(flagsNewVector.size()));
        //                
        //                // add each found cell to flagsNeighborsNumbersVector, if not already done: 
        //                for (size_t t = 0; t < tempVector.size(); ++t)
        //                {
        //                    if (std::find(flagsNeighborsNumbersVector.begin(), flagsNeighborsNumbersVector.end(), common.coordsToInt(tempVector.at(t), field.getCols())) == flagsNeighborsNumbersVector.end())
        //                    {
        //                        flagsNeighborsNumbersVector.push_back(common.coordsToInt(tempVector.at(t), field.getCols()));
        //                    }
        //                }
        //            }

        //            // for each of these found numbered cells run fiel.flagAutoUncover():
        //            for (size_t f = 0; f < flagsNeighborsNumbersVector.size(); ++f)
        //            {
        //                Common::CoordsStruct tempCoords2 = common.intToCoords(flagsNeighborsNumbersVector.at(f), field.getCols());
        //                int col = tempCoords2.col;
        //                int row = tempCoords2.row;
        //                Common::UserInputReturnStruct tempUserInputReturnStruct;
        //                Common::PlaceUserInputReturnStruct tempPlaceUserInputReturnStruct;
        //                tempUserInputReturnStruct.Coords = tempCoords2;
        //                field.flagAutoUncover(tempUserInputReturnStruct, tempPlaceUserInputReturnStruct);
        //            }
        //        }
        //    }
        //}

    //for (int i = 1; i <= field.getCols(); ++i)
    //{
    //    for (int j = 1; j <= field.getRows(); ++j)
    //    {
    //        Common::CoordsStruct tempCoords;
    //        tempCoords.col = i;
    //        tempCoords.row = j;

    //        // if it's a number:
    //        if (field.isNumber(tempCoords))
    //        {
    //            // create 2 vectors: 1 for covered neighbors, 1 for flag neighbors:
    //            std::vector<Common::CoordsStruct> coveredVector;
    //            std::vector<Common::CoordsStruct> flagsVector;
    //            coveredVector = field.findNeighbors(field.field2DVector, tempCoords, symbols.symbolCovered);
    //            flagsVector = field.findNeighbors(field.field2DVector, tempCoords, symbols.symbolFlag);

    //            if (coveredVector.size() != 0)
    //            {
    //                // if the number of covered neighbors plus the number of flag neighbors matches the current cells number,
    //                // put a flag on every covered neighbor and print it:
    //                if ((flagsVector.size() + coveredVector.size()) == common.stringToInt(field.getCoordsContent(tempCoords)))
    //                {
    //                    Common::UserInputReturnStruct tempUserInputReturnStruct;
    //                    Common::PlaceUserInputReturnStruct tempPlaceUserInputReturnStruct;
    //                    tempUserInputReturnStruct.Coords = tempCoords;
    //                    field.flagAutoUncover(tempUserInputReturnStruct, tempPlaceUserInputReturnStruct);
    //                }
    //            }
    //        }
    //    }
    //}


