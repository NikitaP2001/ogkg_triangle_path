#include "logic.hpp"
#include "main.hpp"
#include "error.hpp"

static int btn1proc(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam);

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
	toolbox->add_button(btn_rec, btn1proc, IDI_BUTTON_PENCIL);
	btn_rec.x0 += btn_rec.width + 3;
	toolbox->add_button(btn_rec, NULL);
	btn_rec.x0 += btn_rec.width + 3;
	toolbox->add_button(btn_rec, NULL);
	btn_rec.x0 += btn_rec.width + 3;
	toolbox->add_button(btn_rec, NULL);
	btn_rec.x0 += btn_rec.width + 3;
	toolbox->add_button(btn_rec, NULL);

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
		}, LTGRAY_BRUSH);	

	return mcanvas;
}


static int btn1proc(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_LBUTTONDOWN:
			MessageBoxA(0, "Invoked", "hmm", 0);
			return 0;
	}

	return 1;
}

