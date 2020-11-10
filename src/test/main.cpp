#include <iostream>
#include <cubui/config/config.h>
#include <cubui/core/global.h>
#include <cubui/util/logging.h>
#include <cubui/core/window.h>
#include <cubui/core/event.h>

int main(int argc, char* argv[]){
    using namespace cubui;
    CubuiConfig config;
    auto n = config.init(argc, argv);

    LOG(INFO)<<"hello";

    return 0;
}