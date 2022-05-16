#include "canvas.hpp"
#include "error.hpp"
#include "main.hpp"

#define CLASS_NAME "canvas"

using namespace gui;

#define PT_SIZE 6

template <class T>
static void free_vector(std::vector<T*> &vec)
{
	if (vec.size() != 0) {
		for (auto el : vec)
			delete el;
		vec.erase(vec.begin(), vec.end());
	}
}

static void draw_points(HDC dc, std::vector<point*> &vec, COLORREF col)
{
	HBRUSH hbst = CreateSolidBrush(col);
	HGDIOBJ hold = SelectObject(dc, hbst);

	for (auto pt : vec) {
		Ellipse(dc, pt->x-PT_SIZE/2, pt->y-PT_SIZE/2, pt->x + PT_SIZE,
		pt->y + PT_SIZE);
	}

	SelectObject(dc, hold);
	DeleteObject(hbst);
}

static void draw_lines(HDC dc, std::vector<line*> &vec, COLORREF col)
{
	HPEN hp = CreatePen(PS_SOLID, 4, col);
	HGDIOBJ hold = SelectObject(dc, hp);

	for (auto ln : vec) {
		MoveToEx(dc, ln->p1->x, ln->p1->y, NULL);
		LineTo(dc, ln->p2->x, ln->p2->y);
	}

	SelectObject(dc, hold);
	DeleteObject(hp);
}

