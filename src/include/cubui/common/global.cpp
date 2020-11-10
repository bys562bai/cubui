#include "global.h"
namespace cubui {
	namespace global_val {
#ifdef CUBUI_PLATFORM_WINDOWS
		HINSTANCE g_hinstance;
		PWSTR g_pCmdLine;
		int g_nCmdShow;
#endif
		int g_argc;
		char** g_argv;

	}
}