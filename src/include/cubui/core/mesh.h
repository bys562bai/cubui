#pragma once
#include <cubui/container/buffer.h>
#include "point.h"


namespace cubui
{
    template<typename T, typename U>
    struct Mesh2d
    {
        Buffer<Point2d<T> > vertices;
        Buffer<U> indices;
    };
    
    template<typename T, typename U>
    struct Mesh3d
    {
        Buffer<Point3d<T> > vertices;
        Buffer<U> indices;
    };
} // namespace cubui
