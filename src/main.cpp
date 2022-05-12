#include <vector>
#include <windows.h>

#include "main.hpp"
#include "error.hpp"
#include "panel.hpp"
#include "button.hpp"

#ifdef DEBUG
std::fstream dbg_out("log.txt", std::ios::out);
#endif

// child windows, which wait resize msg
std::vector<HWND> send_resize;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("OGKG");
	HWND	hWnd;
	MSG		msg;
	WNDCLASS wc;
	
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor	= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszMenuName	= MAKEINTRESOURCE(IDM_MYMENURESOURCE);
	wc.lpszClassName = szAppName;
	
	if (!RegisterClass(&wc)) {
		int dwErr = GetLastError();
		ERR2("register class failed", dwErr);
	}
	
	if (!(hWnd = CreateWindow(szAppName,
		TEXT("OGKG"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL))) {
		int dwErr = GetLastError();
		PrintCSBackupAPIErrorMessage(dwErr);
	}

	// create and show toolbox
	gui::panel toolbox(hWnd,
		gui::Rectangle {
		.x_relative = true,
		.y_relative = false,
		.x0 = 0, 
		.y0 = 0,
		.width = 100,
		.height = 20,
		}, DKGRAY_BRUSH);	

	gui::Rectangle btn_rec = {
		.x_relative = false,
		.y_relative = false,
		.x0 = 3, 
		.y0 = 2,
		.width = 16,
		.height = 16,
	};
	toolbox.add_button(btn_rec);
	btn_rec.x0 += btn_rec.width + 3;
	toolbox.add_button(btn_rec);
	btn_rec.x0 += btn_rec.width + 3;
	toolbox.add_button(btn_rec);
	btn_rec.x0 += btn_rec.width + 3;
	toolbox.add_button(btn_rec);
	btn_rec.x0 += btn_rec.width + 3;
	toolbox.add_button(btn_rec);
	
	send_resize.push_back(toolbox.get_hwnd());

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)) {

		DispatchMessage(&msg);
	}
		
	return msg.wParam;
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	
	switch (message)
	{
	case WM_CREATE:		
		return 0;
		
	case WM_COMMAND:
		switch(LOWORD(wParam)) { 
		
                case IDM_SAVE:
					MessageBoxPrintf(TEXT("Menu"), TEXT("Not yet"));
					break;
				case IDM_LOAD:
					MessageBoxPrintf(TEXT("Menu"), TEXT("Not yet"));
					break;				
        }             
        return 0; 

	case WM_SIZE:
		for (auto ch_hw : send_resize) {
			SendMessage(ch_hw, WM_SIZE, 0, 0);
			InvalidateRgn(ch_hw, NULL, false);
			UpdateWindow(ch_hw);

		}
		return 0;
		
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);	
		DrawText(hdc, TEXT("Oh shit"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hWnd, &ps);
		return 0;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}