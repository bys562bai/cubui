#include <SDL2/SDL.h>
#include <cubui/core/window.h>

namespace cubui{

    Result Window::init(){
        wnd = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_VULKAN);
        if(wnd)
            return 0;
        return -1;
    }

}