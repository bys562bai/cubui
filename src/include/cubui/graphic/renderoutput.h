#pragma once
#include <cubui/common/predef.h>

#ifdef CUBUI_USING_VULKAN
#include <cubui/ext/vulkaninc.h>
#endif

namespace cubui
{
    struct RenderOutput{


#ifdef CUBUI_USING_VULKAN
        private:
        VkImage m_vkImage = VK_NULL_HANDLE;
        VkImageView colorImageView = VK_NULL_HANDLE;
        VkDeviceMemory m_vkDeviceMemory = VK_NULL_HANDLE;
#endif
    };
} // namespace cubui
