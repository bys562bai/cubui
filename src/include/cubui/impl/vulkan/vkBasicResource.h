#pragma once
#include <cubui/ext/vulkaninc.h>
#include <cubui/container/buffer.h>
#include <cubui/container/hashmap.h>

#include <cubui/basic/windowbase.h>

#include "vkfuncext.h"
#include "vkwrapper.h"
#include "vkhelpler.h"

namespace cubui{
    
	struct CUVkBasicResource {

		VkResult init(bool enableDebug = true);

		void uninit();

		VkInstance getInstance() const {
			return m_vkInstance;
		}

		const VkPhysicalDeviceProperties& getPhysicalDeviceProperty(VkPhysicalDevice device) const{
			return m_vkPhysicalDeviceProperties.at(device);
		}

		CUBuffer<uint32_t> filterQueueFamilyIdxs(VkPhysicalDevice device, VkQueueFlags flags) {
			
			auto& familyProps = getQueueFamilyProperties(device);
			return vkhelpler::cuFilterQueueFamilyIdices(familyProps, VK_QUEUE_GRAPHICS_BIT);
		}


		const CUBuffer<VkQueueFamilyProperties>& getQueueFamilyProperties(VkPhysicalDevice device) const{
			return m_vkQueueFamilyPropertiesMap.at(device);
		}

		const CUBuffer<VkPhysicalDevice>& getPhysicalDevices() const {
			return m_vkPhysicalDevices;
		}

		const CUBuffer<const char*>& getValidationLayers() const {
			return m_validationLayers;
		}

		~CUVkBasicResource() { 
			uninit();
		}


	private:
		//return true if fail
		bool initVkFuncExt();

		VkResult createVkDebugMessenger(const VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void destroyVkDebugMessenger();

		VkInstance m_vkInstance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT m_vkDebugMessenger = VK_NULL_HANDLE;
		CUBuffer<VkPhysicalDevice> m_vkPhysicalDevices;
		CUHashMap<VkPhysicalDevice, CUBuffer<VkQueueFamilyProperties> > m_vkQueueFamilyPropertiesMap;
		CUHashMap<VkPhysicalDevice, VkPhysicalDeviceProperties > m_vkPhysicalDeviceProperties;

		mutable CUVkFuncExt m_vkExt;

		CUBuffer<const char*> m_validationLayers;

		inline static const char* s_validationLayers[] = {
			"VK_LAYER_KHRONOS_validation"
		};




	public:
#ifdef CUBUI_PLATFORM_WINDOWS
		VkSurfaceKHR createSurfaceKHR(WindowBase& wnd) const;
		void destroySurfaceKHR(VkSurfaceKHR surface) const;
#endif

	};
}