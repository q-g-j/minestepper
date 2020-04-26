#pragma once

struct dimensionsStruct
{
    int cols;
    int rows;
};

int getDifficulty();
bool getAnyKey();
dimensionsStruct getDimensions();
int getBombsCount(int);


