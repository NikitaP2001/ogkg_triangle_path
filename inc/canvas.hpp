#ifndef CANVAS_HPP
#define CANVAS_HPP
#include "child_window.hpp"
#include "algos.hpp"
#include <vector>
#include <set>


namespace gui {

	struct line_to_cursor {
		bool drawn;
		POINT p1_old;
		POINT p2_old;
	};

    class canvas : public child_window {
		user_proc user_def_proc;

		std::set<point*, comp_pt> points;

		std::vector<line*> lines;
		std::vector<line*> temp_lines;

		
		point *start;
		point *finish;
		COLORREF bg_color;
		HBRUSH bg_brush;

		virtual int msg_handler(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

	public:
		point* last_point;
		line_to_cursor lntc;
		canvas(HWND hwParent, Rectangle plRec, COLORREF bgcolor);

		void remove_point(point *pt);

		point *add_point(POINT new_point);

		point *add_line(point *pt1, point *pt2);

		void set_start(POINT pt);

		void set_finish(POINT pt);

		void clear();

		void update();

		void build_hull();

		user_proc set_handler(user_proc new_proc);

		virtual ~canvas();

    };
}

#endif // CANVAS_HPP