#pragma once

#include <string>

// color codes:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    enum ColorCode
    {
        DEFAULT             = 7,
        FG_GREY             = 8,
        FG_WHITE            = 15,
        FG_RED              = 4,
        FG_GREEN            = 2,
        FG_YELLOW           = 14,
        FG_BLUE             = 1,
        FG_MAGENTA          = 5,
        FG_CYAN             = 3,
        FG_BROWN            = 6,
        FG_LIGHT_RED        = 12,
        FG_LIGHT_GREEN      = 10,
        FG_LIGHT_BLUE       = 9,
        FG_LIGHT_MAGENTA    = 13,
        FG_LIGHT_CYAN       = 11,

        // grey background with different colors:
        BG_BLACK            = 112,
        BG_DARK_BLUE        = 113,
        BG_GREEN            = 114,
        BG_LIGHT_BLUE       = 115,
        BG_RED              = 116,
        BG_DARKEST_BLUE     = 117,
        BG_WHITE            = 118,
        BG_GREY             = 119,
        BG_BLUE             = 121,
        BG_LIGHT_GREEN      = 122,
        BG_CYAN             = 123,
        BG_LIGHT_RED        = 124,
        BG_MAGENTA          = 125,
        BG_YELLOW           = 126,
        BG_LIGHT_GREY       = 127
    };
#else
    enum ColorCode {
        DEFAULT          = 39,
        BLACK            = 30,
        WHITE            = 37,
        RED              = 31,
        GREEN            = 32,
        YELLOW           = 33,
        BLUE             = 34,
        MAGENTA          = 35,
        CYAN             = 36,
        LIGHT_RED        = 91,
        LIGHT_GREEN      = 92,
        LIGHT_YELLOW     = 93,
        LIGHT_BLUE       = 94,
        LIGHT_MAGENTA    = 95,
        LIGHT_CYAN       = 96,
        GREY             = 90
    };
#endif

class Colors
{
public:
    static void initColorMap();

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        static const std::string setTextColor(std::string const&);
    #else
        static const std::string setTextColor(std::string const&);
    #endif
};
