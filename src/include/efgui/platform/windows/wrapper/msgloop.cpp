#include <Windows.h>
#include "msgloop.h"

namespace os_windows {

	void MsgLoop::quit()
	{
		PostQuitMessage(0);
	}

	int MsgLoop::exec()
	{
		MSG msg{};
		while (msg.message != WM_QUIT) {
			if (m_enable_idle) {
				if (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessageW(&msg);
				}
				else {
					when_idle();
				}
			}
			else {
				GetMessageW(&msg, NULL, 0, 0);
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
		return 0;
	}
}