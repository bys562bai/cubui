#include <assert.h>
#include <algorithm>
#include <optional>
#include <cubui/cubui.h>
#include <cubui/util/logging_m.h>
#include <cubui/impl/vulkan/cuvulkan.h>


namespace cubui {
	CUVkBasicResource g_resource;

	struct CUVkDevice {
		VkResult init(VkPhysicalDevice device, const CUBuffer<VkDeviceQueueCreateInfo>& queueInfos) {
			uninit();
			return _init(device, queueInfos, s_renderWindowExtensions);
		}

		//default init, has graphic ability
		VkResult init() {
			using namespace vkhelpler;
			if (m_vkDevice)
				return VK_SUCCESS;

			uninit();

			auto& physDevices = g_resource.getPhysicalDevices();
			auto physDevice = m_physDevice = physDevices[0];
			auto graphicIdxs = g_resource.filterQueueFamilyIdxs(physDevice, VK_QUEUE_GRAPHICS_BIT);
			auto idx = graphicIdxs[0];

			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = idx;
			queueCreateInfo.queueCount = 1;
			float queuePriority = 1.0f;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			CUBuffer<VkDeviceQueueCreateInfo> infos{ queueCreateInfo };
			return _init(physDevice, infos, s_renderWindowExtensions);
		}

		CUResult init(VkSurfaceKHR surface) {
			auto& physDevices = g_resource.getPhysicalDevices();
			
			//find supported physical device
			{
				auto size = physDevices.size();
				for (int i = 0; i < size; i++) {
					m_physDevice = physDevices[i];
					if (getUnsupportExtensions(s_renderWindowExtensions).empty()) {
						break;
					}
					else if (i == size - 1) {
						LOG_ERR("fail to find suitable queue family!");
						m_physDevice = VK_NULL_HANDLE;
						return -1;
					}
				}
			}
			auto physDevice = m_physDevice;
			//find supported graphic family
			uint32_t graphicFamilyIdx;
			uint32_t presentFamilyIdx;
			{
				auto graphicIdxs = g_resource.filterQueueFamilyIdxs(physDevice, VK_QUEUE_GRAPHICS_BIT);

				auto size = graphicIdxs.size();

				bool fail = !size;
				for (int i = 0; i < size; i++) {
					graphicFamilyIdx = graphicIdxs[i];
					if (ifSupportPresent(surface, graphicFamilyIdx)) {
						presentFamilyIdx = graphicFamilyIdx;
						break;
					}
					else if (i == size - 1) {
						//find from other queue family
						auto size = g_resource.getQueueFamilyProperties(physDevice).size();
						for (int i = 0; i < size; i++) {
							if (ifSupportPresent(surface, i)) {
								presentFamilyIdx = i;
							}
							else if (i == size - 1) {
								fail = true;
							}
						}
					}
				}
				if (fail) {
					LOG_ERR("fail to find suitable graphic family!");
					m_physDevice = VK_NULL_HANDLE;
					return -2;
				}

				m_presentFamilyIdx = presentFamilyIdx;
				m_graphicFamilyIdx = graphicFamilyIdx;
			}

			

			CUBuffer<uint32_t> familyIdxs;

			if (graphicFamilyIdx == presentFamilyIdx) {
				familyIdxs.push_back(graphicFamilyIdx);
			}
			else {
				familyIdxs.insert(familyIdxs.end(), { graphicFamilyIdx, presentFamilyIdx });
			}

			auto size = familyIdxs.size();
			CUBuffer<VkDeviceQueueCreateInfo> infos(size);
			float priority = 1.;
			for (int i = 0; i < size; i++) {
				auto& info = infos[i];
				info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				info.pNext = nullptr;
				info.flags = 0;
				info.queueCount = 1;
				info.queueFamilyIndex = familyIdxs[i];
				info.pQueuePriorities = &priority;
			}

			return (CUResult)_init(m_physDevice, infos, s_renderWindowExtensions);
		}

		void uninit() {
			m_queueInfos.clear();
			m_physDevice = VK_NULL_HANDLE;
			if(m_vkDevice)
				vkDestroyDevice(m_vkDevice, nullptr);
			m_vkDevice = VK_NULL_HANDLE;
		}

		virtual ~CUVkDevice() {
			uninit();
		}

		uint32_t getQueueNum(uint32_t idx) const {
			const auto& it = m_queueInfos.find(idx);
			if (it != m_queueInfos.end()) {
				const auto& info = it->second;
				return (uint32_t) info.priorites.size();
			}
			return 0;
		}

