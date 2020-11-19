#pragma once
#include <cubui/common/predef.h>
#ifdef CUBUI_PLATFORM_WINDOWS
#include <cubui/ext/windowsinc.h>
#endif

#include "loggingconfig.h"
#include "windowconfig.h"
#include "renderconfig.h"

#include <cubui/common/usefultype.h>
#include <cubui/common/global.h>
#include "confignode.h"

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
					auto argl = (argwl+1) * 6;

					char* arg = new char[argl];
					auto argl_real = WideCharToMultiByte(
						CP_UTF8,
						0,
						argw,
						(int) argwl,
						arg,
						(int) argl,
						NULL,
						NULL
					);

					
					for (int i = argl_real; i < argl_real+6; i++) {
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
				&windowConfig,
				&renderConfig
			};
			//init loop
			for (int i = 0; i < sizeof(arr) / sizeof(ConfigNode*); i++) {
				auto n = arr[i];
				if (n->inited())
					continue;
				if (auto re = n->init()) {
					if (n->whenFail(re))
						return n;
				}
				n->setInited();
			}

			setInited();
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
		WindowConfig windowConfig;
		RenderConfig renderConfig;

	};

} // namespace cubui
