#include <cubui/cubui.h>
#include <cubui/util/logging_m.h>
#include <cubui/ext/vulkaninc.h>


using namespace cubui;
struct VulkanWindow : Window{

};

CUBUI_MAIN_HEADER{
    CubuiConfig config;
    auto n = config.init(hInstance, pCmdLine, nCmdShow);

    auto wnd = new VulkanWindow();
    wnd->init();

    EventLoop loop;

    LOG_INFO("success");

    return loop.run();
}