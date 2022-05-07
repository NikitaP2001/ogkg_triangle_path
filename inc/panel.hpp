
namespace gui {
	
#define CLASS_NAME "panel"

	struct Rectangle {
		bool x_relative; // if flag set, size by axis is specified
		bool y_relative; // in percents, relative to parent size
		long x0;
		long y0;
		long width;
		long height;
	};

	class panel {
		HWND chld_hwnd;
		HWND m_hwnd;
		Rectangle wind_rec;
	public:
		panel(HWND hwParent, Rectangle pnRec, int bgColor);

		Rectangle get_panel_rect();

		HWND get_panel_hwnd();

		void show();
		
		~panel();
		
	};

	
}