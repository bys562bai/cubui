#pragma once
#include <cubui/common/predef.h>

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif 



#ifdef CUBUI_PLATFORM_WINDOWS
#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#endif


#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>