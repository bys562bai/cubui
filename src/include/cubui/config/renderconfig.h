#pragma once
#include <cubui/common/predef.h>
#include "confignode.h"




#ifdef CUBUI_USING_VULKAN
#include <cubui/impl/vulkan/config.h>
namespace cubui{
    struct RenderConfig : CUVulkanConfig{
    };
}
#endif

