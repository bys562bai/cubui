#pragma once
#include <cubui/common/predef.h>

#include <stdint.h>

#ifdef CUBUI_PLATFORM_WINDOWS
#include <cubui/ext/windowsinc.h>
#endif

#include <cubui/common/usefultype.h>
#include <cubui/graphic/primitive.h>


namespace cubui
{
    struct CUEvent;
    //must be created by new
    struct WindowBase{
        virtual CUResult init(const wchar_t* title = nullptr, const int sizes[4] = nullptr);
        virtual void uninit();

        //It should be deleted when destroy message received.
        //return 0 default process
        virtual CUResult when(CUEvent* e);
        virtual Rect32 getClientSize();

        
        void show(bool x = true);
        
        //No virtual, because uninit() is already virtual
        ~WindowBase() { 
            uninit();
        };  


    private:


#ifdef CUBUI_PLATFORM_WINDOWS
    public:
        static uint32_t getWndNum() { 
            return s_wnd_count; 
        }

        HWND getHwnd() const { return m_hwnd; }

        //return 0 default process
        virtual CUResult handleMsg(UINT msg, WPARAM wParam, LPARAM lParam);

        //call it first! return 0 means scuccess
        static CUResult s_init();

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
