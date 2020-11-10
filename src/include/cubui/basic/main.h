#pragma once
#include <cubui/common/predef.h>

#ifdef CUBUI_PLATFORM_WINDOWS

#define CUBUI_MAIN_HEADER  int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#else

#define CUBUI_MAIN_HEADER int main(int argc, char* argv[])

#endif


/*
example:
#include <cubui/common/config.h>

CUBUI_MAIN_HEADER{
    CubuiConfig config;
    auto n = config.init(hInstance, pCmdLine, nCmdShow);

    return 0;
}

*/