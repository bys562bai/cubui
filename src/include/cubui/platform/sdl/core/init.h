#include <SDL2/SDL.h>

#include <cubui/core/initnode.h>

namespace cubui{
    struct SDLInitNode : InitNode{
        virtual Result init(initzer){
            auto re = SDL_Init( SDL_INIT_VIDEO );
            if(re<0)
                return re;
            return 0;
        }

        const char* getError(){
            return SDL_GetError();
        }
    };
}