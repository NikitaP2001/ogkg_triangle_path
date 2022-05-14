#include "canvas.hpp"
#include "error.hpp"
#include "main.hpp"

#define CLASS_NAME "canvas"

using namespace gui;

int canvas::msg_handler(HWND hWnd, UINT message, WPARAM wParam, 
LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

canvas::canvas(HWND hwParent, Rectangle plRect, int bgColor)
: child_window(hwParent, plRect)
{			
	WNDCLASSEX wc;
	HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE);

	// absolute panel rectangle
	Rectangle apr = get_rect();
	
	if (!GetClassInfoExA(hInst, CLASS_NAME, (LPWNDCLASSEXA)&wc)) {
	
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInst;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor	= LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(bgColor);
		wc.lpszMenuName	= 0;
		wc.lpszClassName = TEXT(CLASS_NAME);
		wc.hIconSm = NULL;
		
		if (!RegisterClassEx(&wc))		
			ERR2("register class failed", GetLastError());
	
	}
		
	chld_hwnd = CreateWindowEx(0,
							TEXT(CLASS_NAME),
							NULL,
							WS_CHILD | WS_VISIBLE,
							apr.x0, apr.y0,
							apr.width, apr.height,
							m_hwnd,
							NULL,
							hInst,
							NULL);
	if (chld_hwnd == NULL)
		ERR2("CreateWindowEx() failed", GetLastError());

	// add panel to global panels vec
	all_childs.push_back(this);
}

canvas::~canvas()
{

}