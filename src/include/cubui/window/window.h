#pragma once
#include <cubui/common/predef.h>

#include <stdint.h>

#ifdef CUBUI_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include <cubui/common/usefultype.h>

namespace cubui
{
    struct Event;

    //must be created by new
    struct Window{
        virtual Result init();

        //It should be deleted when destroy message received.
        virtual Result when(Event* e);

        
        void show(bool x = true);

    
        void destroy();

        
        //Do not destroy it in this function because window is controled by user.
        virtual ~Window() { };  

        #ifdef CUBUI_PLATFORM_WINDOWS
        HWND getHwnd() const;
        HINSTANCE getHinstance() const;
        #endif //CUBUI_PLATFORM_WINDOWS
    };
    
};
