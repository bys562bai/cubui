#pragma once
#include <cubui/container/buffer.h>
#include <cubui/util/smartptr.h>
#include "camera.h"
#include "surface.h"
#include "renderer.h"


namespace cubui
{
    struct Scene{
        virtual void setRenderer(sp<Renderer>);
        virtual void update(double time) {};
        virtual void update(int frame_num) {};
        virtual void setOutput(Surface*);
        virtual void setOutput(Buffer<Surface*>& );
        virtual void render();
    };

    struct Scene2d : Scene{
        virtual void setCamera(Camera2d&);
    };

    struct Scene3d : Scene{
        virtual void setCamera(Camera3d&);
    };

} // namespace cubui
