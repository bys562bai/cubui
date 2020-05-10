#pragma once
#include <cubui/common/typedef.h>

namespace cubui{

struct MsgHandler;

struct Message{
    MsgID id = 0;
    MsgHandler* prnt = nullptr;
    MsgHandler* sender = nullptr;
};

}
