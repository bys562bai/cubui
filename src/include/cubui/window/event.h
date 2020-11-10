#pragma once
#include <cubui/common/predef.h>

#ifdef CUBUI_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include <cubui/container/list.h>

#ifdef ERROR
#undef ERROR
#endif

namespace cubui
{
    
    namespace eventid {
        enum ID
        {
            UNKNOWN,
            ERROR,
            QUIT,
            DESTROY,
            IDLE
        };
    }

#ifdef CUBUI_PLATFORM_WINDOWS

struct RawEvent{
    //TODO
};

#endif

    using EventID = uint32_t;

    struct Event{
        EventID id;
        RawEvent rawEvent;
    };

    struct EventLoop
    {
        void run();
        void quit(){m_quit = true;}
        void enableIdle(bool x = true) { m_enableIdle = x; }
        virtual void when(Event* e) {
            if (e->id == eventid::QUIT)
                return;
        }

    private:
        bool m_enableIdle = false;
        bool m_quit = false;
    };
    
} // namespace cubui

