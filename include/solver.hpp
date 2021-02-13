#pragma once

// forward declarations of classes:
class Common;
class Symbols;

class Solver
{
private:
    // declare class objects as unique pointers:
    std::unique_ptr<Common> common;
    std::unique_ptr<Symbols> symbols;
public:
    Solver();
    ~Solver();

    // public methods:
    void autoPlaceFlagsRecursive(Field&);
};