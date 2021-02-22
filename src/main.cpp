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

// project headers:
#include <common.hpp>
#include <game.hpp>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    CONSOLE_SCREEN_BUFFER_INFO origScreenSize;
#else
    struct winsize origScreenSize;
#endif

int main()
{
    origScreenSize = saveScreenSize();

    #if defined(_DEBUG)
        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );        
        #endif
    #endif

    while (true)
    {
        Game game;
        game.startGame();
    }

    return 0;
}
