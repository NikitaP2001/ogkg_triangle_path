#include <windows.h>

#include "main.hpp"
#include "child_window.hpp"
#include "error.hpp"

using namespace gui;

/* get rect of panel in coords, relative
 * to the parent window
 */
gui::Rectangle child_window::get_rect()
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

HWND child_window::get_hwnd()
{
	return chld_hwnd;
}

