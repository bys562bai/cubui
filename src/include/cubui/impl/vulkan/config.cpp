#include "config.h"
#include <cubui/util/logging.h>

namespace cubui {

	const std::vector<const char*> VulkanConfig::sm_validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	Result VulkanConfig::init()
	{
		Result re;
		re = createInstance();
		if (re) return re;
		setupDebugMessenger();
		re = pickPhysicalDevice();
		if (re) return re;

		return 0;
	}

	void VulkanConfig::uninit()
	{
		if (m_debugMessenger) {
			destroyDebugUtilsMessengerEXT(nullptr);
			m_debugMessenger = VK_NULL_HANDLE;
		}
		if (m_vkInstance) {
			vkDestroyInstance(m_vkInstance, nullptr);	
			m_vkInstance = VK_NULL_HANDLE;
		}
	}

	VulkanConfig::~VulkanConfig()
	{
		uninit();
	}

	Result VulkanConfig::createInstance()
	{
		auto& validationLayers = sm_validationLayers;

		VkApplicationInfo appInfo{};
		VkInstanceCreateInfo createInfo{};
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};


		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		//init app info
		{
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; 
			appInfo.pNext = nullptr;
			appInfo.pApplicationName = "";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;
			createInfo.pApplicationInfo = &appInfo;	
		}


		//request extensions
		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		//debug setting
		if (m_enableValidationLayers) {
			if (checkValidationLayerSupport()) {
				//init debug create info
				{
					fillDebugMessengerCreateInfo(debugCreateInfo);
					createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
				}

				createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
				createInfo.ppEnabledLayerNames = validationLayers.data();

			}
			else {
				LOG_WARN("Valkan Config: Validation layers are not available!");
				goto GOTO_NOT_ENABLE_VALIDATION_LAYERS;
			}
		}
		else {
			GOTO_NOT_ENABLE_VALIDATION_LAYERS:
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		auto re = vkCreateInstance(&createInfo, nullptr, &m_vkInstance);
		if (re) {
			LOG_ERR("Valkan Config: Can't create vulkan instance!");
		}
		return re;
	}

	Result VulkanConfig::setupDebugMessenger(){
		if (!m_enableValidationLayers) return 0;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		fillDebugMessengerCreateInfo(createInfo);

		auto re = createDebugUtilsMessengerEXT(&createInfo, nullptr);
		if (re)
			LOG_WARN( "Vulkan Config: failed to set up debug messenger!");
		return re;
	}


	void VulkanConfig::fillDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		auto debugCallback = s_debugCallback;
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		createInfo.pfnUserCallback = debugCallback;
	}

	Result VulkanConfig::pickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);

		if (deviceCount == 0) {
			LOG_ERR("Vulkan: Fail to find devices!");
			return -1;
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices.data());

		for (const auto& device : devices) {
			if (isDeviceSuitable(device)) {
				m_physDevice = device;
				break;
			}
		}

		if (m_physDevice == VK_NULL_HANDLE) {
			LOG_ERR("failed to find a suitable GPU!");
			return -2;
		}

		return 0;
	}



	bool VulkanConfig::isDeviceSuitable(VkPhysicalDevice device)
	{
		auto queueFamilies = getQueueFamilies(device);
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				return true;
			}
		}
		return false;
	}

	VkResult VulkanConfig::createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator)
	{
		auto requested_vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_vkInstance, "vkCreateDebugUtilsMessengerEXT");
		if (requested_vkCreateDebugUtilsMessengerEXT != nullptr) {
			return requested_vkCreateDebugUtilsMessengerEXT(m_vkInstance, pCreateInfo, pAllocator, &m_debugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void VulkanConfig::destroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator)
	{
		auto requested_vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_vkInstance, "vkDestroyDebugUtilsMessengerEXT");
		if (requested_vkDestroyDebugUtilsMessengerEXT != nullptr) {
			requested_vkDestroyDebugUtilsMessengerEXT(m_vkInstance, m_debugMessenger, pAllocator);
		}
	}

	std::vector<const char*> VulkanConfig::getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (m_enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool VulkanConfig::checkValidationLayerSupport() {
		auto& validationLayers = sm_validationLayers;

		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	std::vector<VkQueueFamilyProperties> VulkanConfig::getQueueFamilies(VkPhysicalDevice device)
	{
		std::vector<VkQueueFamilyProperties> queueFamilies;
		if (device) {
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
			queueFamilies.resize(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
			return queueFamilies;
		}
		return queueFamilies;
	}


	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanConfig::s_debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		static auto titleText = "Vulkan: ";

		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			LOG_TRACE(titleText, pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			LOG_INFO(titleText, pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			LOG_WARN(titleText, pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		    LOG_ERR(titleText, pCallbackData->pMessage);
			break;
		default:
			LOG_INFO(titleText, pCallbackData->pMessage);
			break;
		}
		return VK_FALSE;
	}
}