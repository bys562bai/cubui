#include <cubui/util/logging.h>
#include <cubui/platform/sdl/core/init.h>
#include "init.h"

namespace cubui{

    InitNode::InitNode(){
        global_val::g_init.add(this);
    }

    Initializer::Initializer(){

    }
}