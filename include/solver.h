#pragma once

// forward declarations of classes:
class Common;
class Print;
class Symbols;

class Solver
{
private:
    
public:
    Solver();
    ~Solver();

    // public methods:
    void autoSolve(Field&, bool, bool, bool);
};