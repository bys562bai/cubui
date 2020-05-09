#pragma once
namespace os_windows {

	struct MsgLoop
	{
		int exec();
		void quit();
		void enable_idle(bool enable) { m_enable_idle = enable; }
		virtual void when_idle() {}
	private:
		bool m_enable_idle = false;
	};
}