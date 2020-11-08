#include "init.h"

namespace cubui{
    InitNode::InitNode(){
        global_val::g_init.add(this);
    }
}