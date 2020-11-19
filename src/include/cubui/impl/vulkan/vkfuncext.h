#pragma once
#include <cubui/ext/vulkaninc.h>

namespace cubui{
    struct CUVkFuncExt {
		VkInstance vkInstance = VK_NULL_HANDLE;
		PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
		PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;

		static constexpr size_t funcNum() {
			return (sizeof(CUVkFuncExt) - sizeof(VkInstance)) / sizeof(void*);
		}

		ArrayRef<void*> funcRef() {
			return ArrayRef<void*>((void**)this, funcNum());
		}

		void clear() {
			vkInstance = VK_NULL_HANDLE;
			funcRef().fill(nullptr);
		}

		template<typename T>
		T request(T f) {}

		template <>
		PFN_vkCreateDebugUtilsMessengerEXT request(PFN_vkCreateDebugUtilsMessengerEXT f) {
			return vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkInstance, "vkCreateDebugUtilsMessengerEXT");
		}

		template <>
		PFN_vkDestroyDebugUtilsMessengerEXT request(PFN_vkDestroyDebugUtilsMessengerEXT f) {
			return vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkInstance, "vkDestroyDebugUtilsMessengerEXT");
		}



#ifdef CUBUI_PLATFORM_WINDOWS
		PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;

		template<>
		PFN_vkCreateWin32SurfaceKHR request(PFN_vkCreateWin32SurfaceKHR f) {
			return vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(vkInstance, "vkCreateWin32SurfaceKHR");
		}
#endif


		//return true if fail
		bool requestDebugUtilsMessengerEXT() {
			bool failed = false;
			failed |= !request(vkCreateDebugUtilsMessengerEXT);
			failed |= !request(vkDestroyDebugUtilsMessengerEXT);
			return failed;
		}
	};
}