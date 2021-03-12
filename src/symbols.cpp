// system headers:
#include <map>

// project headers:
#include <symbols.h>

// define frame and cell symbol constants:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    std::map<std::string, std::wstring> symbolsMap
    {
        { "symbolCornerTopLeft",		L"\u250C" },
        { "symbolCornerTopRight",		L"\u2510" },
        { "symbolCornerBottomLeft",		L"\u2514" },
        { "symbolCornerBottomRight",	L"\u2518" },
        { "symbolHorizontalLine",		L"\u2500" },
        { "symbolVerticalLine",			L"\u2502" },
        { "symbolDownT",			    L"\u252C" },
        { "symbolUpT",			        L"\u2534" },
        { "symbolRightT",			    L"\u251C" },
        { "symbolLeftT",			    L"\u2524" },
        { "symbolPlus",			        L"\u253C" },

        { "symbolFlag",			        L"\u25ba" },
        { "symbolMine",			        L"\u263c" },
        { "symbolMineHit",		        L"#" },
        { "symbolZero",			        L" " },
        { "symbolCovered",			    L"\u2588" },
        { "symbolCursor",			    L"\u2588" }
    };
#else
    std::map<std::string, std::string> symbolsMap
    {
        { "symbolCornerTopLeft",		"\u250C" },
        { "symbolCornerTopRight",		"\u2510" },
        { "symbolCornerBottomLeft",		"\u2514" },
        { "symbolCornerBottomRight",	"\u2518" },
        { "symbolHorizontalLine",		"\u2500" },
        { "symbolVerticalLine",		    "\u2502" },
        { "symbolDownT",			    "\u252C" },
        { "symbolUpT",			        "\u2534" },
        { "symbolRightT",			    "\u251C" },
        { "symbolLeftT",			    "\u2524" },
        { "symbolPlus",			        "\u253C" },

        { "symbolFlag",			        "\u25ba" },
        { "symbolMine",			        "\u263c" },
        { "symbolMineHit",			    "#" },
        { "symbolZero",			        " " },
        { "symbolCovered",			    "\u2588" },
        { "symbolCursor",			    "\u2588" }
    };
#endif

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    std::wstring const& Symbols::getSymbol(std::string const& symbol)
    {
        return symbolsMap[symbol];
    }
#else
    std::string const& Symbols::getSymbol(std::string const& symbol)
    {
        return symbolsMap[symbol];
    }
#endif