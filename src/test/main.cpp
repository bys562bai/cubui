#include <iostream>
#include <fmt/format.h>
#include <glog/logging.h>
#include <cubui/container/list.h>
#include <cubui/core/init.h>

namespace cubui{
    namespace global_val{
        int g_argc;
        char** g_argv;
        Initializer g_init;
    }

    InitNode::InitNode(){
        global_val::g_init.add(this);
    }
}


using namespace cubui;

struct GLOGInitNode:InitNode{
    // GLOGInitNode(){
    //     global_val::g_init.add(this);
    // }

    virtual Result init(Initializer& initzer){
        using namespace global_val;
        gflags::SetCommandLineOption("stderrthreshold", "0");
        google::InitGoogleLogging(g_argv[0]);
        gflags::ParseCommandLineFlags(&g_argc, &g_argv, true);
        return 0;
    }
};

GLOGInitNode node;


int main(int argc, char* argv[]){
    system("pause");
    using namespace global_val;
    g_init.init(argc, argv);

    LOG(INFO)<<"SUCCESS";

    system("pause");
    return 0;
}