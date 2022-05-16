#ifndef CHILD_WINDOW_HPP
#define CHILD_WINDOW_HPP
#include <vector>
#include "windows.h"

namespace gui {

    typedef int (*user_proc)(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam);

    struct Rectangle {
        bool x_relative; // if flag set, size by axis is specified
        long x0;
        bool y_relative; // in percents, relative to parent size
        long y0;
        bool w_relative;
        long width;
        bool h_relative;
        long height;
        bool use_reverse_x1; // use distance from parent wnd end
        long x1;
        bool use_reverse_y1;
        long y1;
    };

    class child_window {

    protected:

        HWND chld_hwnd;
        HWND m_hwnd;
        Rectangle wind_rec;

        // hold all created childs for wndproc callback
        static std::vector<child_window*> all_childs;

        // childs of this window
		std::vector<gui::child_window*> elements;

        static gui::child_window* find_child_window(HWND hwin);

        static LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

        virtual int msg_handler(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam) = 0;

    public:

        child_window(HWND ParentWindow, Rectangle WindRect)
        : m_hwnd(ParentWindow), wind_rec(WindRect) {}
        
        virtual gui::Rectangle get_rect();

        virtual ~child_window();

        HWND get_hwnd() const;

        void scale_position();
    };


}


#endif // CHILD_WINDOW_HPP