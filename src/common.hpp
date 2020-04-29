#pragma once

#define nl '\n'

class Common
{
private:
    
public:
    struct coordsStruct
    {
        int col = 0;
        int row = 0;
    };
    struct userInputStruct
    {
        coordsStruct coords;
        bool isFlag = false;
    };
    struct placeUserInput
    {
        bool hasLost = false;
        bool hasWon = false;
        bool isFlag = false;
    };
    void clearScreen();
    coordsStruct intToStruct(int, int);
    int structToInt(coordsStruct, int);
};

