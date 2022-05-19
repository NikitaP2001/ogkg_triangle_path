#include "logic.hpp"
#include "main.hpp"
#include "error.hpp"

static int btn_paint(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam);
static int canvasproc(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam);
static int btn_clear(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam);
static int btn_start(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam);
static int btn_finish(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam);
static int btn_begin(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam);

gui::panel *toolbox;
gui::canvas *mcanvas;

#define BUTTON_WIDTH 20
#define BUTTON_HEIGHT 20

gui::panel *create_toolbox(HWND hWnd)
{
	// create and show toolbox
	toolbox = new gui::panel(hWnd,
		gui::Rectangle {
		.x_relative = true,
		.x0 = 0, 
		.y_relative = false,
		.y0 = 0,
        .w_relative = true,
		.width = 100,
        .h_relative = false,
		.height = BUTTON_HEIGHT + 4,
		.use_reverse_x1 = false,
		.use_reverse_y1 = false,
		}, DKGRAY_BRUSH);	

	gui::Rectangle btn_rec = {
		.x_relative = false,
		.x0 = 3, 
		.y_relative = false,
		.y0 = 2,
		.w_relative = false,
		.width = BUTTON_WIDTH,
		.h_relative = false,
		.height = BUTTON_HEIGHT,
		.use_reverse_x1 = false,
		.use_reverse_y1 = false,
	};
	toolbox->add_button(btn_rec, btn_paint, IDI_BUTTON_PENCIL);
	btn_rec.x0 += btn_rec.width + 3;
	toolbox->add_button(btn_rec, btn_start, IDI_BUTTON_START);
	btn_rec.x0 += btn_rec.width + 3;
	toolbox->add_button(btn_rec, btn_finish, IDI_BUTTON_FINISH);
	btn_rec.x0 += btn_rec.width + 3;
	toolbox->add_button(btn_rec, btn_begin, IDI_BUTTON_BEGIN);
	btn_rec.x0 += btn_rec.width + 3;
	toolbox->add_button(btn_rec, btn_clear, IDI_BUTTON_CLEAR);

	return toolbox;
}

gui::canvas *create_canvas(HWND hWnd)
{
	mcanvas = new gui::canvas(hWnd,
		gui::Rectangle {
		.x_relative = false,
		.x0 = 5,
		.y_relative = false,
		.y0 = BUTTON_HEIGHT + 9,
        .use_reverse_x1 = true,
		.x1 = 20,
		.use_reverse_y1 = true,
		.y1 = 65,
		}, RGB(211, 211, 211));	

	mcanvas->set_handler(canvasproc);
	mcanvas->lntc.drawn = false;

	return mcanvas;
}

bool is_drawing = false;
bool is_setstart = false;
bool is_setfinish = false;

point *last_point = NULL;

static int canvasproc(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_LBUTTONDOWN:
			POINT pt;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);

			if (is_drawing) {
				
				point *new_p = mcanvas->add_point(pt);
				if (last_point != new_p && last_point != NULL) {
					new_p = mcanvas->add_line(last_point, new_p);
				}
				mcanvas->last_point = new_p;
				last_point = new_p;
				mcanvas->lntc.drawn = true;
			} else if (is_setstart) {

				mcanvas->set_start(pt);
				is_setstart = false;

			} else if (is_setfinish) {

				mcanvas->set_finish(pt);
				is_setfinish = false;
			}
			return 0;

		case WM_RBUTTONDOWN:
			last_point = NULL;
			mcanvas->lntc.drawn = false;
			mcanvas->update();
			return 0;
	}

	return 1;
}

static int btn_paint(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_LBUTTONDOWN:
			is_drawing ^= 1;
			return 0;
	}

	return 1;
}

static int btn_clear(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_LBUTTONDOWN:
		{
			is_drawing = false;
			last_point = NULL;
			mcanvas->clear();
			return 0;
		}
	}
	return 1;
}

static int btn_start(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_LBUTTONDOWN:
		{
			mcanvas->last_point = NULL;
			mcanvas->lntc.drawn = false;
			mcanvas->update();
			is_drawing = false;
			last_point = NULL;
			is_setstart = true;
			return 0;
		}
	}
	return 1;
}

static int btn_finish(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_LBUTTONDOWN:
		{	
			mcanvas->last_point = NULL;
			mcanvas->lntc.drawn = false;
			mcanvas->update();
			is_drawing = false;
			last_point = NULL;
			is_setfinish = true;
			return 0;
		}
	}
	return 1;
}

static int btn_begin(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_LBUTTONDOWN:
		{
			mcanvas->last_point = NULL;
			mcanvas->lntc.drawn = false;
			mcanvas->update();
			is_drawing = false;
			last_point = NULL;

			mcanvas->find_path();
			return 0;
		}
	}
	return 1;
}
