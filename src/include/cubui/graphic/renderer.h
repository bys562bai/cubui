#pragma once
#include <cubui/common/predef.h>

#ifdef CUBUI_USING_VULKAN
#include <cubui/ext/vulkaninc.h>
#endif

namespace cubui
{
    struct Renderer
    {

    private:
        VkDevice m_device = VK_NULL_HANDLE;
    };  
} // namespace cubui
