#include "../os_windows_wrap.h"

void test1(){
    using namespace os_windows;
    WindowBase::s_init();
    WindowBase wnd;
    wnd.init();
}