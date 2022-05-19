#include <ostream>
#include <set>

struct line;

struct point {
    long x;
    long y;
    std::vector<line*> incindent;

    friend std::ostream &operator<<(std::ostream &o, const point &pt)
    {
        o << pt.x << " " << pt.y;
        return o;
    }

    bool operator==(point &other)
    {
        return this->x == other.x && this->y == other.y;
    }
};

struct comp_pt {
			bool operator()(point *lp, point *rp) const
			{
				if (lp->x == rp->x)
					return lp->y < rp->y;
				else
					return lp->x < rp->x;
            }
};

struct line {
    point *p1;
    point *p2;

    line(point *_p1, point *_p2)
    : p1(_p1), p2(_p2)
    {
        _p1->incindent.push_back(this);
        _p2->incindent.push_back(this);
    }

    ~line();
};

namespace algos {

namespace intersection {

    bool doIntersect(point p1, point q1, point p2, point q2);

    bool IntersectPoint(point p1, point q1, point p2, point q2);
}

namespace convex_hull {

    std::vector<point> convexHull(point points[], int n);
}

namespace regularization {

    std::vector<line*> adjust_to_regular(std::vector<line*> lines,
    std::set<point*, comp_pt> points);
}

} // ::algos
