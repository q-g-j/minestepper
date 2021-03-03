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
#include <chrono>
#include <codecvt>
#include <locale>
#include <iostream>
#include <math.h>
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

#if defined(__MINGW32__)
    #include <mingw.thread.h> // needed for std::this_thread (https://github.com/meganz/mingw-std-threads)
#else
    #include <thread>
#endif

// project headers:
#include <colors.hpp>
#include <common.hpp>
#include <debug.hpp>
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

// restore screen size, font color and cursor visibility at program exit:
void cleanUp()
{
    Common common;
    Colors colors;
    Input input;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        extern CONSOLE_SCREEN_BUFFER_INFO origScreenSize;
    #else
        extern struct winsize origScreenSize;
    #endif

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        int columns = origScreenSize.srWindow.Right - origScreenSize.srWindow.Left + 1;
        int rows = origScreenSize.srWindow.Bottom - origScreenSize.srWindow.Top + 1;
        common.resizeConsole(columns, rows);
        common.centerWindow();
        common.setUnicode(false);
    #else
        common.resizeConsole(origScreenSize.ws_col, origScreenSize.ws_row);
        disableNonCanonicalMode();
    #endif

    input.showBlinkingCursor(true);
    colors.setTextColor(colors.color_default);

    common.clearScreen();
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
            if (result == -1) exit(1);
        }
        else
        {
            int result = _setmode(_fileno(stdout), _O_TEXT);
            if (result == -1) exit(1);
        }
    }
#endif

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    const int &Common::stringToInt(std::wstring wstr)
    {
        this->stringToIntReturn = stoi(wstr);
        return this->stringToIntReturn;
    }
#else
    const int &Common::stringToInt(std::string str)
    {
        this->stringToIntReturn = stoi(str);
        return this->stringToIntReturn;
    }
#endif

// convert a string to wide string and vice versa:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
const std::wstring & Common::stringConvert(std::string const& str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> stringConvertTemp;
        this->stringConvertReturn = stringConvertTemp.from_bytes(str);
        return this->stringConvertReturn;
    }
#else
const std::string &Common::stringConvert(std::wstring const& wstr)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> stringConvertTemp;
        this->stringConvertReturn = stringConvertTemp.to_bytes(wstr);
        return this->stringConvertReturn;
    }
#endif

// convert an integer to string or wide string:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    const std::wstring &Common::intToStringConv(int const& num)
    {
        this->intToStringConvReturn = std::to_wstring(num);
        return this->intToStringConvReturn;
    }
#else
    const std::string &Common::intToStringConv(int const& num)
    {
        this->intToStringConvReturn = std::to_string(num);
        return this->intToStringConvReturn;
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
        if (system("cls") != 0) exit(1);
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
const Common::CoordsStruct &Common::intToCoords(int const& position, int const& cols)
{
    if (position <= cols)
    {
        this->intToCoordsReturn.col = position;
        this->intToCoordsReturn.row = 1;
    }
    else if (position % cols == 0)
    {
        this->intToCoordsReturn.col = cols;
        this->intToCoordsReturn.row = position / cols;
    }
    else
    {
        this->intToCoordsReturn.col = position % cols;
        this->intToCoordsReturn.row = position / cols + 1;
    }
    return this->intToCoordsReturn;
}

// the above function the other way around
const int &Common::coordsToInt(CoordsStruct const& coords, int const& cols)
{
    this->coordsToIntReturn = 1;
    if (coords.row == 1)
    {
        this->coordsToIntReturn = coords.col;
    }
    else
    {
        this->coordsToIntReturn = (cols) * (coords.row-1) + coords.col;
    }
    return this->coordsToIntReturn;
}

// convert field coordinates to cursor coordinates:
const Common::CoordsStruct &Common::coordsToCursorPosition(CoordsStruct const& coords, int const& offsetX, int const& offsetY, int const& cellWidth)
{
    this->coordsToCursorPositionReturn.col = offsetX + (cellWidth-1)/2 - 1;
    if (this->coordsToCursorPositionReturn.col > 1)
    {
    for (int i = 1; i < coords.col; ++i)
        {
            this->coordsToCursorPositionReturn.col = this->coordsToCursorPositionReturn.col + (cellWidth-1)/2 + 1 + (cellWidth-1)/2 + 1;
        }
    }

    this->coordsToCursorPositionReturn.row = offsetY;
    for (int i = 1; i < coords.row; ++i)
    {
        this->coordsToCursorPositionReturn.row = this->coordsToCursorPositionReturn.row + 2;
    }
    return this->coordsToCursorPositionReturn;
}

// replacement for Sleep() and usleep() for a more precise sleep:
// (code snippet from https://blat-blatnik.github.io/computerBear/making-accurate-sleep-function/)
void Common::preciseSleep(double seconds)
{
    static double estimate = 5e-3;
    static double mean = 5e-3;
    static double m2 = 0;
    static int64_t count = 1;

    while (seconds > estimate) {
        auto start = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        auto end = std::chrono::high_resolution_clock::now();

        double observed = (end - start).count() / 1e9;
        seconds -= observed;

        ++count;
        double delta = observed - mean;
        mean += delta / count;
        m2 += delta * (observed - mean);
        double stddev = sqrt(m2 / (count - 1));
        estimate = mean + stddev;
    }

    // spin lock
    auto start = std::chrono::high_resolution_clock::now();
    while ((std::chrono::high_resolution_clock::now() - start).count() / 1e9 < seconds);
}

const Common::TimeStruct &Common::secondsToTimeStruct(int seconds)
{
    if (seconds < 60)
    {
        this->secondsToTimeStructReturn.minutes = "00";
        if (seconds < 10)
        {
            this->secondsToTimeStructReturn.seconds = "0" + std::to_string(seconds);
        }
        else
        {
            this->secondsToTimeStructReturn.seconds = std::to_string(seconds);
        }
        return this->secondsToTimeStructReturn;
    }
    else
    {
        if (seconds < 600)
        {
            this->secondsToTimeStructReturn.minutes = "0" + std::to_string(seconds / 60);
        }
        else
        {
            this->secondsToTimeStructReturn.minutes = std::to_string(seconds / 60);
        }
        if (seconds % 60 == 0)
        {
            this->secondsToTimeStructReturn.seconds = "00";
        }
        else if (seconds % 60 < 10)
        {
            this->secondsToTimeStructReturn.seconds = "0" + std::to_string(seconds % 60);
        }
        else
        {
            this->secondsToTimeStructReturn.seconds = std::to_string(seconds % 60);
        }
        return this->secondsToTimeStructReturn;
    }
}
