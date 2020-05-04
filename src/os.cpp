#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include "windows.h"  
#endif

#include "os.hpp"

bool OS::isWindows()
{
    bool isWindowsBool = false;
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        isWindowsBool = true;
    #endif
    return isWindowsBool;
}
    
bool OS::isWine()
{
    bool isWineBool = false;
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)     
        HKEY hKey;
        LPCSTR lpRegPath = ("SOFTWARE\\Wine");
        if (RegOpenKeyExA(HKEY_CURRENT_USER, lpRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
            isWineBool = true;
    #endif
    return isWineBool;
}
