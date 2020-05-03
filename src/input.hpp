#pragma once

class Input
{
private:

public:
    int getDifficulty();
    coordsStruct getDimensions();
    int getMinesCount(int);
    bool getAnyKey();
    void moveCursorUp();
    userInputReturnStruct getUserInput(Field &field);
};


