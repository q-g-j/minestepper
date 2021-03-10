#ifdef _DEBUG
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        #define _CRTDBG_MAP_ALLOC
        #include <stdlib.h>
        #include <crtdbg.h>
        #define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
        // Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
        // allocations to be of _CLIENT_BLOCK type
    #endif
#endif

// system headers:
#include <map>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#endif

// project headers:
#include <colors.h>
#include <debug.h>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    std::map<std::string, ColorCode> colorMap;
#else
    std::map<std::string, std::string> colorMap;
#endif

void Colors::initColorMap()
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        colorMap =
        {
        	{ "color_default", 		ColorCode::DEFAULT },

        	{ "mine", 		        ColorCode::FG_LIGHT_RED },
        	{ "minehit", 		    ColorCode::FG_YELLOW },

        	{ "fg_covered", 		ColorCode::FG_GREY },

        	{ "fg_white", 		    ColorCode::FG_WHITE },
        	{ "fg_grey", 		    ColorCode::FG_GREY },
        	{ "fg_yellow", 		    ColorCode::FG_YELLOW },
        	{ "fg_magenta", 		ColorCode::FG_MAGENTA },
        	{ "fg_light_blue", 		ColorCode::FG_LIGHT_BLUE },
        	{ "fg_light_green", 	ColorCode::FG_LIGHT_GREEN },
        	{ "fg_light_red", 		ColorCode::FG_LIGHT_RED },

        	{ "fg_number_1", 		ColorCode::FG_LIGHT_BLUE },
        	{ "fg_number_2", 		ColorCode::FG_LIGHT_GREEN },
        	{ "fg_number_3", 		ColorCode::FG_LIGHT_RED },
        	{ "fg_number_4", 		ColorCode::FG_MAGENTA },
        	{ "fg_number_5", 		ColorCode::FG_YELLOW },
        	{ "fg_number_6", 		ColorCode::FG_GREEN },
        	{ "fg_number_7", 		ColorCode::FG_LIGHT_CYAN },
        	{ "fg_number_8", 		ColorCode::FG_WHITE },
        	{ "fg_flag", 		    ColorCode::FG_RED },

        	{ "bg_covered", 		ColorCode::BG_GREY },
        	{ "bg_number_1", 		ColorCode::BG_LIGHT_BLUE },
        	{ "bg_number_2", 		ColorCode::BG_LIGHT_GREEN },
        	{ "bg_number_3", 		ColorCode::BG_LIGHT_RED },
        	{ "bg_number_4", 		ColorCode::BG_MAGENTA },
        	{ "bg_number_5", 		ColorCode::BG_YELLOW },
        	{ "bg_number_6", 		ColorCode::BG_GREEN },
        	{ "bg_number_7", 		ColorCode::BG_CYAN },
        	{ "bg_number_8", 		ColorCode::BG_WHITE },
        	{ "bg_flag", 		    ColorCode::BG_RED }
        };
    #else
        colorMap =
        {
            { "color_default",      "\033[" + std::to_string(ColorCode::DEFAULT) + ";0m" },
            { "mine",               "\033[" + std::to_string(ColorCode::LIGHT_RED) + ";1m" },
            { "minehit",            "\033[" + std::to_string(ColorCode::YELLOW) + "m" },

            { "fg_covered",         "\033[" + std::to_string(ColorCode::GREY) + "m" },

            { "fg_white",		    "\033[" + std::to_string(ColorCode::WHITE) + ";1m" },
            { "fg_red",		        "\033[" + std::to_string(ColorCode::RED) + "m" },
            { "fg_green",		    "\033[" + std::to_string(ColorCode::GREEN) + "m" },
            { "fg_yellow",		    "\033[" + std::to_string(ColorCode::YELLOW) + "m" },
            { "fg_blue",		    "\033[" + std::to_string(ColorCode::BLUE) + "m" },
            { "fg_magenta",		    "\033[" + std::to_string(ColorCode::MAGENTA) + "m" },
            { "fg_cyan",		    "\033[" + std::to_string(ColorCode::CYAN) + "m" },
            { "fg_grey",		    "\033[" + std::to_string(ColorCode::GREY) + "m" },
            { "fg_light_red",		"\033[" + std::to_string(ColorCode::LIGHT_RED) + "m" },
            { "fg_light_green",		"\033[" + std::to_string(ColorCode::LIGHT_GREEN) + "m" },
            { "fg_light_yellow",	"\033[" + std::to_string(ColorCode::LIGHT_YELLOW) + "m" },
            { "fg_light_blue",		"\033[" + std::to_string(ColorCode::LIGHT_BLUE) + "m" },
            { "fg_light_magenta",	"\033[" + std::to_string(ColorCode::LIGHT_MAGENTA) + "m" },
            { "fg_light_cyan",		"\033[" + std::to_string(ColorCode::LIGHT_CYAN) + "m" },

            { "fg_number_1",		"\033[" + std::to_string(ColorCode::LIGHT_BLUE) + "m" },
            { "fg_number_2",		"\033[" + std::to_string(ColorCode::LIGHT_GREEN) + "m" },
            { "fg_number_3",		"\033[" + std::to_string(ColorCode::LIGHT_RED) + "m" },
            { "fg_number_4",		"\033[" + std::to_string(ColorCode::MAGENTA) + "m" },
            { "fg_number_5",		"\033[" + std::to_string(ColorCode::YELLOW) + "m" },
            { "fg_number_6",		"\033[" + std::to_string(ColorCode::GREEN) + "m" },
            { "fg_number_7",		"\033[" + std::to_string(ColorCode::LIGHT_MAGENTA) + "m" },
            { "fg_number_8",		"\033[" + std::to_string(ColorCode::WHITE) + ";1m" },
            { "fg_flag",		    "\033[" + std::to_string(ColorCode::RED) + ";1m" },

            { "bg_covered",		    "\033[" + std::to_string(ColorCode::DEFAULT) + ";0m" },

            { "bg_number_1",		"\033[47;" + std::to_string(ColorCode::LIGHT_BLUE) + "m" },
            { "bg_number_2",		"\033[47;" + std::to_string(ColorCode::LIGHT_GREEN) + "m" },
            { "bg_number_3",		"\033[47;" + std::to_string(ColorCode::LIGHT_RED) + "m" },
            { "bg_number_4",		"\033[47;" + std::to_string(ColorCode::MAGENTA) + "m" },
            { "bg_number_5",		"\033[47;" + std::to_string(ColorCode::YELLOW) + "m" },
            { "bg_number_6",		"\033[47;" + std::to_string(ColorCode::GREEN) + "m" },
            { "bg_number_7",		"\033[47;" + std::to_string(ColorCode::LIGHT_MAGENTA) + "m" },
            { "bg_number_8",		"\033[47;" + std::to_string(ColorCode::WHITE) + ";1m" },
            { "bg_flag",		    "\033[47;" + std::to_string(ColorCode::RED) + ";1m" }
        };
    #endif
}

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    const std::string Colors::setTextColor(std::string const& color)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, colorMap[color]);
        return "";
    }
#else
    const std::string Colors::setTextColor(std::string const& color)
    {
        return colorMap[color];
    }
#endif