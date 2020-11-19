#pragma once
#include <stdint.h>

namespace cubui
{
    template<typename T>
    struct Rect{
        T width;
        T height;
    };

    using Rectu = Rect<unsigned int>;
    using Rect32 = Rect<uint32_t>;
    using Rect64 = Rect<uint64_t>;
    using Rectf = Rect<float>;
    using Rectd = Rect<double>;

} // namespace cubui
