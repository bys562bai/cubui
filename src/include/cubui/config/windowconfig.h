#pragma once
#include <cubui/common/predef.h>
#include "confignode.h"

#ifdef CUBUI_PLATFORM_WINDOWS
#include <cubui/impl/windows/common/config.h>
#endif

namespace cubui{
    struct WindowConfig : WindowsPlatformConfig{};
}