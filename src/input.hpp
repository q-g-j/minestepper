#pragma once

#include "common.hpp"

class Input
{
private:

public:
    int getDifficulty(Common &common);
    Common::coordsStruct getDimensions(Common &common);
    int getBombsCount(Common &common, int);
    bool getAnyKey();
    Common::coordsStruct getUserInput(Common &common, Board &board);
};


