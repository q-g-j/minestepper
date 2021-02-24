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
#include <codecvt>
#include <locale>
#include <string>
#include <time.h>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <fcntl.h>
    #include <io.h>
    #include <cstdlib>
    #include <stdio.h>
    #include <windows.h>
    #include <wingdi.h>
#endif

// project headers:
#include <colors.hpp>
#include <common.hpp>
#include <game.hpp>
#include <input.hpp>
#include <print.hpp>

// save original console screen size in a global variable:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    extern CONSOLE_SCREEN_BUFFER_INFO origScreenSize;

    void saveScreenSize()
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int columns, rows;

        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        origScreenSize = csbi;
    }
#else
    extern struct winsize origScreenSize;

    void saveScreenSize()
    {
        struct winsize osize;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &osize);
        origScreenSize = osize;
    }
#endif

Common::Common()
{
}

Common::~Common()
{
}

void Common::setWindowTitle(std::string const& titleText)
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        SetConsoleTitle(static_cast<LPCSTR>(titleText.c_str()));
    #else
        std::cout << "\033]0;" << titleText << "\007";
    #endif
}

// Windows only: disable maximizing of the console window:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    void Common::setWindowProperties()
    {
        HWND hwnd = GetConsoleWindow();
        DWORD style = GetWindowLong(hwnd, GWL_STYLE);
        style &= ~WS_MAXIMIZEBOX;
    //        style &= ~WS_CAPTION;
        SetWindowLong(hwnd, GWL_STYLE, style);
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_FRAMECHANGED);
    }
#endif

// Resize console window to desired size
// (Linux: needs a terminal that supports control sequences):
void Common::resizeConsole(int const& cols, int const& rows)
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        std::string str = "MODE CON COLS=" + std::to_string(cols) + "LINES=" + std::to_string(rows);
        char const* cstr = str.c_str();
        system(cstr);
    #else
        std::string x = std::to_string(cols);
        std::string y = std::to_string(rows);
        std::cout << "\033[8;" + y + ";" + x + "t";
    #endif
}

// Windows only: move console window to desired position:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    void Common::centerWindow()
    {
        RECT rectWindow;
        HWND hWnd = GetConsoleWindow();
        GetWindowRect(hWnd, &rectWindow);
        int posx, posy;
        posx = GetSystemMetrics(SM_CXSCREEN) / 2 - (rectWindow.right - rectWindow.left) / 2,
        posy = GetSystemMetrics(SM_CYSCREEN) / 2 - (rectWindow.bottom - rectWindow.top) / 2,

        MoveWindow(hWnd, posx, posy, rectWindow.right - rectWindow.left, rectWindow.bottom - rectWindow.top, TRUE);
    }
#endif

// enable unicode mode in Windows to be able to print the symbols:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    void Common::setUnicode(bool sw)
    {
        if (sw)
        {
            int result =_setmode(_fileno(stdout), 0x00020000);
            if (result == -1) exitProgram(1);
        }
        else
        {
            int result = _setmode(_fileno(stdout), _O_TEXT);
            if (result == -1) exitProgram(1);
        }
    }
#endif

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    int Common::stringToInt(std::wstring wstr)
    {
        int num = stoi(wstr);
        return num;
    }
#else
    int Common::stringToInt(std::string str)
    {
        int num = stoi(str);
        return num;
    }
#endif

// convert a string to wide string and vice versa:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
std::wstring Common::stringConvert(std::string const& str)
    {
        using convert_t = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_t, wchar_t> strconverter;
        return strconverter.from_bytes(str);
    }
#else
std::string Common::stringConvert(std::wstring const& wstr)
    {
        using convert_t = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_t, wchar_t> strconverter;
        return strconverter.to_bytes(wstr);
    }
#endif

// convert an integer to string or wide string:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    std::wstring Common::intToString(int const& num)
    {
        return std::to_wstring(num);
    }
#else
    std::string Common::intToString(int const& num)
    {
        return std::to_string(num);
    }
#endif

// go to position on screen (x = column, y = row):
void Common::gotoXY(int const& x, int const& y)
{
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordsNew;

    coordsNew.X = x;
    coordsNew.Y = y;
    SetConsoleCursorPosition(hConsole, coordsNew);
#else
    printf("%c[%d;%df", 0x1B, y + 1, x + 1);
#endif
}

// clear the whole screen. Used rarely to avoid screen blinking / slow refresh during the game:
void Common::clearScreen()
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        if (system("cls") != 0) exitProgram(1);
    #else
//        try
//        {
//            system("clear");
//        }
//        catch (...)
//        {
//            std::cout << "\x1B[2J\x1B[H";
//        }
    if (system("clear")) {};
    std::cout << "\x1B[2J\x1B[H";
    #endif
}

// convert coords of type integer to coords of type struct
// (e.g. "position = 4" will return "coords.col = 4, coords.row = 1"):
Common::CoordsStruct Common::intToCoords(int const& position, int const& cols)
{
    CoordsStruct coords;

    if (position <= cols)
    {
        coords.col = position;
        coords.row = 1;
    }
    else if (position % cols == 0)
    {
        coords.col = cols;
        coords.row = position / cols;
    }
    else
    {
        coords.col = position % cols;
        coords.row = position / cols + 1;
    }
    return coords;
}

// the above function the other way around
unsigned int Common::coordsToInt(CoordsStruct const& coords, int const& cols)
{
    int position = 0;
    if (coords.row == 1)
    {
        return position = coords.col;
    }
    else
    {
        return position = (cols) * (coords.row-1) + coords.col;
    }
}

// convert field coordinates to cursor coordinates:
Common::CoordsStruct Common::coordsToCursorPosition(CoordsStruct const& coords, int const& offsetX, int const& offsetY, int const& cellWidth)
{
    CoordsStruct cursorPosition;
    cursorPosition.col = offsetX + (cellWidth-1)/2 - 1;
    if (cursorPosition.col > 1)
    {
    for (int i = 1; i < coords.col; ++i)
        {
            cursorPosition.col = cursorPosition.col + (cellWidth-1)/2 + 1 + (cellWidth-1)/2 + 1;
        }
    }

    cursorPosition.row = offsetY;
    for (int i = 1; i < coords.row; ++i)
    {
        cursorPosition.row = cursorPosition.row + 2;
    }
    return cursorPosition;
}

// restore screen size, font color and cursor visibility at program exit:
void Common::exitProgram(int const& errorCode)
{
    Colors colors;
    Print print;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        extern CONSOLE_SCREEN_BUFFER_INFO origScreenSize;
    #else
        extern struct winsize origScreenSize;
    #endif

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        int columns = origScreenSize.srWindow.Right - origScreenSize.srWindow.Left + 1;
        int rows = origScreenSize.srWindow.Bottom - origScreenSize.srWindow.Top + 1;
        resizeConsole(columns, rows);
        centerWindow();
        setUnicode(false);
    #else
        resizeConsole(origScreenSize.ws_col, origScreenSize.ws_row);
        disableNonCanonicalMode();
    #endif

    print.showBlinkingCursor(true);
    colors.setTextColor(colors.color_default);

    clearScreen();
    exit (errorCode);
}
