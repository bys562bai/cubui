#include <iostream>
#include <cubui/core/global.h>
#include <cubui/util/logging.h>


int main(int argc, char* argv[]){
    using namespace cubui;
    global_val::g_init.init(argc, argv);
    LOG(INFO)<<"hello";

    return 0;
}