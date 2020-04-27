#pragma once

class Input
{
private:

public:
    struct dimensionsStruct
    {
        unsigned int cols;
        unsigned int rows;
    };
    int getDifficulty();
    bool getAnyKey();
    int getBombsCount(int);
    dimensionsStruct getDimensions();
};