		VkQueue getQueue(uint32_t familyidx, uint32_t queueidx) const {
			VkQueue queue = VK_NULL_HANDLE;
			vkGetDeviceQueue(m_vkDevice, familyidx, queueidx, &queue);
			return queue;
		}

		VkDevice getDevice() const {
			return m_vkDevice;
		}

		VkPhysicalDevice getPhysDevice() const {
			return m_physDevice;
		}

		std::optional<uint32_t> getGraphicFamilyIdx() const {
			return m_graphicFamilyIdx;
		}

		std::optional<uint32_t> getPresentFamilyIdx() const {
			return m_presentFamilyIdx;
		}

		bool ifSupport(VkSurfaceKHR surface) {
			return m_presentFamilyIdx.has_value() && m_graphicFamilyIdx.has_value();
		}

		bool ifSupportPresent(VkSurfaceKHR surface, uint32_t familyidx) {
			VkBool32 presentSupport = 0;
			auto re = vkGetPhysicalDeviceSurfaceSupportKHR(m_physDevice, familyidx, surface, &presentSupport);
			return presentSupport;
		}

		CUBuffer<uint32_t> getQueueIdxs() const {
			CUBuffer<uint32_t> idxs;
			idxs.reserve(m_queueInfos.size());
			for (auto& it : m_queueInfos) {
				idxs.push_back(it.first);
			}
			return idxs;
		}

		CUBuffer<const char*> getUnsupportExtensions(CUBuffer<const char*> layerNames) {
			CUBuffer<const char*> unsupport;
			for (auto name : layerNames) {
				if (getExtensionProperties(name).empty()) {
					unsupport.push_back(name);
				}
			}
			return unsupport;
		}

		CUBuffer<VkExtensionProperties> getExtensionProperties(const char* layerName = nullptr) const {
			return vkhelpler::cuGetDeviceExtensionProperties(m_physDevice, layerName);
		}

	private:
		VkResult _init(VkPhysicalDevice device, const CUBuffer<VkDeviceQueueCreateInfo>& queueInfos, const CUBuffer<const char*>& extensions) {
			using namespace vkhelpler;

			m_physDevice = device;


			//save queue infos
			for (auto& info : queueInfos) {
				CUBuffer<float> priorities;
				assert(info.pQueuePriorities);
				priorities.insert(0, info.pQueuePriorities, info.pQueuePriorities + info.queueCount);
				m_queueInfos[info.queueFamilyIndex] = DeviceQueueInfo{ priorities };
			}

			VkPhysicalDeviceFeatures deviceFeatures{};

			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			createInfo.pQueueCreateInfos = queueInfos.data();
			createInfo.queueCreateInfoCount = (uint32_t) queueInfos.size();
			createInfo.pEnabledFeatures = &deviceFeatures;

			for (auto ext : extensions) {
				if (getExtensionProperties(ext).empty()) {
					LOG_ERR("fail to get extension: {}", ext);
					return VK_ERROR_EXTENSION_NOT_PRESENT;
				}
			}
			createInfo.enabledExtensionCount = (uint32_t)extensions.size();
			createInfo.ppEnabledExtensionNames = extensions.data();

			auto& validationLayers = g_resource.getValidationLayers();
			cuFillLayers(createInfo, validationLayers);
			if (auto re = vkCreateDevice(device, &createInfo, nullptr, &m_vkDevice)) {
				LOG_ERR("failed to create logical device!");
				return re;
			}

			return VK_SUCCESS;
		}

		struct DeviceQueueInfo {
			CUBuffer<float> priorites;
		};

		VkDevice m_vkDevice = VK_NULL_HANDLE;
		VkPhysicalDevice m_physDevice = VK_NULL_HANDLE;
		CUHashMap<uint32_t, DeviceQueueInfo> m_queueInfos;

		std::optional<uint32_t> m_graphicFamilyIdx;
		std::optional<uint32_t> m_presentFamilyIdx;

		inline const static CUBuffer<const char*> s_renderWindowExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
	};

	enum CUVsync {
		CUBUI_VSYNC_NONE,
		CUBUI_VSYNC_RELAX,
		CUBUI_VSYNC_STRONG
	};


	struct CUVulkanWindowBase : WindowBase {
		CUResult init() {
			using namespace vkhelpler;
			//check supported


			//initialize parent
			{
				auto re = WindowBase::init();
				if (re)
					return re;
			}

			m_surface = g_resource.createSurfaceKHR(*this);
			if (!m_surface) {
				return -1;
			}
			return 0;
		}


