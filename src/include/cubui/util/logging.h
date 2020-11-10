#pragma once
#include <glog/logging.h>
#include <cubui/common/global.h>
#include <cubui/util/confignode.h>

namespace cubui{
    struct LOGConfig:ConfigNode{
        virtual Result init(){
            using namespace global_val;
            gflags::SetCommandLineOption("stderrthreshold", "0");
            google::InitGoogleLogging(g_argv[0]);
            gflags::ParseCommandLineFlags(&g_argc, &g_argv, true);
            return 0;
        }
    };
}