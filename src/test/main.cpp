#include <iostream>
#include <cubui/cubui.h>
#include <cubui/util/logging.h>

CUBUI_MAIN_HEADER{
    using namespace cubui;

//test init
    CubuiConfig config;
    auto n = config.init(hInstance, pCmdLine, nCmdShow);


    if (n) {
        LOG(FATAL) << "FAIL: " << n->getName();
    }
    {
        using namespace global_val;
        for (int i = 0; i < g_argc; i++) {
            LOG(INFO) << g_argv[i];
        }
    }

    LOG(INFO)<<"success";
//test init end

    auto wnd = new Window();
    wnd->init();
    //wnd->show();
    //wnd->uninit();

    EventLoop loop;

    return loop.run();
}