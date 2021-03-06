#pragma once
#include <cubui/util/smartptr.h>
#include "camera.h"
#include "renderer.h"


namespace cubui
{
    struct SceneNode{
        virtual SceneNode& next();
    };

    struct Scene{
        virtual SceneNode& getRoot();
        virtual void setRenderer(SPtr<Renderer>);
        virtual void update(double time) {};
        virtual void update(int frame_num) {};
        virtual void render();
    };

    struct Scene2d : Scene{
        virtual void setCamera(Camera2d&);
    };

    struct Scene3d : Scene{
        virtual void setCamera(Camera3d&);
    };

} // namespace cubui
