
namespace gui {
	
#define CLASS_NAME "panel"

	class panel {
		HWND hpnlWnd;
	public:
		panel(HINSTANCE hmInst, HWND hwParent);
		
		void show();
		
		~panel();
		
	};
	

}