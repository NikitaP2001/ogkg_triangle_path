#include "child_window.hpp"

namespace gui {
	
	class panel : public child_window {
		virtual int msg_handler(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

	public:
		panel(HWND hwParent, Rectangle pnRec, int bgColor);

		void add_button(gui::Rectangle btnRect);

		virtual ~panel();
		
	};
	
}