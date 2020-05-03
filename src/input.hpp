#pragma once

class Input
{
private:

public:
    int getDifficulty();
    coordsStruct getDimensions();
    int getMinesCount(int);
    bool getAnyKey();
    userInputReturnStruct getUserInput(Field &field);
};


