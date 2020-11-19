#include <cubui/ext/windowsinc.h>
#include <cubui/basic/event.h>
#include <cubui/basic/windowbase.h>


namespace cubui {
	int EventLoop::run()
	{
		CUEvent e;
		auto& msg = e.rawEvent;
		while (!m_quit) {
			if (m_enableIdle) {
				if (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
				}
				else {
					e.id = eventid::IDLE;
				}
			}
			else {
				GetMessageW(&msg, NULL, 0, 0);
				TranslateMessage(&msg);
			}
			when(&e);
		}

		return 0;
	}

	void EventLoop::dispatch(CUEvent* e)
	{
		DispatchMessageW(&(e->rawEvent));
	}

	void EventLoop::when(CUEvent* e)
	{
		if (WindowBase::getWndNum() == 0) {
			quit();
			return;
		}
		if (e->id == eventid::QUIT)
			return;
		else {
			dispatch(e);
		}

	}
}