		CUResult initSwapChain(CUVkDevice* cudevice) {
			using namespace vkhelpler;
			//check supported
			if (!cudevice->ifSupport(m_surface))
				return -1;


			m_pcudevice = cudevice;

			auto physDevice = cudevice->getPhysDevice();

			updateSurfaceInfo();
			if (m_formats.empty() || m_presentModes.empty()) {
				LOG_ERR("fail to get surface info");
				return -2;
			}
			
			{
				auto format = m_surfaceFormat = chooseSwapSurfaceFormat();
				std::optional<VkPresentModeKHR> mode;
				unsigned int vsyncArr[] = { CUBUI_VSYNC_STRONG, CUBUI_VSYNC_RELAX, CUBUI_VSYNC_NONE };
				for (auto vsync : vsyncArr) {
					mode = choosePresentMode(vsync);
					if (mode.has_value())
						break;
				}
				auto presentMode = mode.value();
				auto imageCount = chooseImageCount(presentMode);

				VkSharingMode sharingMode;
				uint32_t familyIdxs[2];
				uint32_t familyIdxCount = 0;
				uint32_t* pFamilyIdxs = nullptr;
				if (cudevice->getGraphicFamilyIdx() == cudevice->getPresentFamilyIdx()) {
					sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				}
				else {
					sharingMode = VK_SHARING_MODE_CONCURRENT;
					familyIdxs[0] = cudevice->getGraphicFamilyIdx().value();
					familyIdxs[1] = cudevice->getPresentFamilyIdx().value();
					pFamilyIdxs = familyIdxs;
					familyIdxCount = 2;
				}


				VkSwapchainCreateInfoKHR info{
					.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
					.pNext = nullptr,
					.flags = 0,
					.surface = m_surface,
					.minImageCount = imageCount,
					.imageFormat = format.format,
					.imageColorSpace = format.colorSpace,
					.imageExtent = getSwapChainExtent(),

					//used for VR app
					.imageArrayLayers = 1,	

					.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
					.imageSharingMode = sharingMode,
					.queueFamilyIndexCount = familyIdxCount,
					.pQueueFamilyIndices = pFamilyIdxs,

					//If the swapchain suppport transfomrs, it can be set to some transformation like rotation
					.preTransform = m_surfaceCapabilities.currentTransform,

					//using for being composited with other window in the window system
					.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,

					.presentMode = presentMode,

					//if the value is ture, not visible pixels may be discarded.
					//if the application don't want to read the back the original content, it should be set to true
					.clipped = VK_TRUE,
					.oldSwapchain = VK_NULL_HANDLE
				};

				auto re = vkCreateSwapchainKHR(cudevice->getDevice(), &info, nullptr, &m_swapChain);
				if (re) {
					LOG_ERR("fail to create swap chain");
					return -3;
				}
			}
			{
				auto device = cudevice->getDevice();
				uint32_t count = 0;
				vkGetSwapchainImagesKHR(device, m_swapChain, &count, nullptr);
				m_swapChainImages.resize(count);
				vkGetSwapchainImagesKHR(device, m_swapChain, &count, m_swapChainImages.data());
			}

			initImageViews();

			return 0;
		}

		virtual void uninit() {
			if (m_swapChain) {
				m_swapChainImageViews.clear();
				m_swapChainImages.clear();
				vkDestroySwapchainKHR(m_pcudevice->getDevice(), m_swapChain, nullptr);
				m_swapChain = VK_NULL_HANDLE;
			}
			
			if (m_surface) {
				vkDestroySurfaceKHR(g_resource.getInstance(), m_surface, nullptr);
				m_surface = VK_NULL_HANDLE;
			}
			if (m_pcudevice) {
				m_pcudevice = nullptr;
			}
			WindowBase::uninit();
		}


		VkSurfaceKHR getSurface() const {
			return m_surface;
		}

		void initImageViews() {
			auto size = m_swapChainImages.size();
			m_swapChainImageViews.resize(size);
			for (int i = 0; i < size; i++) {
				VkImageViewCreateInfo info{
					.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.image = m_swapChainImages[i],
					.viewType = VK_IMAGE_VIEW_TYPE_2D,
					.format = m_surfaceFormat.format,
					.components = VkComponentMapping{
						.r = VK_COMPONENT_SWIZZLE_IDENTITY,
						.g = VK_COMPONENT_SWIZZLE_IDENTITY,
						.b = VK_COMPONENT_SWIZZLE_IDENTITY,
						.a = VK_COMPONENT_SWIZZLE_IDENTITY, 
					},
					.subresourceRange = VkImageSubresourceRange{
						.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,	//indicate the usage of image
						.baseMipLevel = 0,
						.levelCount = 1,
						.baseArrayLayer = 0,
						.layerCount = 1
					}
				};
				auto re = vkCreateImageView(m_pcudevice->getDevice(), &info, nullptr, &m_swapChainImageViews[i]);
				assert(re == 0);
			}
		}

