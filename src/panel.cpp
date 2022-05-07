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
		case WM_SIZE:
			RECT prRec;
			if (!GetWindowRect(hw_par, &prRec))
				ERR2("GetWindowRect()", GetLastError());

			long prW = prRec.right - prRec.left;
			long prH = prRec.bottom - prRec.top;
			
			long cld_x0, cld_w;
			if (pnl_rec.x_relative) {
				cld_x0 = prW * pnl_rec.x0 / 100;
				cld_w = prW * pnl_rec.width / 100;
			} else {
				cld_x0 = pnl_rec.x0;
				cld_w = pnl_rec.width;
			}
			long cld_y0, cld_h;
			if (pnl_rec.y_relative) {
				cld_y0 = prH * pnl_rec.y0 / 100;
				cld_h = prH * pnl_rec.heigth / 100;
			} else {
				cld_y0 = pnl_rec.y0;
				cld_h = pnl_rec.heigth;
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

	// calculate rect of panel
	RECT prRec;
	if (!GetWindowRect(hw_par, &prRec))
		ERR2("GetWindowRect()", GetLastError());

	long prW = prRec.right - prRec.left;
	long prH = prRec.bottom - prRec.top;
	
	long cld_x0, cld_w;
	if (pnl_rec.x_relative) {
		cld_x0 = prW * pnl_rec.x0 / 100;
		cld_w = prW * pnl_rec.width / 100;
	} else {
		cld_x0 = pnl_rec.x0;
		cld_w = pnl_rec.width;
	}
	long cld_y0, cld_h;
	if (pnl_rec.y_relative) {
		cld_y0 = prH * pnl_rec.y0 / 100;
		cld_h = prH * pnl_rec.heigth / 100;
	} else {
		cld_y0 = pnl_rec.y0;
		cld_h = pnl_rec.heigth;
	}

	
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
							cld_x0, cld_y0,
							cld_w, cld_h,
							hw_par,
							NULL,
							hm_inst,
							NULL);
	if (h_pnlwnd == NULL)
		ERR2("CreateWindowEx() failed", GetLastError());


}

void panel::show()
{
	ShowWindow(h_pnlwnd, SW_NORMAL);
	if (!UpdateWindow(h_pnlwnd))
		ERR("UpdateWindow()");
}

panel::~panel()
{
	if (IsWindow(h_pnlwnd))
		DestroyWindow(h_pnlwnd);
}