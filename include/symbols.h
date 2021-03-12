#pragma once

// system headers:
#include <string>

class Symbols
{
public:
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        static std::wstring const& getSymbol(std::string const&);
    #else
        static std::string const& getSymbol(std::string const&);
    #endif
};
