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
    struct userInputReturnStruct
    {
        coordsStruct coords;
        bool isFlag = false;
    };
    struct placeUserInputReturnStruct
    {
        bool hasLost = false;
        bool hasWon = false;
        bool isTurn = false;
    };
    void clearScreen();
    coordsStruct intToStruct(int, int);
    int structToInt(coordsStruct, int);
};

typedef struct Common::coordsStruct coordsStruct;
typedef struct Common::userInputReturnStruct userInputReturnStruct;
typedef struct Common::placeUserInputReturnStruct placeUserInputReturnStruct;

