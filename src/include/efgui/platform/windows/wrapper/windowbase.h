#pragma once
#define UNICODE
#include <cstdint>
#include <Windows.h>
#include "globalvar.h"

namespace os_windows
{
	struct WindowBase
	{
		virtual ~WindowBase() {
			uninit();
		}

		bool init(const wchar_t* title = nullptr, const int sizes[4] = nullptr);
		void uninit();
		HWND get_hwnd() const {
			return m_hwnd;
		}

		//return 0 default process
		virtual bool handle_message(UINT msg, WPARAM wParam, LPARAM lParam) {
			return 0;
		}

		static int get_wnd_count() {
			return s_wnd_count;
		}

		static bool s_init();//should call this!!!
	private:
		HWND m_hwnd = nullptr;

		static uint32_t s_wnd_count;
		static const wchar_t s_wcName[];
		static const wchar_t s_default_title[];
		static const int s_defaultSizes[];
		static const WNDCLASSW s_wc;
		static LRESULT wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}
