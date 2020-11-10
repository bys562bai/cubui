#pragma once
#include <Windows.h>
#include <cubui/util/confignode.h>
#include <cubui/basic/window.h>

namespace cubui{
    struct WindowsPlatformConfig : ConfigNode{
        virtual const char* getName() const {
            static auto name = "WindowsPlatform";
            return name;
        }

        virtual Result init() {
            return Window::s_init();
        }
    };
}