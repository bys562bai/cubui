#pragma once
#include <cubui/graphic/scene.h>
#include <cubui/container/list.h>

namespace cubui
{
    struct SimpleSceneNode
    {

    };

    struct SimpleScene : Scene2d
    {
        virtual const IntrusiveList<SimpleSceneNode>& getNodes(); 
    };
    
} // namespace cubui
