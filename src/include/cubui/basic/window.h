#pragma once
#include <cubui/common/predef.h>

#include <stdint.h>

#ifdef CUBUI_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include <cubui/common/usefultype.h>


namespace cubui
{
    struct Event;
    //must be created by new
    struct Window{
        virtual Result init(const wchar_t* title = nullptr, const int sizes[4] = nullptr);
        void uninit();

        //It should be deleted when destroy message received.
        //return 0 default process
        virtual Result when(Event* e);

        
        void show(bool x = true);
        
        //Do not destroy it in this function because window is controled by user.
        virtual ~Window() { };  

#ifdef CUBUI_PLATFORM_WINDOWS
        static uint32_t getWndNum() { 
            return s_wnd_count; 
        }
#endif


#ifdef CUBUI_PLATFORM_WINDOWS
        HWND getHwnd() const { return m_hwnd; }

        //return 0 default process
        virtual Result handleMsg(UINT msg, WPARAM wParam, LPARAM lParam);

        //call it first! return 0 means scuccess
        static Result s_init();

        private:

		static LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

        HWND m_hwnd = nullptr;
		static uint32_t s_wnd_count;
		static const wchar_t s_wcName[];
		static const wchar_t s_default_title[];
		static const int s_defaultSizes[];
		static const WNDCLASSW s_wc;

#endif //CUBUI_PLATFORM_WINDOWS
    };
    
};
