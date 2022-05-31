#include <ostream>
#include <set>

struct line;

struct point {
    long x;
    long y;

    bool upper_chain;
    long way_len;

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
    long weight; // need for monotonic chain algorithm
    bool is_temp;

    line(point *_p1, point *_p2)
    : p1(_p1), p2(_p2), is_temp(false)
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

namespace monotone_chains {

    std::vector<std::vector<point*>> build_chain(std::vector<line*> &lines,
    std::set<point*, comp_pt> &points);

}

namespace triangulation {

    // and new lines to triangulate chains
    // note: returned lines are temporary and must be freed with del
    std::vector<line*> triangulate_chains(std::vector<std::vector<point*>> &chains);

}

namespace way {

    std::vector<point*> build_way(std::set<point*, comp_pt> all_pts,
    point *start, point *finish);

}

} // ::algos
