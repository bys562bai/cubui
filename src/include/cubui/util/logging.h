#pragma once
#include <glog/logging.h>
#include <cubui/core/initnode.h>
#include <cubui/core/global.h>


namespace cubui{
    struct LOGInitNode:InitNode{
        virtual Result init(){
            using namespace global_val;
            gflags::SetCommandLineOption("stderrthreshold", "0");
            google::InitGoogleLogging(g_argv[0]);
            gflags::ParseCommandLineFlags(&g_argc, &g_argv, true);
            return 0;
        }
    };
}