#include <ostream>

struct point {
    long nlines;
    long x;
    long y;

    friend std::ostream &operator<<(std::ostream &o, const point &pt)
    {
        o << pt.x << " " << pt.y;
        return o;
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

    std::vector<point*> convexHull(point points[], int n);
}

} // ::algos
