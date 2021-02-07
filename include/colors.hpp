#pragma once

#include "../include/debug.hpp"

class Colors
{
private:
    // color codes:
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        enum class ColorCode
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
        enum class ColorCode {
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

public:
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        const int color_default         = static_cast<int>(ColorCode::DEFAULT);

        const int mine                  = static_cast<int>(ColorCode::FG_LIGHT_RED);
        const int minehit               = static_cast<int>(ColorCode::FG_YELLOW);

        const int fg_covered            = static_cast<int>(ColorCode::FG_GREY);

        const int fg_white              = static_cast<int>(ColorCode::FG_WHITE);
        const int fg_grey               = static_cast<int>(ColorCode::FG_GREY);
        const int fg_yellow             = static_cast<int>(ColorCode::FG_YELLOW);
        const int fg_magenta            = static_cast<int>(ColorCode::FG_MAGENTA);
        const int fg_light_blue         = static_cast<int>(ColorCode::FG_LIGHT_BLUE);
        const int fg_light_green        = static_cast<int>(ColorCode::FG_LIGHT_GREEN);
        const int fg_light_red          = static_cast<int>(ColorCode::FG_LIGHT_RED);

        const int fg_number_1           = static_cast<int>(ColorCode::FG_LIGHT_BLUE);
        const int fg_number_2           = static_cast<int>(ColorCode::FG_LIGHT_GREEN);
        const int fg_number_3           = static_cast<int>(ColorCode::FG_LIGHT_RED);
        const int fg_number_4           = static_cast<int>(ColorCode::FG_MAGENTA);
        const int fg_number_5           = static_cast<int>(ColorCode::FG_YELLOW);
        const int fg_number_6           = static_cast<int>(ColorCode::FG_GREEN);
        const int fg_number_7           = static_cast<int>(ColorCode::FG_LIGHT_CYAN);
        const int fg_number_8           = static_cast<int>(ColorCode::FG_WHITE);
        const int fg_flag               = static_cast<int>(ColorCode::FG_RED);

        const int bg_covered            = static_cast<int>(ColorCode::BG_GREY);
        const int bg_number_1           = static_cast<int>(ColorCode::BG_LIGHT_BLUE);
        const int bg_number_2           = static_cast<int>(ColorCode::BG_LIGHT_GREEN);
        const int bg_number_3           = static_cast<int>(ColorCode::BG_LIGHT_RED);
        const int bg_number_4           = static_cast<int>(ColorCode::BG_MAGENTA);
        const int bg_number_5           = static_cast<int>(ColorCode::BG_YELLOW);
        const int bg_number_6           = static_cast<int>(ColorCode::BG_GREEN);
        const int bg_number_7           = static_cast<int>(ColorCode::BG_CYAN);
        const int bg_number_8           = static_cast<int>(ColorCode::BG_WHITE);
        const int bg_flag               = static_cast<int>(ColorCode::BG_RED);
    #else
        const std::string color_default         = "\033[" + std::to_string(static_cast<int>(ColorCode::DEFAULT)) + ";0m";

        const std::string mine                  = "\033[" + std::to_string(static_cast<int>(ColorCode::LIGHT_RED)) + "m";
        const std::string minehit               = "\033[" + std::to_string(static_cast<int>(ColorCode::YELLOW)) + "m";

