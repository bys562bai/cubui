#pragma once
#include <cubui/common/predef.h>

#ifdef CUBUI_PLATFORM_WINDOWS
#include <Windows.h>
#endif // CUBUI_PLATFORM_WINDOWS


namespace cubui{
    namespace global_val{
#ifdef CUBUI_PLATFORM_WINDOWS
        extern HINSTANCE g_hinstance;
        extern PWSTR g_pCmdLine;
        extern int g_nCmdShow;
#endif
        extern int g_argc;
        extern char** g_argv;
    }
}