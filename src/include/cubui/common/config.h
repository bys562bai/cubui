#pragma once
#include "predef.h"

#ifdef CUBUI_PLATFORM_WINDOWS
#include <cubui/impl/windows/common/config.h>
#endif

#ifdef CUBUI_USING_VULKAN
#include <cubui/impl/vulkan/config.h>
#endif

#include <cubui/util/confignode.h>
#include <cubui/util/logging.h>

#include <cubui/common/usefultype.h>
#include <cubui/common/global.h>

namespace cubui
{
	struct CubuiConfig : ConfigNode{
#ifdef CUBUI_PLATFORM_WINDOWS

		ConfigNode* init(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow) {
			{
				using namespace global_val;
				g_hinstance = hInstance;
				g_pCmdLine = pCmdLine;
				g_nCmdShow = nCmdShow;
				LPWSTR* argvw = CommandLineToArgvW(GetCommandLineW(), &g_argc);
				if (!argvw)
					return this;
				g_argv = new char* [g_argc];
				for (int i = 0; i < g_argc; i++) {
					LPWSTR argw = argvw[i];
					auto argwl = wcslen(argw);
					auto argl = (argwl+1) * 4;

					char* arg = new char[argl];
					auto argl_real = WideCharToMultiByte(
						CP_UTF8,
						0,
						argw,
						argwl,
						arg,
						argl,
						NULL,
						NULL
					);

					
					for (int i = argl_real; i < argl_real+4; i++) {
						arg[i] = '\0';
					}

					g_argv[i] = arg;
				}

			}

#else
			{
				using namespace global_val;
				g_argc = argc;
				g_argv = argv;
			}
#endif
			ConfigNode* arr[] = {
				&logConfig,
#ifdef CUBUI_PLATFORM_WINDOWS
				&windowsConfig,
#endif
#ifdef CUBUI_USING_VULKAN
				&vulkanConfig,
#endif
			};

			for (int i = 0; i < sizeof(arr) / sizeof(ConfigNode*); i++) {
				auto n = arr[i];
				if (n->inited())
					continue;
				if (auto re = n->init()) {
					if (n->whenFail(re))
						return n;
				}
				n->set_inited();
			}
			return nullptr;
		}
		virtual ~CubuiConfig(){
#ifdef CUBUI_PLATFORM_WINDOWS
			using namespace global_val;
			for (int i = 0; i < g_argc; i++) {
				delete[] g_argv[i];
			}
			delete[] g_argv;
			g_argc = 0;
			g_argv = nullptr;
#endif
		}

		LoggingConfig logConfig;


#ifdef CUBUI_PLATFORM_WINDOWS
		WindowsPlatformConfig windowsConfig;
#endif

#ifdef CUBUI_USING_VULKAN
		VulkanConfig vulkanConfig;
#endif

	};

} // namespace cubui
