#pragma once
#include <cubui/util/signal.h>
#include "windowbase.h"

namespace cubui{
    struct Window : WindowBase, CUSignalDispatcher{
        virtual CUResult when(CUEvent* e){
            dispatch(1ull<<32, *(CUSignal*) e);

            return WindowBase::when(e);
        }
    };
}