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
			INFO("create");
			return 0;
			
		case WM_PAINT:
			return 0;

		case WM_SIZE:
			INFO("resized");
			for (std::vector<panel*>::iterator it = panels.begin();
			it != panels.end(); ++it) {
				if ((*it)->get_panel_hwnd() == hWnd) {
					INFO("find wnd");
					gui::Rectangle apr = (*it)->get_panel_rect();
					if (!MoveWindow(hWnd, apr.x0, apr.y0, apr.width,
					apr.height, true))
						ERR2("MoveWindow", GetLastError());
					break;
				}
			}		

			return 0;

		case WM_DESTROY:
			INFO("destroy");
			return 0;
	}	
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}

panel::panel(HINSTANCE hmInstance, HWND hwParent, Rectangle pnlRect, int bgColor)
: hm_inst(hmInstance), hw_par(hwParent), pnl_rec(pnlRect)
{			
	WNDCLASSEX wc;

	// absolute panel rectangle
	Rectangle apr = get_panel_rect();
	
	if (!GetClassInfoExA(NULL, CLASS_NAME, (LPWNDCLASSEXA)&wc)) {
	
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = PnlProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hm_inst;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor	= LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(bgColor);
		wc.lpszMenuName	= 0;
		wc.lpszClassName = TEXT(CLASS_NAME);
		wc.hIconSm = NULL;
		
		if (!RegisterClassEx(&wc))		
			ERR2("register class failed", GetLastError());
	
	}
		
	h_pnlwnd = CreateWindowEx(0,
							TEXT(CLASS_NAME),
							NULL,
							WS_BORDER | WS_CHILD | WS_GROUP | WS_VISIBLE,
							apr.x0, apr.y0,
							apr.width, apr.height,
							hw_par,
							NULL,
							hm_inst,
							NULL);
	if (h_pnlwnd == NULL)
		ERR2("CreateWindowEx() failed", GetLastError());

	// add panel to global panels vec
	panels.push_back(this);
}

void panel::show()
{
	ShowWindow(h_pnlwnd, SW_NORMAL);
	if (!UpdateWindow(h_pnlwnd))
		ERR("UpdateWindow()");
}

panel::~panel()
{
	for (std::vector<panel*>::iterator it = panels.begin();
	it != panels.end(); ++it) {
		if (*it == this)
			panels.erase(it);
	}

	if (IsWindow(h_pnlwnd))
		DestroyWindow(h_pnlwnd);
}

/* get rect of panel in coords, relative
 * to the parent window
 */

gui::Rectangle panel::get_panel_rect()
{
	RECT prRec;
	Rectangle chRec = {
		.x_relative = false,
		.y_relative = false,
	};
	if (!GetWindowRect(hw_par, &prRec))
		ERR2("GetWindowRect()", GetLastError());

	long prW = prRec.right - prRec.left;
	long prH = prRec.bottom - prRec.top;
	
	if (pnl_rec.x_relative) {
		chRec.x0 = prW * pnl_rec.x0 / 100;
		chRec.width = prW * pnl_rec.width / 100;
	} else {
		chRec.x0 = pnl_rec.x0;
		chRec.width = pnl_rec.width;
	}

	if (pnl_rec.y_relative) {
		chRec.y0 = prH * pnl_rec.y0 / 100;
		chRec.height = prH * pnl_rec.height / 100;
	} else {
		chRec.y0 = pnl_rec.y0;
		chRec.height = pnl_rec.height;
	}


	return chRec;
}

HWND panel::get_panel_hwnd()
{
	return h_pnlwnd;
}
