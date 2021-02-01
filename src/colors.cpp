#include <iostream>

#include "../include/colors.hpp"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#endif

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    std::string Colors::setTextColor(int const& colorCodeWindows)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, colorCodeWindows);
        return "";
    }
#else
    std::string Colors::setTextColor(std::string const& colorCodeLinux)
    {
        std::string color = colorCodeLinux;
        return color;
    }
#endif