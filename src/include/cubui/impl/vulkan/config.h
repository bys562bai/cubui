#pragma once
#include <vector>

#include "vulkanheader.h"

#include <cubui/config/confignode.h>
#include <cubui/common/usefultype.h>

namespace cubui{
	namespace global_val{
		extern VkInstance g_vkInstance;
	}

	struct VulkanConfig : ConfigNode {
		virtual Result init();
		virtual void uninit();

		virtual const char* getName() const {
			static auto name = "Vulkan";
			return name;
		}

		virtual ~VulkanConfig();

	private:
		virtual Result createInstance();
		virtual Result setupDebugMessenger();
		virtual Result pickPhysicalDevice();
		virtual Result createLogicalDevice();

		virtual bool checkValidationLayerSupport(); //success return 1
		virtual std::vector<const char*> getRequiredExtensions();

		VkResult createDebugUtilsMessengerEXT(
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
			const VkAllocationCallbacks* pAllocator);

		void destroyDebugUtilsMessengerEXT(
			const VkAllocationCallbacks* pAllocator);

		void fillDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);


		static bool isDeviceSuitable(VkPhysicalDevice device);

		//return -1 if not found
		static int getQueueFamilyIndex(VkPhysicalDevice device, VkQueueFlags flags);

		static std::vector<VkQueueFamilyProperties> getQueueFamilies(VkPhysicalDevice device);

		static VKAPI_ATTR VkBool32 VKAPI_CALL s_debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData
		);

	private:
		VkPhysicalDevice m_physDevice = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
		VkDevice m_device = VK_NULL_HANDLE;
		VkQueue m_graphicQueue = VK_NULL_HANDLE;
		bool m_enableValidationLayers = true;

		static const std::vector<const char*> s_validationLayers;
	};
}