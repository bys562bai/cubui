#define UNICODE
#include <cstdint>
#include <Windows.h>
#include "windowbase.h"
//return 0 if success, otherwise fail


namespace os_windows
{
	HINSTANCE g_hinstance;
	PWSTR g_cmdLine;
	int g_ncmdShow;


	uint32_t WindowBase::s_wnd_count = 0;
	const wchar_t WindowBase::s_wcName[] = L"WindowBase";
	const wchar_t WindowBase::s_default_title[] = L"";
	const int WindowBase::s_defaultSizes[] = { CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT };
	const WNDCLASSW WindowBase::s_wc = {
			CS_HREDRAW | CS_VREDRAW,
			wnd_proc,
			0,
			0,
			g_hinstance,
			NULL,
			NULL,
			NULL,
			NULL,
			s_wcName
	};

	bool WindowBase::init(const wchar_t* title, const int sizes[4])
	{
		if (title == nullptr) {
			title = s_default_title;
		}
		if (sizes == nullptr) {
			sizes = s_defaultSizes;
		}

		CreateWindowExW(
			0,
			s_wcName,
			title,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			sizes[0], sizes[1], sizes[2], sizes[3],
			NULL,       // Parent window    
			NULL,       // Menu
			g_hinstance,  // Instance handle
			this
		);
		return !m_hwnd;
	}

	void WindowBase::uninit()
	{
		if (m_hwnd) {
			DestroyWindow(m_hwnd);
			m_hwnd = NULL;
		}
	}

	bool WindowBase::s_init()
	{
		auto& wc = s_wc;
		if (RegisterClassW(&wc)) {
			return 0;//success
		}
		return -1;//fail
	}

	LRESULT WindowBase::wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		WindowBase* self = (WindowBase*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
		if (self) {
			auto ret = self->handle_message(msg, wParam, lParam);
			if (msg == WM_DESTROY) {
				--s_wnd_count;
			}
			if (ret) {
				return 0;
			}
		}
		else if (msg == WM_CREATE) {
			self = (WindowBase*)((CREATESTRUCT*)lParam)->lpCreateParams;
			SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)self);
			self->m_hwnd = hwnd;
			++s_wnd_count;
		}

		return DefWindowProcW(hwnd, msg, wParam, lParam);
	}

}

