#include <windows.h>

#include "main.hpp"
#include "panel.hpp"
#include "error.hpp"

using namespace gui;

static LRESULT CALLBACK PnlProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_CREATE:
			INFO("create");
			return 0;
			
			
		case WM_PAINT:
			INFO("paint");
			return 0;
			
		case WM_DESTROY:
			INFO("destroy");
			return 0;
	}	
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}

panel::panel(HINSTANCE hmInst, HWND hwParent)
{			
	WNDCLASSEX wc;
	
	if (!GetClassInfoExA(NULL, TEXT(CLASS_NAME), &wc)) {
	
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = PnlProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hmInst;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor	= LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName	= 0;
		wc.lpszClassName = TEXT(CLASS_NAME);
		wc.hIconSm = NULL;
		
		if (!RegisterClassEx(&wc))		
			ERR2("register class failed", GetLastError());
	
	}
		
	hpnlWnd = CreateWindowEx(0,
							TEXT(CLASS_NAME),
							NULL,
							WS_BORDER | WS_CHILD | WS_GROUP,
							0, 0,
							600, 50,
							hwParent,
							NULL,
							hmInst,
							NULL);
	if (hpnlWnd == NULL)
		ERR2("CreateWindowEx() failed", GetLastError());


}

void panel::show()
{
	ShowWindow(hpnlWnd, SW_NORMAL);
	UpdateWindow(hpnlWnd);
}

panel::~panel()
{
	if (!DestroyWindow(hpnlWnd))
		ERR2("DestroyWindow() failed", GetLastError());;
}