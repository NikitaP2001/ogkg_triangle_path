#include <vector>
#include <windows.h>

#include "main.hpp"
#include "error.hpp"
#include "logic.hpp"

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

	gui::panel *toolbox = create_toolbox(hWnd);
	send_resize.push_back(toolbox->get_hwnd());

	gui::canvas *mcanvas = create_canvas(hWnd);
	send_resize.push_back(mcanvas->get_hwnd());

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)) {

		DispatchMessage(&msg);
	}

	delete toolbox;
		
	return msg.wParam;
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	
	switch (message)
	{
	case WM_CREATE:		
	{
		HINSTANCE hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		HICON hIcon = LoadIcon (hInstance, MAKEINTRESOURCE (IDI_ICON));
		if (hIcon == NULL) {
			ERR2("icon load failed", GetLastError());
		} else 
			SendMessage (hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		return 0;
	}
		
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
		
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}