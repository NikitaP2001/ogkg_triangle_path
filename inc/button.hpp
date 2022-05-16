#include "child_window.hpp"

namespace gui {

    class button : public child_window {
        user_proc user_def_proc;

        HBITMAP bmpActive;
        HBITMAP bmpDisabled;
        HBITMAP bmpCurrent;

        virtual int msg_handler(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

    public:
        button(HWND hwParent, Rectangle pnlRect);

        int set_bmp_active(int resource);
        int set_bmp_disabled(int resource);

        user_proc set_handler(user_proc new_proc);

        virtual ~button();

    };
}