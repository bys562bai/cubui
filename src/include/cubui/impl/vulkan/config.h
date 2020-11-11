#pragma once
#include <vector>

#include "headeruse.h"

#include <cubui/util/confignode.h>
#include <cubui/common/usefultype.h>


namespace cubui{
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

		virtual bool checkValidationLayerSupport(); //success return 1
		virtual std::vector<const char*> getRequiredExtensions();

		VkResult createDebugUtilsMessengerEXT(
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
			const VkAllocationCallbacks* pAllocator);

		void destroyDebugUtilsMessengerEXT(
			const VkAllocationCallbacks* pAllocator);

		void fillDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		Result pickPhysicalDevice();


		static bool isDeviceSuitable(VkPhysicalDevice device);


		static std::vector<VkQueueFamilyProperties> getQueueFamilies(VkPhysicalDevice device);

		static VKAPI_ATTR VkBool32 VKAPI_CALL s_debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData
		);




	private:
		VkInstance m_vkInstance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
		VkPhysicalDevice m_physDevice = VK_NULL_HANDLE;
		bool m_enableValidationLayers = true;

		static const std::vector<const char*> sm_validationLayers;
	};
}