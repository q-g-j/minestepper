#pragma once

class Board
{
private:
    int cols;
    int rows;
    int bombsCount;
    int bombsLeft = 0;
    char** boardArray;    
public:
    Board(int cols = 0, int rows = 0, int bombsCount = 0);
    ~Board();
    
    char** createBoardArray();
    void clearBoard();
    void drawBoard();
    void printBombsLeft();
    void printExplanation();
};
