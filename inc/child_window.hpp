namespace gui {

    struct Rectangle {
            bool x_relative; // if flag set, size by axis is specified
            bool y_relative; // in percents, relative to parent size
            long x0;
            long y0;
            long width;
            long height;
        };

    class child_window {
            HWND chld_hwnd;
            HWND m_hwnd;
            Rectangle wind_rec;

        public:
            child_window(HWND ChildWindow, HWND ParentWindow, Rectangle WindRect)
            : chld_hwnd(ChildWindow), m_hwnd(ParentWindow), wind_rec(WindRect) {}
            
            virtual Rectangle get_rect();

            virtual HWND get_hwnd();
    };

}



