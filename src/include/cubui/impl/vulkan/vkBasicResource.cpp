#include <algorithm>
#include <cubui/util/logging_m.h>

#include <cubui/common/predef.h>
#include <cubui/common/global.h>

#include "vkbasicResource.h"
#include "vkhelpler.h"

namespace cubui{
    
	VkResult CUVkBasicResource::init(bool enableDebug) {
		using namespace vkhelpler;

		if (m_vkDebugMessenger) {
			return VK_SUCCESS;
		}
		
		VkResult re;
		VkApplicationInfo appInfo{};
		VkInstanceCreateInfo createInfo{};
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

		if (!m_vkInstance) {

			m_vkExt.clear();

			m_validationLayers.insert(0, s_validationLayers, s_validationLayers + (sizeof(s_validationLayers) / sizeof(char*)));
			auto& validationLayers = m_validationLayers;




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
			auto extensions_string = cuGetRequiredInstanceExtensions();
			auto extensions = extractCharPtrs(extensions_string);
			cuFillExtensions(createInfo, extensions);


			cuFillEmptyLayers(createInfo);

			//validation layers setting
			if (enableDebug) {
				cuFilterInstanceLayerSupport(validationLayers);
				if (validationLayers.empty()) {
					LOG_WARN("Valkan Config: Validation layers are not available!");
				}
				else {
					cuFillDebugMessengerCreateInfo(debugCreateInfo);
					createInfo.pNext = &debugCreateInfo;
					cuFillLayers(createInfo, validationLayers);
				}
			}

			//create vulkan instance
			re = vkCreateInstance(&createInfo, nullptr, &m_vkInstance);
			if (re) {
				LOG_ERR("Vulkan: Can't create vulkan instance!");
				return re;
			}
		}

		//set vkExtFuncs instance
		if (initVkFuncExt()) {
			LOG_WARN("Vulkan: some functions of extensions failed to get!");
		}

		//create debug messenger
		if (enableDebug)
			re = createVkDebugMessenger(debugCreateInfo);

		//init other members
		m_vkPhysicalDevices = cuGetPhysicalDevices(m_vkInstance);
		for (auto device : m_vkPhysicalDevices) {
			auto& val = m_vkPhysicalDeviceProperties[device];
			vkGetPhysicalDeviceProperties(device, &val);
			m_vkQueueFamilyPropertiesMap[device] = cuGetQueueFamilies(device);
		}

		return VK_SUCCESS;
	}

	void CUVkBasicResource::uninit()
	{
		m_validationLayers.clear();
		m_vkQueueFamilyPropertiesMap.clear();
		m_vkPhysicalDeviceProperties.clear();
		m_vkPhysicalDevices.clear();

		if (m_vkDebugMessenger) {
			destroyVkDebugMessenger();
			m_vkDebugMessenger = VK_NULL_HANDLE;
		}

		if (m_vkInstance) {
			vkDestroyInstance(m_vkInstance, nullptr);
			m_vkInstance = VK_NULL_HANDLE;
		}
	}


	bool CUVkBasicResource::initVkFuncExt()
	{
		bool failed = false;
		m_vkExt.vkInstance = m_vkInstance;
		failed|=m_vkExt.requestDebugUtilsMessengerEXT();
		return failed;
	}

	VkResult CUVkBasicResource::createVkDebugMessenger(const VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		auto vkCreateDebugUtilsMessengerEXT = m_vkExt.vkCreateDebugUtilsMessengerEXT;
		if (!vkCreateDebugUtilsMessengerEXT) {
			LOG_WARN("Vulkan: Can't create debug messenger!");
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}

		auto re = vkCreateDebugUtilsMessengerEXT(m_vkInstance, &createInfo, nullptr, &m_vkDebugMessenger);
		if (re) {
			LOG_WARN("Vulkan: Can't create debug messenger!");
		}
		return re;
	}

	void CUVkBasicResource::destroyVkDebugMessenger()
	{
		auto vkDestroyDebugUtilsMessengerEXT = m_vkExt.vkDestroyDebugUtilsMessengerEXT;
		vkDestroyDebugUtilsMessengerEXT(m_vkInstance, m_vkDebugMessenger, nullptr);
	}




#ifdef CUBUI_PLATFORM_WINDOWS
	VkSurfaceKHR CUVkBasicResource::createSurfaceKHR(WindowBase& wnd) const
	{
		VkResult re;
		VkSurfaceKHR surface = VK_NULL_HANDLE;
		VkWin32SurfaceCreateInfoKHR info;
		info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		info.pNext = nullptr;
		info.flags = 0;
		info.hinstance = global_val::g_hinstance;
		info.hwnd = wnd.getHwnd();
		//vkCreateWin32SurfaceKHR(m_vkInstance, &info, nullptr, &surface);
		if (!m_vkExt.request(vkCreateWin32SurfaceKHR)) {
			LOG_WARN("Vulkan: Fail to create surface");
			return VK_NULL_HANDLE;

		}

		re = m_vkExt.vkCreateWin32SurfaceKHR(m_vkInstance, &info, nullptr, &surface);
		if (re) {
			LOG_WARN("Vulkan: Fail to create surface");
			return VK_NULL_HANDLE;
		}

		return surface;
	}

	void CUVkBasicResource::destroySurfaceKHR(VkSurfaceKHR surface) const
	{
		vkDestroySurfaceKHR(m_vkInstance, surface, nullptr);
	}
#endif


}