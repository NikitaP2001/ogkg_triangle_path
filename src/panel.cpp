#include <windows.h>
#include <vector>

#include "main.hpp"
#include "panel.hpp"
#include "error.hpp"

using namespace gui;

std::vector<panel*> panels;

static LRESULT CALLBACK PnlProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_CREATE:
			return 0;
			
		case WM_PAINT:
			return 0;

		case WM_SIZE:
			for (std::vector<panel*>::iterator it = panels.begin();
			it != panels.end(); ++it) {
				if ((*it)->get_hwnd() == hWnd) {
					INFO("find wnd");
					gui::Rectangle apr = (*it)->get_rect();
					if (!MoveWindow(hWnd, apr.x0, apr.y0, apr.width,
					apr.height, true))
						ERR2("MoveWindow", GetLastError());
					break;
				}
			}		

			return 0;

		case WM_DESTROY:
			return 0;
	}	
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}

panel::panel(HWND hwParent, Rectangle pnlRect, int bgColor)
: child_window(hwParent, pnlRect)
{			
	WNDCLASSEX wc;

	// absolute panel rectangle
	Rectangle apr = get_rect();
	
	if (!GetClassInfoExA(NULL, CLASS_NAME, (LPWNDCLASSEXA)&wc)) {
	
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = PnlProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = (HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE);
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
							WS_BORDER | WS_CHILD | WS_GROUP | WS_VISIBLE,
							apr.x0, apr.y0,
							apr.width, apr.height,
							m_hwnd,
							NULL,
							(HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE),
							NULL);
	if (chld_hwnd == NULL)
		ERR2("CreateWindowEx() failed", GetLastError());

	// add panel to global panels vec
	panels.push_back(this);
	INFO("panels size: " << panels.size());
}

void panel::show()
{
	ShowWindow(chld_hwnd, SW_NORMAL);
	if (!UpdateWindow(chld_hwnd))
		ERR("UpdateWindow()");
}

panel::~panel()
{
	for (std::vector<panel*>::iterator it = panels.begin();
	it != panels.end(); ++it) {
		if (*it == this)
			panels.erase(it);
	}

	if (IsWindow(chld_hwnd))
		DestroyWindow(chld_hwnd);
}

/* get rect of panel in coords, relative
 * to the parent window
 */
gui::Rectangle panel::get_rect()
{
	RECT prRec;
	Rectangle chRec = {
		.x_relative = false,
		.y_relative = false,
	};
	if (!GetWindowRect(m_hwnd, &prRec))
		ERR2("GetWindowRect()", GetLastError());

	long prW = prRec.right - prRec.left;
	long prH = prRec.bottom - prRec.top;
	
	if (wind_rec.x_relative) {
		chRec.x0 = prW * wind_rec.x0 / 100;
		chRec.width = prW * wind_rec.width / 100;
	} else {
		chRec.x0 = wind_rec.x0;
		chRec.width = wind_rec.width;
	}

	if (wind_rec.y_relative) {
		chRec.y0 = prH * wind_rec.y0 / 100;
		chRec.height = prH * wind_rec.height / 100;
	} else {
		chRec.y0 = wind_rec.y0;
		chRec.height = wind_rec.height;
	}


	return chRec;
}

HWND panel::get_hwnd()
{
	return chld_hwnd;
}
