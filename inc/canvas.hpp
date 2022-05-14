#include "child_window.hpp"


namespace gui {

    class canvas : public child_window {
		virtual int msg_handler(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

	public:
		canvas(HWND hwParent, Rectangle plRec, int bgColor);

		virtual ~canvas();

    };
}