		void updateSurfaceInfo() {
			using namespace vkhelpler;
			auto physDevice = m_pcudevice->getPhysDevice();
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, m_surface, &m_surfaceCapabilities);
			m_formats = cuGetPhysicalDeviceSurfaceFormatsKHR(physDevice, m_surface);
			m_presentModes = cuGetPhysicalDeviceSurfacePresentModesKHR(physDevice, m_surface);
		}

		VkExtent2D getSwapChainExtent() {
			if (m_surfaceCapabilities.currentExtent.width != UINT32_MAX)
				return m_surfaceCapabilities.currentExtent;
			else {
				auto size = getClientSize();
				auto& cap = m_surfaceCapabilities;

				return VkExtent2D{
					.width = std::max(cap.minImageExtent.width, std::min(cap.maxImageExtent.width, size.width)),
					.height = std::min(cap.minImageExtent.height, std::min(cap.maxImageExtent.height, size.height))
				};
			}
		}
		VkSurfaceFormatKHR chooseSwapSurfaceFormat() {
			auto& availableFormats = m_formats;
			for (const auto& availableFormat : availableFormats) {
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return availableFormat;
				}
			}

			return availableFormats[0];
		}

		uint32_t chooseImageCount(VkPresentModeKHR mode) {
			uint32_t count;
			auto& cap = m_surfaceCapabilities;
			if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				count = 3;
			}
			else {
				count = 2;
			}

			if (cap.maxImageCount) {
				count = std::min(count, cap.maxImageCount);
			}

			count = std::max(count, cap.minImageCount);
			return count;
		}

		std::optional<VkPresentModeKHR> choosePresentMode(unsigned int vsync) {
			
			switch (vsync)
			{
			case CUBUI_VSYNC_NONE:
			{
				auto pmode = std::find(m_presentModes.begin(), m_presentModes.end(),
					VK_PRESENT_MODE_IMMEDIATE_KHR
				);
				if (pmode)
					return *pmode;
			}
				break;
			case CUBUI_VSYNC_RELAX:
			{
				auto pmode = std::find(m_presentModes.begin(), m_presentModes.end(),
					VK_PRESENT_MODE_FIFO_RELAXED_KHR
				);
				if (pmode)
					return *pmode;
			}
				break;
			case CUBUI_VSYNC_STRONG:
			{
				auto pmode = std::find(m_presentModes.begin(), m_presentModes.end(),
					VK_PRESENT_MODE_MAILBOX_KHR
				);
				if (!pmode) {
					pmode = std::find(m_presentModes.begin(), m_presentModes.end(),
						VK_PRESENT_MODE_FIFO_KHR
					);
				}
				if (pmode) {
					return *pmode;
				}
			}
				break;
			}
			return {};
		}


	private:
		CUVkDevice* m_pcudevice = nullptr;
		VkSurfaceKHR m_surface = VK_NULL_HANDLE;
		VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
		CUBuffer<VkImage> m_swapChainImages;
		CUBuffer<VkImageView> m_swapChainImageViews;

		VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
		CUBuffer<VkSurfaceFormatKHR> m_formats;
		CUBuffer<VkPresentModeKHR> m_presentModes;
		VkSurfaceFormatKHR m_surfaceFormat;
	};
}



using namespace cubui;
using namespace cubui::vkhelpler;
struct MyWindow : CUVulkanWindowBase {
	virtual CUResult when(CUEvent* e) {
		auto& msg = e->rawEvent;

		switch (msg.message)
		{
		case WM_PAINT:
			LOG_INFO("PAINTING");

		default:
			break;
		}
		//return Window::when(e);
		return WindowBase::when(e);
	}
};

CUBUI_MAIN_HEADER{
    CubuiConfig config;
    auto n = config.init(hInstance, pCmdLine, nCmdShow);

	using namespace vkhelpler;


	g_resource.init();

	CUVkDevice device;

    auto wnd = MyWindow();
	wnd.init();
	auto size = wnd.getClientSize();
	assert(device.init(wnd.getSurface()) == 0);
	wnd.initSwapChain(&device);






    EventLoop loop;

    LOG_INFO("success");
	loop.run();
	Sleep(200);

	wnd.uninit();
	device.uninit();
	g_resource.uninit();
	return 0;
}
