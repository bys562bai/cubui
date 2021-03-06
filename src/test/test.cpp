#include <cubui/cubui.h>
#include <cubui/util/logging_m.h>

CUBUI_MAIN_HEADER{
    using namespace cubui;

//test init
    CubuiConfig config;
    auto n = config.init(hInstance, pCmdLine, nCmdShow);


    if (n) {
        LOG_FATAL("FAIL: ", n->getName());
        exit(-1);
    }
    {
        using namespace global_val;
        for (int i = 0; i < g_argc; i++) {
            LOG_INFO(g_argv[i]);
        }
    }

    LOG_INFO("success");
//test init end

    auto wnd = new WindowBase();
    wnd->init();
    //wnd->show();
    //wnd->uninit();

    EventLoop loop;

    return loop.run();
}