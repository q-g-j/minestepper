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
#include <colors.h>
#include <common.h>
#include <debug.h>
#include <game.h>
#include <symbols.h>

int main()
{
    #if defined(_DEBUG)
        #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
            _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );        
        #endif
    #endif
    
    Common::saveScreenSize();
    atexit(Common::cleanUp);

    while (true)
    {
        Game game;
        game.startGame();
    }

    return 0;
}
