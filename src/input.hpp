#pragma once

#include "common.hpp"

class Input
{
private:

public:
    int getDifficulty();
    Common::coordsStruct getDimensions();
    int getBombsCount(int);
    bool getAnyKey();
    Common::userInputStruct getUserInput(Field &field);
};


