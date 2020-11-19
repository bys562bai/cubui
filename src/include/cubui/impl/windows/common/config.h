#pragma once
#include <cubui/ext/windowsinc.h>
#include <cubui/config/confignode.h>
#include <cubui/basic/windowbase.h>

namespace cubui{
    struct WindowsPlatformConfig : ConfigNode{
        virtual const char* getName() const {
            static auto name = "WindowsPlatform";
            return name;
        }

        virtual CUResult init() {
            return WindowBase::s_init();
        }
    };
}