#include "../os_windows_wrap.h"
#include "test1.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
    
    using namespace os_windows;

    g_hinstance = hInstance;
    g_cmdLine = pCmdLine;
    g_ncmdShow = nCmdShow;

    test1();

	return 0;
}