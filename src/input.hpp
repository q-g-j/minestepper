#pragma once

class Input
{
private:

public:
    int getDifficulty();
    coordsStruct getDimensions();
    int getMinesCount(int);
    void getEnterKey(std::string);
    void deleteLastLine(size_t);
    userInputReturnStruct getUserInput(Field &field);
};


