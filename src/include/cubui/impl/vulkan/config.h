#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cubui/util/confignode.h>
#include <cubui/common/usefultype.h>

namespace cubui{
	struct VulkanConfig : ConfigNode {
		virtual Result init();
		virtual const char* getName() const {
			static auto name = "Vulkan";
			return name;
		}

		virtual ~VulkanConfig();
	private:
		virtual Result createInstance();
		VkInstance m_instance;
	};
}