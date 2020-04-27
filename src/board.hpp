#pragma once

class Board
{
private:
    unsigned int cols;
    unsigned int rows;
    unsigned int bombsCount;
    char** boardArray;    
public:
    Board(unsigned int cols = 0, unsigned int rows = 0, unsigned int bombsCount = 0);
    ~Board();
    
    char** createBoardArray();
    void clearBoard();
    void drawBoard();
    void printExplanation();
};
