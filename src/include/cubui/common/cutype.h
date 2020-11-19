#pragma once
#include <stdint.h>

namespace cubui{
    using CUTypeID = uint64_t;

    struct CUType{
        virtual ~CUType(){}
        virtual CUTypeID getID() const noexcept {return 0;}
    };
}