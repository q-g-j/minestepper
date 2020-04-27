#pragma once

class Input
{
private:

public:
    struct dimensionsStruct
    {
        int cols;
        int rows;
    };
    int getDifficulty();
    bool getAnyKey();
    int getBombsCount(int);
    dimensionsStruct getDimensions();
};


