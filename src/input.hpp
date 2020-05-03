#pragma once

class Input
{
private:

public:
    int getDifficulty();
    std::string moveUp = setMoveUp();
    coordsStruct getDimensions();
    int getMinesCount(int);
    bool getAnyKey();
    std::string setMoveUp();
    userInputReturnStruct getUserInput(Field &field);
};


