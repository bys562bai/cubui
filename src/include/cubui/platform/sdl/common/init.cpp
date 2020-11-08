#include <SDL2/SDL.h>

#include <cubui/core/global.h>

namespace cubui{
    struct SDLInitNode{
        virtual Result init(Initializer* initzer){
            auto re = SDL_Init( SDL_INIT_VIDEO );
            if(re<0)
                return re;
            return 0;
        }

        const char* getError(){
            return SDL_GetError();
        }
    };

    namespace _inner_local_jdosdos{
        SDLInitNode g_sdlinitnode;
    }
}