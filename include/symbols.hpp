#pragma once

#include "../include/colors.hpp"
#include "../include/common.hpp"

class Symbols
{
private:
    Colors colors;    
public:
    // define frame and cell symbol constants:
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        const stringconv symbolCornerTopLeft = L"\u250C";
        const stringconv symbolCornerTopRight = L"\u2510";
        const stringconv symbolCornerBottomLeft = L"\u2514";
        const stringconv symbolCornerBottomRight = L"\u2518";
        const stringconv symbolHorizontalLine = L"\u2500";
        const stringconv symbolVerticalLine = L"\u2502";
        const stringconv symbolDownT = L"\u252C";
        const stringconv symbolUpT = L"\u2534";
        const stringconv symbolRightT = L"\u251C";
        const stringconv symbolLeftT = L"\u2524";
        const stringconv symbolPlus = L"\u253C";
        
        const stringconv symbolFlag = L"\u25ba";
        const stringconv symbolMine = L"\u263c";
        const stringconv symbolMineHit = L"#";
        const stringconv symbolZero = L" ";
        const stringconv symbolCovered = L"\u2591";
        const stringconv symbolCursor = L"\u2588";
    #else
        const stringconv symbolCornerTopLeft = "\u250C";
        const stringconv symbolCornerTopRight = "\u2510";
        const stringconv symbolCornerBottomLeft = "\u2514";
        const stringconv symbolCornerBottomRight = "\u2518";
        const stringconv symbolHorizontalLine = "\u2500";
        const stringconv symbolVerticalLine = "\u2502";
        const stringconv symbolDownT = "\u252C";
        const stringconv symbolUpT = "\u2534";
        const stringconv symbolRightT = "\u251C";
        const stringconv symbolLeftT = "\u2524";
        const stringconv symbolPlus = "\u253C";
        
        const stringconv symbolFlag = "\u25ba";
        const stringconv symbolMine = "\u263c";
        const stringconv symbolMineHit = "#";
        const stringconv symbolZero = " ";
        const stringconv symbolCovered = "\u2591";
        const stringconv symbolCursor = "\u2588";
    #endif
};
