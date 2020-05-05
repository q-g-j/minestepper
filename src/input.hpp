#pragma once

class Input
{
private:

public:
    int getDifficulty();
    coordsStruct getDimensions();
    int getMinesCount(int const&);
    void getEnterKey(std::string const&);
    void deleteLastLine(size_t const&);
    userInputReturnStruct getUserInput(Field &field);
};


