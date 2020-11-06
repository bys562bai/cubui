#pragma once

namespace cubui{

    template<typename T, int size>
    struct Vector
    {
        
    };
    
    template<typename T>
    using Vector2d = Vector<T,2>;

    template<typename T>
    using Vector3d = Vector<T,3>;
}