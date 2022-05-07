
namespace gui {
	
#define CLASS_NAME "panel"

	struct Rectangle {
		bool x_relative; // if flag set, size by axis is specified
		bool y_relative; // in percents, relative to parent size
		long x0;
		long y0;
		long width;
		long heigth;
	};

	class panel {
		HWND h_pnlwnd;
		HWND hw_par;
		HINSTANCE hm_inst;
		Rectangle pnl_rec;
	public:
		panel(HINSTANCE hmInst, HWND hwParent, Rectangle pnRec, int bgColor);
		
		void show();
		
		~panel();
		
	};

	
}