#pragma once
#include <cubui/ext/vulkaninc.h>
#include <cubui/container/buffer.h>
#include <cubui/container/string.h>

namespace cubui{
       namespace vkhelpler{
		inline CUBuffer<const char*> extractCharPtrs(const CUBuffer<CUString>& strs) {
			auto size = strs.size();
			CUBuffer<const char*> buf(size);
			for (int i = 0; i < size; i++) {
				buf[i] = strs[i].data();
			}
			return buf;
		}
        
		inline void cuFillEmptyLayers(VkInstanceCreateInfo& info) {
			info.enabledLayerCount = 0;
			info.ppEnabledLayerNames = nullptr;
		}
        
		inline void cuFillLayers(VkInstanceCreateInfo& info, const CUBuffer<const char*>& layers) {
			auto size = (uint32_t) layers.size();
			info.enabledLayerCount = size;
			if (size)
				info.ppEnabledLayerNames = layers.data();
			else
				info.ppEnabledLayerNames = nullptr;
		}

		inline void cuFillLayers(VkDeviceCreateInfo& info, const CUBuffer<const char*>& layers) {
			auto size = (uint32_t) layers.size();
			info.enabledLayerCount = size;
			if (size)
				info.ppEnabledLayerNames = layers.data();
			else
				info.ppEnabledLayerNames = nullptr;
		}


		inline void cuFillExtensions(VkInstanceCreateInfo& info, const CUBuffer<const char*>& extensions) {
			auto size = (uint32_t) extensions.size();
			info.enabledExtensionCount = size;
			if (size)
				info.ppEnabledExtensionNames = extensions.data();
			else
				info.ppEnabledExtensionNames = nullptr;
		};

		CUBuffer<VkSurfaceFormatKHR> cuGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice device, VkSurfaceKHR surface);
		CUBuffer<VkPresentModeKHR> cuGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice device, VkSurfaceKHR surface);

		CUBuffer<VkExtensionProperties> cuGetDeviceExtensionProperties(VkPhysicalDevice physDevice);
		CUBuffer<VkExtensionProperties> cuGetDeviceExtensionProperties(VkPhysicalDevice physDevice, const char* layerName);
		void cuFilterDeviceExtensionSupport(VkPhysicalDevice physDevice, CUBuffer<const char*>& extensionNames);

		CUBuffer<VkExtensionProperties> cuGetInstanceExtensionProperties();
		CUBuffer<VkExtensionProperties> cuGetInstanceExtensionProperties(const char* layerName);

        CUBuffer<VkPhysicalDevice> cuGetPhysicalDevices(VkInstance instance);
		CUBuffer<VkQueueFamilyProperties> cuGetQueueFamilies(VkPhysicalDevice device);
		CUBuffer<uint32_t> cuFilterQueueFamilyIdices(const CUBuffer<VkQueueFamilyProperties>& properties,VkQueueFlags flags);

		CUBuffer<VkLayerProperties> cuGetInstanceLayerProperties();
		void cuFilterInstanceLayerSupport(CUBuffer<const char *>& layerNames);

        CUBuffer<CUString> cuGetRequiredInstanceExtensions( bool enable_debug  = true);

        VKAPI_ATTR VkBool32 VKAPI_CALL cuDebugMessageCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		void cuFillDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info,
			VKAPI_ATTR VkBool32 (* debugCallBack)(
				VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData) = cuDebugMessageCallback
		);


    }//end namespace vkhelpler
}