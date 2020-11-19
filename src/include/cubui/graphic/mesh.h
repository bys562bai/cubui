#pragma once
#include <cubui/container/buffer.h>
#include "point.h"


namespace cubui
{
    template<typename T, typename U>
    struct Mesh2d
    {
        CUBuffer<Point2d<T> > vertices;
        CUBuffer<U> indices;
    };
    
    template<typename T, typename U>
    struct Mesh3d
    {
        CUBuffer<Point3d<T> > vertices;
        CUBuffer<U> indices;
    };
} // namespace cubui