int canvas::msg_handler(HWND hWnd, UINT message, WPARAM wParam, 
LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT lprc;

	switch (message) {

		case WM_PAINT:
		{
			HBRUSH obrush;
			HDC tempdc;
			BeginPaint(hWnd, &ps);
			tempdc = CreateCompatibleDC(ps.hdc);
			GetClientRect(hWnd, &lprc);
			HBITMAP dcbmp = CreateCompatibleBitmap(ps.hdc,
			lprc.right - lprc.left, lprc.bottom - lprc.top);
			SelectObject(tempdc, (HBITMAP)dcbmp);	

			FillRect(tempdc, &lprc, bg_brush);

			draw_points(tempdc, points, RGB(0, 0, 0));
			
			draw_lines(tempdc, lines, RGB(0, 0, 0));

			draw_lines(tempdc, hull_lines, RGB(0, 255, 0));

			// draw way start and finish point
			if (start != NULL) {
				HBRUSH hbst = CreateSolidBrush(RGB(0, 255, 0));
				SelectObject(tempdc, hbst);
				Ellipse(tempdc, start->x-PT_SIZE/2, start->y-PT_SIZE/2,
				start->x + PT_SIZE, start->y + PT_SIZE);

				RECT texr = { .left = start->x - 15, .top = start->y - 15,
				.right = start->x, .bottom = start->y };
				DrawText(tempdc, "S", 1, &texr, 0);

				DeleteObject(hbst);
			}

			if (finish != NULL) {
				HBRUSH hbfin = CreateSolidBrush(RGB(255, 0, 0));
				SelectObject(tempdc, hbfin);
				Ellipse(tempdc, finish->x-PT_SIZE/2, finish->y-PT_SIZE/2,
				finish->x + PT_SIZE, finish->y + PT_SIZE);

				RECT texr = { .left = finish->x - 15, .top = finish->y - 15,
				.right = finish->x, .bottom = finish->y };
				DrawText(tempdc, "T", 1, &texr, 0);

				DeleteObject(hbfin);
			}

			// draw line to cursor if needed
			if (lntc.drawn && last_point != NULL) {
				POINT cpos;
				// get cursor coordinates
				GetCursorPos(&cpos);
				ScreenToClient(hWnd, &cpos);
 
				HPEN erase = CreatePen(PS_SOLID, 5, bg_color);
				SelectObject(ps.hdc, erase);
				MoveToEx(tempdc, lntc.p1_old.x, lntc.p1_old.y, NULL);
				LineTo(tempdc, lntc.p2_old.x, lntc.p2_old.y);
				lntc.p1_old.x = last_point->x;
				lntc.p1_old.y = last_point->y;
				lntc.p2_old.x = cpos.x;
				lntc.p2_old.y = cpos.y;

 				HPEN Pen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
				SelectObject(tempdc, Pen);
                MoveToEx(tempdc, last_point->x, last_point->y, NULL);
                LineTo(tempdc, cpos.x, cpos.y);

				DeleteObject(Pen);
				DeleteObject(erase);
			}
			BitBlt(ps.hdc, 0, 0, lprc.right - lprc.left, lprc.bottom - lprc.top,
			tempdc, 0, 0, SRCCOPY);

			DeleteDC(tempdc);
			DeleteObject(dcbmp);
			EndPaint(hWnd, &ps);
		}

		case  WM_MOUSEMOVE:
		{
			RECT updRect;
			POINT cpos;
			if (lntc.drawn && last_point != NULL) {
				GetCursorPos(&cpos);
				ScreenToClient(hWnd, &cpos);

				InvalidateRect(hWnd, NULL, false);
				UpdateWindow(hWnd);
			}
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

canvas::canvas(HWND hwParent, Rectangle plRect, COLORREF bgcolor)
: child_window(hwParent, plRect), last_point(NULL), bg_color(bg_color),
start(NULL), finish(NULL)
{			
	WNDCLASSEX wc;
	HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE);
	HBRUSH bg_brush = CreateSolidBrush(bg_color);

	// absolute panel rectangle
	Rectangle apr = get_rect();
	
	if (!GetClassInfoExA(hInst, CLASS_NAME, (LPWNDCLASSEXA)&wc)) {
	
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInst;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor	= LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = bg_brush;
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

user_proc canvas::set_handler(user_proc new_proc)
{
	user_proc old = user_def_proc;
	user_def_proc = new_proc;
	return old;
}

point *canvas::add_point(POINT new_point)
{
	for (auto pt : points) {
		if (abs(pt->x - new_point.x) < 20 && abs(pt->y - new_point.y) < 20)
			return pt;
	}

	point *p = new point();
	p->x = new_point.x;
	p->y = new_point.y;
	points.push_back(p);
	last_point = p;
	update();

	return p;
}

point *canvas::add_line(point *pt1, point *pt2)
{
	if (pt1 == NULL || pt2 == NULL) {
		ERR("nullptr point");
		return pt1;
	}

	// check intersection
	for (auto ln : lines) {
		if (algos::intersection::IntersectPoint(*ln->p1, *ln->p2, *pt1, *pt2))
			continue;
		if (algos::intersection::doIntersect(*ln->p1, *ln->p2, *pt1, *pt2)) {
			remove_point(pt2);
			update();
			return pt1;
		}
	}

	line *newl = new line;
	newl->p1 = pt1;
	newl->p2 = pt2;
	pt1->nlines += 1;
	pt2->nlines += 1;
	lines.push_back(newl);
	update();
	return pt2;
}

void canvas::update()
{
	InvalidateRgn(chld_hwnd, NULL, false);
	UpdateWindow(chld_hwnd);
}

void canvas::remove_point(point *pt)
{
	std::vector<point*>::iterator it;
	for (it = points.begin(); it != points.end(); ++it) {
		if ((*it) == pt)
			break;
	}

	if (it != points.end() && (*it)->nlines == 0)
		points.erase(it);
}

void canvas::clear()
{
	free_vector<point>(points);
	free_vector<line>(lines);

	free_vector<point>(hull_points);
	free_vector<line>(hull_lines);
	
	lntc.drawn = false;
	last_point = NULL;
	update();
}

canvas::~canvas()
{
	for (auto ln : lines)
		delete ln;
	for (auto pt : points)
		delete pt;

	if (start != NULL)
		delete start;
	if (finish != NULL)
		delete finish;

	for (auto ln : hull_lines)
		delete ln;
	for (auto pt : hull_points)
		delete pt;

	DeleteObject(bg_brush);
}

void canvas::set_start(POINT pt)
{
	start = new point;
	start->x = pt.x;
	start->y = pt.y;
	update();
}

void canvas::set_finish(POINT pt)
{
	finish = new point;
	finish->x = pt.x;
	finish->y = pt.y;
	update();
}

void canvas::build_hull()
{
	if (start == NULL || finish == NULL) {
		MessageBoxA(NULL, "Set start and finish", "nag!", MB_APPLMODAL);
		return;
	}

	// delete old hull
	free_vector<point>(hull_points);
	free_vector<line>(hull_lines);

	// group all points to one array
	int pc = points.size() + 2;
	point *all_pts = new point[pc];
	for (int i = 0; i < pc-2; i++) {
		all_pts[i] = *points[i];
	}
	all_pts[pc-2] = *start;
	all_pts[pc-1] = *finish;

	// build convex hull around all points
	hull_points = algos::convex_hull::convexHull(all_pts, pc);
	
	for (int i = 1; i <= hull_points.size(); i++) {
		line *ln = new line();
		point *pt1 = hull_points[(i-1) % hull_points.size()];
		point *pt2 = hull_points[(i) % hull_points.size()];
		ln->p1 = pt1;
		ln->p2 = pt2;
		hull_lines.push_back(ln);
	}

	update();
	delete[] all_pts;
}