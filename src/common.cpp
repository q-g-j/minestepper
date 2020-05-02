#include <iostream>

#include "common.hpp"
#include "debug.hpp"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
    #include <winreg.h>
#endif

using namespace std;

void Common::clearScreen()
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)

        // check if running in wine (wine won't find "cls" or "clear"):
        HKEY hKey;
        LPCSTR lpRegPath = ("SOFTWARE\\Wine");
        if (RegOpenKeyExA(HKEY_CURRENT_USER, lpRegPath,0,KEY_ALL_ACCESS,&hKey) == ERROR_SUCCESS)
        {
            // clear screen with cout (won't work in native Windows):
            cout << "\x1B[2J\x1B[H";
        }

        // Don't use "cls" in native Windows to reduce lag (snippet copied from Microsoft support pages):
        else
        {
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

            bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
            dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

            /* fill the entire screen with blanks */

            bSuccess = FillConsoleOutputCharacter( hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten );

            /* get the current text attribute */

            bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );

            /* now set the buffer's attributes accordingly */

            bSuccess = FillConsoleOutputAttribute( hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten );

            /* put the cursor at (0, 0) */

            bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
        }
    #else

        // clear screen with cout if in non-Windows OS:
        // cout << "\x1B[2J\x1B[H";
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

