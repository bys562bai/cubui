#include <algorithm>
#include <cubui/util/logging_m.h>

#include "vkhelpler.h"

namespace cubui{
    namespace vkhelpler{
        
		CUBuffer<uint32_t> cuFilterQueueFamilyIdices(const CUBuffer<VkQueueFamilyProperties>& properties, VkQueueFlags flags)
		{
			CUBuffer<uint32_t> idxs;
			idxs.reserve(properties.size());

			auto size = properties.size();
			for (int i = 0; i < size; i++) {
				auto& prop = properties[i];
				if (flags == (flags & prop.queueFlags ))
					idxs.push_back(i);
			}
			return idxs;
		}


		CUBuffer<VkSurfaceFormatKHR> cuGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice device, VkSurfaceKHR surface)
		{
			uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
			CUBuffer<VkSurfaceFormatKHR> formats(formatCount);
			if (formatCount) {
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats.data());
			}

			return formats;
		}


		CUBuffer<VkPresentModeKHR> cuGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice device, VkSurfaceKHR surface)
		{
			uint32_t presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
			CUBuffer<VkPresentModeKHR> modes(presentModeCount);
			if (presentModeCount) {
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, modes.data());
			}
			return modes;
		}

		CUBuffer<VkExtensionProperties> cuGetDeviceExtensionProperties(VkPhysicalDevice physDevice)
		{
			uint32_t count = 0;
			assert(physDevice);
			vkEnumerateDeviceExtensionProperties(physDevice, nullptr, &count, nullptr);
			CUBuffer<VkExtensionProperties>props(count);
			vkEnumerateDeviceExtensionProperties(physDevice, nullptr, &count, props.data());
			return props;
		}

		CUBuffer<VkExtensionProperties> cuGetDeviceExtensionProperties(VkPhysicalDevice physDevice, const char* layerName)
		{
			assert(physDevice);
			uint32_t count = 1;
			CUBuffer<VkExtensionProperties>props(count);
			auto re = vkEnumerateDeviceExtensionProperties(physDevice, layerName, &count, props.data());
			if (re) {
				props.clear();
				LOG_WARN("vulkan: fail to get device extension properties");
			}
			return props;
		}

		void cuFilterDeviceExtensionSupport(VkPhysicalDevice physDevice, CUBuffer<const char*>& extensionNames)
		{
			extensionNames.erase_if(
				[&](const char* name) {
					return cuGetDeviceExtensionProperties(physDevice, name).size();
				}
			);
		}


		CUBuffer<VkExtensionProperties> cuGetInstanceExtensionProperties()
		{
			uint32_t size = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &size, 0);
			CUBuffer<VkExtensionProperties> props(size);
			auto re = vkEnumerateInstanceExtensionProperties(nullptr, &size, props.data());
			if (re) {
				LOG_WARN("vulkan: fail to get instance extension properties");
				return {};
			}
			return props;
		}

		CUBuffer<VkExtensionProperties> cuGetInstanceExtensionProperties(const char* layerName)
		{
			uint32_t size = 1;
			CUBuffer<VkExtensionProperties> props(size);
			auto re = vkEnumerateInstanceExtensionProperties(layerName, &size, props.data());
			if (re) {
				props.clear();
				LOG_WARN("vulkan: fail to get instance extension properties");
			}
			return props;
		}

		CUBuffer<VkPhysicalDevice> cuGetPhysicalDevices(VkInstance instance) {
			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

			CUBuffer<VkPhysicalDevice> devices(deviceCount);
			if(deviceCount)
				vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
			return devices;
		}

		CUBuffer<VkQueueFamilyProperties> cuGetQueueFamilies(VkPhysicalDevice device)
		{
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
			CUBuffer<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
			return queueFamilies;
		}

		CUBuffer<VkLayerProperties> cuGetInstanceLayerProperties()
		{
			uint32_t count;
			vkEnumerateInstanceLayerProperties(&count, nullptr);
			auto layerProperties = CUBuffer<VkLayerProperties>(count);
			vkEnumerateInstanceLayerProperties(&count, layerProperties.data());
			return layerProperties;
		}

		void cuFilterInstanceLayerSupport(CUBuffer<const char*>& layerNames)
		{
			auto layerProperties = cuGetInstanceLayerProperties();
			layerNames.erase(std::remove_if(layerNames.begin(), layerNames.end(),
				[&](const char* name) {
					return std::none_of(layerProperties.begin(), layerProperties.end(),
						[&](VkLayerProperties property) {
							return strcmp(name, property.layerName) == 0; 
						}); 
				}
			),layerNames.end()
			);
		}

		CUBuffer<CUString> cuGetRequiredInstanceExtensions( bool enable_debug)
		{
			uint32_t extensionCount = 0;
			CUBuffer<CUString> extensions;

			if (glfwVulkanSupported()) {
				uint32_t glfwExtensionCount = 0;
				const char** glfwExtensions;
				glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
				extensionCount = glfwExtensionCount;
				extensions.reserve(extensionCount + 1);
				for (int i = 0; i < extensionCount; i++) {
					extensions.push_back(glfwExtensions[i]);
				}

				if (enable_debug) {
					extensionCount += 1;
					extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
				}
			}
			else {
				assert(false);//Not completing!!!
				auto props = cuGetInstanceExtensionProperties();
				extensionCount = (uint32_t) props.size();
				extensions.reserve(extensionCount + 1);
				for (int i = 0; i < extensionCount; i++) {
					extensions.push_back(props[i].extensionName);
				}

				if (!enable_debug) {
					auto it = extensions.begin();
					auto end = extensions.end();
					while (it != end) {
						if (*it == VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
							extensions.erase(it);
						it++;
					}
				}
			}

			return extensions;
		}

		void cuFillDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info, VKAPI_ATTR VkBool32(*debugCallBack)(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData))
		{
			info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

			info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

			info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

			info.pfnUserCallback = debugCallBack;
		}



		VKAPI_ATTR VkBool32 VKAPI_CALL cuDebugMessageCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData)
		{
			static auto fmtText = "Vulkan: {}";

			switch (messageSeverity)
			{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				LOG_TRACE(fmtText, pCallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				LOG_INFO(fmtText, pCallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				LOG_WARN(fmtText, pCallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				LOG_ERR(fmtText, pCallbackData->pMessage);
				break;
			default:
				LOG_INFO(fmtText, pCallbackData->pMessage);
				break;
			}
			return VK_FALSE;
		}
    }
}