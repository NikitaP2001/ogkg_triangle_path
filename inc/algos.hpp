#include <ostream>
#include <set>

struct point {
    long nlines;
    long x;
    long y;

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
