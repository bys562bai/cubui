#pragma once
#include <functional>
#include <cubui/container/map.h>
#include <cubui/container/list.h>

namespace cubui
{
    struct Event{

    };

    struct EventLoop
    {
        void start();
        void enableIdle(bool );
        virtual void when(Event* event) {}
    };
    
} // namespace cubui

