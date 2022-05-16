#include "vector"
#include "main.hpp"
#include "button.hpp"
#include "error.hpp"

#define CLASS_NAME "my_button"

using namespace gui;

int button::msg_handler(HWND hWnd, UINT message, WPARAM wParam, 
LPARAM lParam)
{
	switch (message) {
		case WM_ERASEBKGND:
		{
			RECT btnRec;
			GetWindowRect(hWnd, &btnRec);
			HDC dcBg = CreateCompatibleDC((HDC)wParam);

			int bw = btnRec.right - btnRec.left;
			int bh = btnRec.bottom - btnRec.top;

			if (bmpCurrent != NULL) {
				SelectObject(dcBg, bmpCurrent);

				if (!BitBlt((HDC)wParam, 0, 0, bw, bh, dcBg,
				 0, 0, SRCCOPY))
					ERR2("bitblt() btn background failed", GetLastError());	

			} else {

				RECT lprc = {
					.left = 0,
					.top = 0,
					.right = bw,
					.bottom = bh,
				};
				HBITMAP hbm = CreateCompatibleBitmap(dcBg, btnRec.right - btnRec.left,
				btnRec.bottom - btnRec.top);
				SelectObject(dcBg, hbm);

				if (!FillRect(dcBg, &lprc, (HBRUSH)GetStockObject(WHITE_BRUSH)))
					ERR("fillrect() failed");
				if (!BitBlt((HDC)wParam, 0, 0, bw, bh, dcBg, 0, 0, SRCCOPY))
					ERR2("bitblt() btn background failed", GetLastError());

				DeleteObject(hbm);
			}

			DeleteDC(dcBg);
		}
	}

	int res;
	if (user_def_proc != NULL) {
		res = (*user_def_proc)(hWnd, message, wParam, lParam);
		if (res == 0)
			return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

button::button(HWND hwParent, Rectangle pnlRect)
: child_window(hwParent, pnlRect), user_def_proc(NULL), bmpCurrent(NULL),
bmpActive(NULL), bmpDisabled(NULL)
{
    WNDCLASSEX wc;
	HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE);

	// absolute button rectangle
	Rectangle apr = get_rect();
	
	if (!GetClassInfoExA(hInst, CLASS_NAME, (LPWNDCLASSEXA)&wc)) {
	
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_DROPSHADOW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInst;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor	= LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
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

int button::set_bmp_active(int resource)
{

	HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE);
	bmpActive = LoadBitmap(hInst, MAKEINTRESOURCE(resource));
	bmpCurrent = bmpActive;
	if (bmpActive == NULL)
		ERR("Loading bmp active res failed");
		return 0;
	return 1;
}

int button::set_bmp_disabled(int resource)
{
	HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE);
	bmpDisabled = LoadBitmap(hInst, MAKEINTRESOURCE(resource));
	if (bmpDisabled == NULL) {
		ERR("Loading bmp disabled res failed");
		return 0;
	}
	return 1;
}

user_proc button::set_handler(user_proc new_proc)
{
	user_proc old = user_def_proc;
	user_def_proc = new_proc;
	return old;
}

button::~button()
{
	if (bmpActive != NULL)
		DeleteObject(bmpActive);
	if (bmpDisabled != NULL)
		DeleteObject(bmpDisabled);

}