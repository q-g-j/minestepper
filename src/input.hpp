#pragma once

class Input
{
private:

public:
    int getDifficulty();
    coordsStruct getDimensions();
    int getMinesCount(int);
    bool getAnyKey();
    void deleteLastLine(size_t);
    userInputReturnStruct getUserInput(Field &field);
};


