#pragma once
#include <stdint.h>

namespace cubui{
    template<typename T>
    struct ArrayRef{
        T* data = nullptr;
        size_t size = 0;
    };
}