#pragma once
#include <memory>

namespace cubui
{
    template<typename T>
    using SPtr = std::shared_ptr<T>;

    template<typename T>
    using UPtr = std::unique_ptr<T>;


} // namespace cubui
