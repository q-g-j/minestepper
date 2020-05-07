#pragma once

#if !defined(_WIN32) && !defined(WIN32) && !defined(_WIN64) && !defined(WIN64)
    void enableNonCanonicalMode();
    void disableNonCanonicalMode();
#endif

class Input
{
private:

public:
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        #define KEY_UP 72
        #define KEY_DOWN 80
        #define KEY_LEFT 75
        #define KEY_RIGHT 77
        #define KEY_ENTER '\r'
        #define KEY_SPACE ' '
    #else
        #define KEY_UP 'A'
        #define KEY_DOWN 'B'
        #define KEY_LEFT 'D'
        #define KEY_RIGHT 'C'
        #define KEY_ENTER '\n'
        #define KEY_SPACE ' '
    #endif
    
    int getDifficulty();
    coordsStruct getDimensions();
    int getMinesCount(int const&);
    void hideCursor();
    void showCursor();
    void getEnterKey(std::string const&);
    void deleteLastLine(size_t const&);
    userInputReturnStruct getUserInput(Field &field, int);
};


