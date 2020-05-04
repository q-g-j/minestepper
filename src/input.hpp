#pragma once

class Input
{
private:

public:
    int getDifficulty();
    coordsStruct getDimensions();
    int getMinesCount(int);
    bool getAnyKey();
    void deleteLastLines(int numLines);
    userInputReturnStruct getUserInput(Field &field);
};


