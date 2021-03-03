#pragma once

// system headers:
#include <memory>

// forward declarations of classes:
class Common;
class Print;
class Symbols;

class Solver
{
private:
    // declare class objects as unique pointers:
    std::unique_ptr<Common> common;
    std::unique_ptr<Print> print;
    std::unique_ptr<Symbols> symbols;
    
public:
    Solver();
    ~Solver();

    // public methods:
    void autoSolve(Field&, bool, bool, bool);
};