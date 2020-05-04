#include <iostream>
#include "time.h"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include "windows.h"  
#endif

#include "common.hpp"
#include "debug.hpp"
#include "os.hpp"

using namespace std;

void Common::setRandomSeed()
{
    OS os;
    if (! (os.isWindows()) || os.isWine())
        srand(time(NULL));
    else
    {
        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            srand(GetTickCount());
        #endif
    }
}

void Common::clearScreen()
{
    OS os;
    if (os.isWine())
    {
        // clear screen with cout (won't work in native Windows):
        cout << "\x1B[2J\x1B[H";
    }
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        // Don't use "cls" in native Windows to reduce lag (snippet copied from Microsoft support pages):
        HANDLE hConsole;
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coordScreen = { 0, 0 };    /* here's where we'll home the
                                            cursor */
        BOOL bSuccess;
        DWORD cCharsWritten;
        CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
        DWORD dwConSize;                 /* number of character cells in
                                            the current buffer */

                                            /* get the number of character cells in the current buffer */

        bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
        dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

        /* fill the entire screen with blanks */

        bSuccess = FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten);

        /* get the current text attribute */

        bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);

        /* now set the buffer's attributes accordingly */

        bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);

        /* put the cursor at (0, 0) */

        bSuccess = SetConsoleCursorPosition(hConsole, coordScreen);
    #else
        if (system("clear") != 0) exit(0);
    #endif
}

// convert coords in type integer to coords in type struct (e.g. position = 4 will return coords.col = 4, coords.row = 1):
coordsStruct Common::intToStruct(int position, int cols)
{
    coordsStruct coords;
    
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
int Common::structToInt(coordsStruct coords, int cols)
{
    int position = 0;
    if (coords.row == 1)
        return position = coords.col;
    else
        return position = (cols) * (coords.row-1) + coords.col;
}

