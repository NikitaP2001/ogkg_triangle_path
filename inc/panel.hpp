#include "child_window.hpp"

namespace gui {
	
#define CLASS_NAME "panel"

	
	class panel : public child_window {
			
	public:
		panel(HWND hwParent, Rectangle pnRec, int bgColor);

		Rectangle get_rect();

		HWND get_hwnd();

		void show();
		
		~panel();
		
	};

	
}