#include <stdexcept>
#include "main.hpp"
#include "child_window.hpp"
#include "error.hpp"

using namespace gui;

std::vector<child_window*> child_window::all_childs;

/* get rect of panel in coords, relative
 * to the parent window
 */
gui::Rectangle child_window::get_rect()
{
	RECT prRec;
	Rectangle chRec;

	if (!GetWindowRect(m_hwnd, &prRec))
		ERR2("GetWindowRect()", GetLastError());

	long prW = prRec.right - prRec.left;
	long prH = prRec.bottom - prRec.top;
	
	if (wind_rec.x_relative)
		chRec.x0 = prW * wind_rec.x0 / 100;
	else
		chRec.x0 = wind_rec.x0;

	if (wind_rec.y_relative)
		chRec.y0 = prH * wind_rec.y0 / 100;
	else
		chRec.y0 = wind_rec.y0;

	if (wind_rec.use_reverse_x1)
		chRec.width = prW - chRec.x0 - wind_rec.x1;
	else if (wind_rec.w_relative)
		chRec.width = prW * wind_rec.width / 100;
	else
		chRec.width = wind_rec.width;

	if (wind_rec.use_reverse_y1)
		chRec.height = prH - chRec.y0 - wind_rec.y1;
	else if (wind_rec.h_relative)
		chRec.height = prH * wind_rec.height / 100;
	else
		chRec.height = wind_rec.height;

	return chRec;
}

child_window::~child_window()
{
	// remove this from list
	auto it = all_childs.begin();
	while (it != all_childs.end()) {
		if ((*it)->get_hwnd() == this->get_hwnd())
			break;
	}

	if (it != all_childs.end()) {
		all_childs.erase(it);

		// destruct all childs of panel
		for (auto el : elements)
			delete el;
	} else
		ERR("panel not found in list");

	if (IsWindow(chld_hwnd))
		DestroyWindow(chld_hwnd);
	CloseHandle(chld_hwnd);
}

HWND child_window::get_hwnd() const
{
	return chld_hwnd;
}

void child_window::scale_position()
{
	// resize this window
	gui::Rectangle apr = get_rect();
	if (!MoveWindow(chld_hwnd, apr.x0, apr.y0, apr.width,
	apr.height, true))
		ERR2("MoveWindow", GetLastError());
	InvalidateRgn(chld_hwnd, NULL, false);

	// resize all childs
	for (auto chld : elements) {
		SendMessage(chld->get_hwnd(), WM_SIZE, 0, 0);
		InvalidateRgn(chld->get_hwnd(), NULL, false);
		UpdateWindow(chld->get_hwnd());
	}
}

/* find window by its handle in all_childs vec
 */
child_window* child_window::find_child_window(HWND hwin)
{
	auto it = all_childs.begin();
	while (it != all_childs.end()) {
		if ((*it)->get_hwnd() == hwin)
			break;
		++it;
	}
	if (it == all_childs.end())
		return NULL;

	return (*it);
}

LRESULT CALLBACK child_window::WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam)
{
	int result;
	switch (message) {
		case WM_CREATE:
			return 0;
			
		case WM_SIZE:
		{
			child_window* cwnd = find_child_window(hWnd);
			if (cwnd != NULL)
				cwnd->scale_position();

			return 0;
		}

		case WM_DESTROY:
			return 0;
	}	

	child_window* cwnd = find_child_window(hWnd);
	if (cwnd != NULL) {
		result = find_child_window(hWnd)->msg_handler(hWnd, message,
		wParam, lParam);
	} else
		result = DefWindowProc(hWnd, message, wParam, lParam);

	return result;
}


