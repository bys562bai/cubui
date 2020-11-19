#include <cubui/common/global.h>
#include <cubui/basic/windowbase.h>
#include <cubui/basic/event.h>


namespace cubui{
	using global_val::g_hinstance;
	using global_val::g_nCmdShow;
	using global_val::g_pCmdLine;

	uint32_t WindowBase::s_wnd_count = 0;
	const wchar_t WindowBase::s_wcName[] = L"CubuiWindow";
	const wchar_t WindowBase::s_default_title[] = L"";
	const int WindowBase::s_defaultSizes[] = { CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT };
	const WNDCLASSW WindowBase::s_wc = {
			CS_HREDRAW | CS_VREDRAW,
			wndProc,
			0,
			0,
			g_hinstance,
			NULL,
			NULL,
			NULL,
			NULL,
			s_wcName
	};

	CUResult WindowBase::init(const wchar_t* title, const int sizes[4])
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
		}
	}

	void WindowBase::show(bool x)
	{
		if (m_hwnd) {
			if (x) {
				ShowWindow(m_hwnd, SW_SHOW);
			}
			else {
				ShowWindow(m_hwnd, SW_HIDE);
			}
		}
	}

	CUResult WindowBase::when(CUEvent* e){
		if (e->id == eventid::QUIT) {
			uninit();
		}
		return 0;
	}

	Rect32 WindowBase::getClientSize()
	{
		RECT rect;
		GetClientRect(m_hwnd,&rect);

		return Rect32{
			.width = (uint32_t)(rect.right - rect.left),
			.height = (uint32_t)(rect.bottom - rect.top)
		};
	}

	namespace {
		void cvtMsgToEvent(CUEvent& e) {
			//TODO
			auto& re = e.rawEvent;
			if (re.message == WM_DESTROY) {
				e.id = eventid::DESTROY;
			}
			else if (re.message == WM_QUIT) {
				e.id = eventid::QUIT;
			}
			else {
				e.id = eventid::UNKNOWN;
			}
		}
	}

	CUResult WindowBase::handleMsg(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		CUEvent e;
		e.rawEvent.message = msg;
		e.rawEvent.wParam = wParam;
		e.rawEvent.lParam = lParam;
		cvtMsgToEvent(e);

		return when(&e);
	}

	CUResult WindowBase::s_init()
	{
		auto& wc = s_wc;
		if (RegisterClassW(&wc)) {
			return 0;//success
		}
		return -1;//fail
	}


	LRESULT WindowBase::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		WindowBase* self = (WindowBase*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
		if (self) {
			auto ret = self->handleMsg(msg, wParam, lParam);
			if (msg == WM_DESTROY) {
				--s_wnd_count;
				self->m_hwnd = NULL;
				SetWindowLongPtrW(hwnd, GWLP_USERDATA, NULL);
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