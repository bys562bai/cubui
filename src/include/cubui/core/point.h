#pragma once

namespace cubui
{
    template <typename T>
    struct Point2d
    {
        T x;
        T y;

        T* asArr(){
            return (T*) this;
        }
    };

    template <typename T>
    struct Point3d
    {
        T x;
        T y;
        T z;

        T* asArr(){
            return (T*) this;
        }
    };
    
    
} // namespace cubui