        const std::string fg_covered            = "\033[" + std::to_string(static_cast<int>(ColorCode::GREY)) + "m";
        const std::string fg_white              = "\033[" + std::to_string(static_cast<int>(ColorCode::WHITE)) + ";1m";
        const std::string fg_red                = "\033[" + std::to_string(static_cast<int>(ColorCode::RED)) + "m";
        const std::string fg_green              = "\033[" + std::to_string(static_cast<int>(ColorCode::GREEN)) + "m";
        const std::string fg_yellow             = "\033[" + std::to_string(static_cast<int>(ColorCode::YELLOW)) + "m";
        const std::string fg_blue               = "\033[" + std::to_string(static_cast<int>(ColorCode::BLUE)) + "m";
        const std::string fg_magenta            = "\033[" + std::to_string(static_cast<int>(ColorCode::MAGENTA)) + "m";
        const std::string fg_cyan               = "\033[" + std::to_string(static_cast<int>(ColorCode::CYAN)) + "m";
        const std::string fg_grey               = "\033[" + std::to_string(static_cast<int>(ColorCode::GREY)) + "m";
        const std::string fg_light_red          = "\033[" + std::to_string(static_cast<int>(ColorCode::LIGHT_RED)) + "m";
        const std::string fg_light_green        = "\033[" + std::to_string(static_cast<int>(ColorCode::LIGHT_GREEN)) + "m";
        const std::string fg_light_yellow       = "\033[" + std::to_string(static_cast<int>(ColorCode::LIGHT_YELLOW)) + "m";
        const std::string fg_light_blue         = "\033[" + std::to_string(static_cast<int>(ColorCode::LIGHT_BLUE)) + "m";
        const std::string fg_light_magenta      = "\033[" + std::to_string(static_cast<int>(ColorCode::LIGHT_MAGENTA)) + "m";
        const std::string fg_light_cyan         = "\033[" + std::to_string(static_cast<int>(ColorCode::LIGHT_CYAN)) + "m";

        const std::string fg_number_1           = "\033[" + std::to_string(static_cast<int>(ColorCode::LIGHT_BLUE)) + "m";
        const std::string fg_number_2           = "\033[" + std::to_string(static_cast<int>(ColorCode::LIGHT_GREEN)) + "m";
        const std::string fg_number_3           = "\033[" + std::to_string(static_cast<int>(ColorCode::LIGHT_RED)) + "m";
        const std::string fg_number_4           = "\033[" + std::to_string(static_cast<int>(ColorCode::MAGENTA)) + "m";
        const std::string fg_number_5           = "\033[" + std::to_string(static_cast<int>(ColorCode::YELLOW)) + "m";
        const std::string fg_number_6           = "\033[" + std::to_string(static_cast<int>(ColorCode::GREEN)) + "m";
        const std::string fg_number_7           = "\033[" + std::to_string(static_cast<int>(ColorCode::LIGHT_MAGENTA)) + "m";
        const std::string fg_number_8           = "\033[" + std::to_string(static_cast<int>(ColorCode::WHITE)) + ";1m";
        const std::string fg_flag               = "\033[" + std::to_string(static_cast<int>(ColorCode::RED)) + ";1m";

        const std::string bg_covered            = "\033[" + std::to_string(static_cast<int>(ColorCode::DEFAULT)) + ";0m";
        const std::string bg_number_1           = "\033[47;" + std::to_string(static_cast<int>(ColorCode::LIGHT_BLUE)) + "m";
        const std::string bg_number_2           = "\033[47;" + std::to_string(static_cast<int>(ColorCode::LIGHT_GREEN)) + "m";
        const std::string bg_number_3           = "\033[47;" + std::to_string(static_cast<int>(ColorCode::LIGHT_RED)) + "m";
        const std::string bg_number_4           = "\033[47;" + std::to_string(static_cast<int>(ColorCode::MAGENTA)) + "m";
        const std::string bg_number_5           = "\033[47;" + std::to_string(static_cast<int>(ColorCode::YELLOW)) + "m";
        const std::string bg_number_6           = "\033[47;" + std::to_string(static_cast<int>(ColorCode::GREEN)) + "m";
        const std::string bg_number_7           = "\033[47;" + std::to_string(static_cast<int>(ColorCode::LIGHT_MAGENTA)) + "m";
        const std::string bg_number_8           = "\033[47;" + std::to_string(static_cast<int>(ColorCode::WHITE)) + ";1m";
        const std::string bg_flag               = "\033[47;" + std::to_string(static_cast<int>(ColorCode::RED)) + ";1m";
    #endif

    // public methods:
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        std::string setTextColor(int const&);
    #else
        std::string setTextColor(std::string const&);
    #endif
};
