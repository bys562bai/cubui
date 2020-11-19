#pragma once
#include <cubui/common/predef.h>
#include <cubui/common/usefultype.h>
#include <cubui/util/signal.h>

#ifdef CUBUI_PLATFORM_WINDOWS
#include <cubui/ext/windowsinc.h>
#endif


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

    using RawEvent = MSG;
#endif


    struct CUEvent : CUSignal{
        RawEvent rawEvent;
    };

    struct EventLoop
    {
        int run();
        void dispatch(CUEvent* e);
        void quit(){m_quit = true;}
        void enableIdle(bool x = true) { m_enableIdle = x; }
        virtual void when(CUEvent* e);

    private:
        bool m_enableIdle = false;
        bool m_quit = false;
    };
    
} // namespace cubui

