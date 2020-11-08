#pragma once
#include <cubui/config/predef.h>

#ifdef CUBUI_USING_SDL2
#include <SDL2/SDL.h>
#endif

#include "usefultype.h"
#include "event.h"

namespace cubui
{
    struct Window{
        virtual Result init();
        virtual Result when(Event*) {}
        virtual ~Window(){}

        private:
        #ifdef CUBUI_USING_SDL2
        SDL_Window* wnd = nullptr;
        #endif
    };
    
};
