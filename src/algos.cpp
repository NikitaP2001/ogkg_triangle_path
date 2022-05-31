#include <algorithm>
#include <stack>
#include <vector>
#include <cmath>
#include "algos.hpp"
#include "error.h"
#include "main.hpp"

static inline bool eqPoint(point p1, point p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

/* To find orientation of ordered triplet (p, q, r).
 *   The function returns following values
 *   0 --> p, q and r are collinear
 *   1 --> Clockwise
 *   2 --> Counterclockwise */
int orientation(point p, point q, point r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
            (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // collinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}

line::~line()
{
    auto &vec1 = p1->incindent;
    auto &vec2 = p2->incindent;
    vec1.erase(std::remove(vec1.begin(), vec1.end(), this), vec1.end());
    vec2.erase(std::remove(vec2.begin(), vec2.end(), this), vec2.end());
}

namespace algos {

namespace intersection {

    static bool onSegment(point p, point q, point r)
    {
        if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
            q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return true;
    
        return false;
    }
    
    bool doIntersect(point p1, point q1, point p2, point q2)
    {
        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);
    
        if (o1 != o2 && o3 != o4)
            return true;
    
        if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    
        // p1, q1 and q2 are collinear and q2 lies on segment p1q1
        if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    
        // p2, q2 and p1 are collinear and p1 lies on segment p2q2
        if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    
        // p2, q2 and q1 are collinear and q1 lies on segment p2q2
        if (o4 == 0 && onSegment(p2, q1, q2)) return true;
    
        return false; // Doesn't fall in any of the above cases
    }

    bool IntersectPoint(point p1, point q1, point p2, point q2)
    {
        return eqPoint(p1, p2) || eqPoint(p1, q2) || eqPoint(q1, p2)
        || eqPoint(q1, q2);
    }
} // ::intersection

namespace convex_hull {

    // A global point needed for  sorting points with reference
    // to  the first point Used in compare function of qsort()
    point p0;
    
    // A utility function to find next to top in a stack
    point nextToTop(std::stack<point> &S)
    {
        point p = S.top();
        S.pop();
        point res = S.top();
        S.push(p);
        return res;
    }
    
    // A utility function to swap two points
    void swap(point &p1, point &p2)
    {
        point temp = p1;
        p1 = p2;
        p2 = temp;
    }
    
    /* A utility function to return square of distance
    * between p1 and p2 */
    int distSq(point p1, point p2)
    {
        return (p1.x - p2.x)*(p1.x - p2.x) +
            (p1.y - p2.y)*(p1.y - p2.y);
    }

    /* A function used by library function qsort() to sort an array of
    * points with respect to the first point */
    int compare(const void *vp1, const void *vp2)
    {
        point *p1 = (point *)vp1;
        point *p2 = (point *)vp2;
        
        // Find orientation
        int o = orientation(p0, *p1, *p2);
        if (o == 0)
            return (distSq(p0, *p2) >= distSq(p0, *p1))? -1 : 1;
        
        return (o == 2)? -1: 1;
    }

    // Graham scan
    std::vector<point> convexHull(point points[], int n)
    {
        std::vector<point> result;
        // Find the bottommost point
        int ymin = points[0].y, min = 0;
        for (int i = 1; i < n; i++)
        {
            int y = points[i].y;
        
            // Pick the bottom-most or chose the left
            // most point in case of tie
            if ((y < ymin) || (ymin == y &&
                points[i].x < points[min].x))
                ymin = points[i].y, min = i;
        }
        
        // Place the bottom-most point at first position
        swap(points[0], points[min]);
        
        // Sort n-1 points with respect to the first point.
        // A point p1 comes before p2 in sorted output if p2
        // has larger polar angle (in counterclockwise
        // direction) than p1
        p0 = points[0];
        qsort(&points[1], n-1, sizeof(point), compare);
        
        // If two or more points make same angle with p0,
        // Remove all but the one that is farthest from p0
        // Remember that, in above sorting, our criteria was
        // to keep the farthest point at the end when more than
        // one points have same angle.
        int m = 1; // Initialize size of modified array
        for (int i=1; i<n; i++)
        {
            // Keep removing i while angle of i and i+1 is same
            // with respect to p0
            while (i < n-1 && orientation(p0, points[i],
                                            points[i+1]) == 0)
                i++;
        
        
            points[m] = points[i];
            m++;  // Update size of modified array
        }
        
        // If modified array of points has less than 3 points,
        // convex hull is not possible
        if (m < 3)
        return result;
        
        // Create an empty stack and push first three points
        // to it.
        std::stack<point> S;
        S.push(points[0]);
        S.push(points[1]);
        S.push(points[2]);
        
        // Process remaining n-3 points
        for (int i = 3; i < m; i++)
        {
            // Keep removing top while the angle formed by
            // points next-to-top, top, and points[i] makes
            // a non-left turn
            while (S.size()>1 && orientation(nextToTop(S), S.top(), points[i]) != 2)
                S.pop();
            S.push(points[i]);
        }
        
        // Now stack has the output points, print contents of stack
        while (!S.empty())
        {
            point p = S.top();
            result.push_back(p);
            S.pop();
        }

        return result;
    }

    
} // ::convex_hull

namespace regularization {
	
	struct sector {
		std::vector<point*> left_border;
		std::vector<line*> status;
    };
	
	// go down flat sweep
	void top_to_down(std::vector<line*> &res, std::vector<sector> &sectors)
	{
		// top to down regularize
        auto it = sectors.rbegin();
        ++it;
        for ( ; it != sectors.rend(); ++it) {

            // for each point on sector border
            for (auto pt : (*it).left_border) {
                bool is_regular = false;

                for (auto ln : (*it).status) {
                    // have outgoing lines
                    if (ln->p1 == pt || ln->p2 == pt) {
                        is_regular = true;
                        break;
                    }
                }

                auto it_to_end = std::prev(it);
                auto oite = it;
                line *new_line = NULL;				
                // make regular, go up, searching where 
				// to inset line
                while (!is_regular) {
                    // try to add new line for each pt in seg
                    for (auto prevpt : (*it_to_end).left_border) {
						bool intersect = false;
						// will new line intercept any line in seg
                        for (auto pln : (*oite).status) {							
                            if (intersection::doIntersect(*pln->p1, *pln->p2,
                            *pt, *prevpt)
							&& !intersection::IntersectPoint(*pln->p1, *pln->p2,
                            *pt, *prevpt))
                                intersect = true;															
                        }
						// found where to place new line
						if (!intersect) {
							new_line = new line(pt, prevpt);
                            new_line->is_temp = true;
							is_regular = true;
							break;
						}
                    }
                    
                    if (it_to_end == sectors.rbegin()) {
						if (!is_regular)
							INFO("not found");
                        break;
                    } else {
                        oite = it_to_end;
                        it_to_end = std::prev(it_to_end); 
                    } 

                }

                // update status
                if (new_line != NULL) {
                    res.push_back(new_line);
                    do {
                        (*oite).status.push_back(new_line);
                    } while(oite++ != it);
                }

            }
        }
	}
	
	// go up flat sweep
	void down_to_top(std::vector<line*> &res, std::vector<sector> &sectors)
	{
		 // down to top regularization
        auto rit = sectors.begin();
        auto prit = rit;
        ++rit;
        for ( ; rit != sectors.end(); prit = rit, ++rit) {

            // for each point on sector border
            for (auto pt : (*rit).left_border) {
                bool is_regular = false;

                for (auto ln : (*prit).status) {
                    // have ingoing lines
                    if (ln->p1 == pt || ln->p2 == pt) {
                        is_regular = true;
                        break;
                    }
                }

                auto it_to_beg = prit;
                line *new_line = NULL;				
				
				// go down, searching where to
				// insert line
                while (!is_regular) {
                    // try insert new line pt - prevpt
                    for (auto prevpt : (*it_to_beg).left_border) {
						bool intersect = false;
						// seek for intercepribg lines
                        for (auto pln : (*it_to_beg).status) {
                            if (intersection::doIntersect(*pln->p1, *pln->p2,
                            *pt, *prevpt)
							&& !intersection::IntersectPoint(*pln->p1, *pln->p2,
                            *pt, *prevpt))
                                intersect = true;
                        }
						// its possible to insert
						if (!intersect) {
							new_line = new line(pt, prevpt);
                            new_line->is_temp = true;
							is_regular = true;
							break;
						}
                    }

                    if (it_to_beg == sectors.begin()) {
						if (!is_regular)
							INFO("not found");
                        break;
                    } else
                        it_to_beg = std::prev(it_to_beg); 
                }

                // update status
                if (new_line != NULL) {
                    res.push_back(new_line);
                    while (it_to_beg != rit) {
                        (*it_to_beg).status.push_back(new_line);
                        ++it_to_beg;
                    }
                }

            }
        }
	}

    // add new lines to make regular by Ox
    std::vector<line*> adjust_to_regular(std::vector<line*> lines,
    std::set<point*, comp_pt> points)
    {
        std::vector<line*> res;
        std::vector<sector> sectors;
        std::vector<line*> not_finished;
        // monotonic interval scheduling by Ox
        for (auto it = points.rbegin(); it != points.rend(); ++it)
        {
            // if in prev sector, add to its points
            sector &prev = sectors.front();
            if (sectors.size() > 0 && prev.left_border.front()->x == (*it)->x) {
                prev.left_border.push_back(*it);
                continue;
            } else {
                sectors.insert(sectors.begin(), sector());
                sectors.front().left_border.push_back(*it);
            }

            sector *sec = &sectors.front();

            // add lines from prev seg
            for (auto ln : not_finished) {
                sec->status.push_back(ln);
            }
            // remove edges finished on this sector
            not_finished.erase(std::remove_if(not_finished.begin(),
            not_finished.end(), [&it](line* ln){
                return (ln->p1 == (*it) || ln->p2 == (*it));
            }), not_finished.end());

            // add new lines in status
            for (auto ln : (*it)->incindent) {
                if (ln->p1 == (*it)) {
                    if (ln->p2->x >= (*it)->x)
                        sec->status.push_back(ln);
                    else 
                        not_finished.push_back(ln);
                } else if (ln->p2 == (*it)) {
                    if (ln->p1->x >= (*it)->x)
                        sec->status.push_back(ln);
                    else
                        not_finished.push_back(ln);
                }
            }
        }

        top_to_down(res, sectors);

		down_to_top(res, sectors);

        return res;
    }


} // ::regularization

namespace monotone_chains {

    /* A utility function to return square of distance
    * between p1 and p2 */
    int distSq(point p1, point p2)
    {
        return (p1.x - p2.x)*(p1.x - p2.x) +
            (p1.y - p2.y)*(p1.y - p2.y);
    }

    /* A function used by library function qsort() to sort an array of
    * points with respect to the first point */
    int compare(point *p0, point *p1, point *p2)
    {
        // Find orientation
        int o = orientation(*p0, *p1, *p2);
        if (o == 0)
            return (distSq(*p0, *p2) >= distSq(*p0, *p1))? -1 : 1;
        
        return (o == 2)? -1: 1;
    }

    void weight_balance(std::vector<line*> &lines,
    std::set<point*, comp_pt> &points)
    {
        // initilize edges with 1 weight 
        for (auto ln : lines) {
            if (ln->p1->y == ln->p2->y)
                ln->weight = 0;
            else
                ln->weight = 1;
        }

        // forward pass
        auto it = points.begin();
        for (it++; it != points.end(); it++) {
            long Win = 0, Wout = 0;
            line *d;
            // for sorting lines clockwise
            auto comp = [&it](line *l1, line *l2)
            { 
                point *pfar1 = ((*it) != l1->p1) ? l1->p1 : l1->p2;
                point *pfar2 = ((*it) != l2->p1) ? l2->p1 : l2->p2;
                return (compare((*it), pfar1, pfar2) == 1); 
            };
            auto lout  = std::set<line*,decltype(comp)>(comp);

            for (auto ln : (*it)->incindent) {
                point *pfar = ((*it) != ln->p1) ? ln->p1 : ln->p2;
                // is outgoing
                if (pfar->x > (*it)->x) {
                    lout.insert(ln);
                    Wout += ln->weight;
                // is ingoing
                } else if (pfar->x < (*it)->x)
                    Win += ln->weight;
            }
            // case border vertex
            if (lout.size() == 0)
                continue;
            // get leftwise from sorted set
            d = *lout.rbegin();

            if (Win > Wout)
                d->weight = Win - Wout + 1;
        }

        // backward pass
        auto rit = points.rbegin();
        for (rit++; rit != points.rend(); rit++) {
            long Win = 0, Wout = 0;
            line *d;
            // for sorting lines clockwise
            auto comp = [&rit](line *l1, line *l2)
            { 
                point *pfar1 = ((*rit) != l1->p1) ? l1->p1 : l1->p2;
                point *pfar2 = ((*rit) != l2->p1) ? l2->p1 : l2->p2;
                return (compare((*rit), pfar1, pfar2) == 1); 
            };
            auto lin  = std::set<line*,decltype(comp)>(comp);
            
            for (auto ln : (*rit)->incindent) {
                point *pfar = ((*rit) != ln->p1) ? ln->p1 : ln->p2;
                // is ingoing
                if (pfar->x < (*rit)->x) {
                    lin.insert(ln);
                    Win += ln->weight;
                // is outgoing
                } else if (pfar->x > (*rit)->x)
                    Wout += ln->weight;
            }
            // case border vertex
            if (lin.size() == 0)
                continue;
            // get leftwise from sorted set
            d = *lin.begin();

            if (Wout > Win)
                d->weight = Wout - Win + d->weight;
        }

    }

    std::vector<std::vector<point*>> build_chain(std::vector<line*> &lines,
    std::set<point*, comp_pt> &points)
    {
        weight_balance(lines, points);

        std::vector<std::vector<point*>> chains;
        point* pt_start = *points.begin();
        point* currp = pt_start;

        // sort incendent lines clockwise
        auto comp_s = [&pt_start](line *l1, line *l2)
        { 
            point *pfar1 = (pt_start != l1->p1) ? l1->p1 : l1->p2;
            point *pfar2 = (pt_start != l2->p1) ? l2->p1 : l2->p2;
            return (compare(pt_start, pfar1, pfar2) == 1); 
        };
        auto linc_start  = std::set<line*,decltype(comp_s)>(comp_s);
        for (auto ln : pt_start->incindent)
            linc_start.insert(ln);

        // for each line incendent to start
        for (auto lsit = linc_start.rbegin(); lsit != linc_start.rend(); lsit++) {
            line *ln = *lsit;

            while (ln->weight > 0) {
                std::vector<point*> chain;
                currp = pt_start;
                line *curl = ln;

                // go up by lines, forming chain
                do {
                    curl->weight -= 1;
                    chain.push_back(currp);
                    currp = (currp != curl->p1) ? curl->p1 : curl->p2;

                    auto comp_c = [&currp](line *l1, line *l2)
                    { 
                        point *pfar1 = (currp != l1->p1) ? l1->p1 : l1->p2;
                        point *pfar2 = (currp != l2->p1) ? l2->p1 : l2->p2;
                        return (compare(currp, pfar1, pfar2) == 1); 
                    };
                    auto linc = std::set<line*,decltype(comp_c)>(comp_c);
                    for (auto cpinc : currp->incindent) {
                        point *pfar = (currp != cpinc->p1)
                        ? cpinc->p1 : cpinc->p2;
                        if (currp->x < pfar->x)
                            linc.insert(cpinc);
                    }

                    auto lincit = linc.rbegin();
                    while (lincit != linc.rend() && (*lincit)->weight == 0)
                        lincit++;
                    if (lincit != linc.rend())
                        curl = *lincit;
                } while (currp != *points.rbegin());
                chain.push_back(currp);
                chains.push_back(chain);
            }
        }

        return chains;
    }

} // ::monotone_chains

namespace triangulation {

    // expects monotone poligon sorted by x increasment
    std::vector<line*> triang_poligon(std::vector<point*> poligon)
    {
        std::vector<point*> s;
        std::vector<line*> triangulation;
        for (int i = 0; i < poligon.size(); i++) {
            point *v = poligon[i];

            if (s.size() < 2) {
                s.push_back(v);
                continue;
            }

            int t = s.size() - 1;
            if (std::count_if(v->incindent.begin(), v->incindent.end(),
            [&s, t](line *ln) { return *ln->p1 == *s[t] || *ln->p2 == *s[t];})
            && !std::count_if(v->incindent.begin(), v->incindent.end(),
            [&s](line *ln) { return *ln->p1 == *s[0] || *ln->p2 == *s[0];})) {
                int sign;
                do {
                    std::vector<long> v1 = { s[t-1]->x - s[t]->x,
                    s[t]->y - s[t-1]->y};
                    std::vector<long> v2 = { v->x - s[t]->x,
                    (s[t]->y - v->y) };
                    long sign = v1[0] * v2[1] - v1[1] * v2[0];
                    if (!v->upper_chain)
                        sign = -sign;
                    if (sign > 0) {
                        line *nl = new line(v, s[t - 1]);
                        nl->is_temp = true;
                        triangulation.push_back(nl);
                        s.pop_back();
                    }
                } while (--t != 0 && sign > 0);
                s.push_back(v);
            } else if (std::count_if(v->incindent.begin(), v->incindent.end(),
            [&s](line *ln) { return *ln->p1 == *s[0] || *ln->p2 == *s[0];})
            && !std::count_if(v->incindent.begin(), v->incindent.end(),
            [&s, t](line *ln) { return *ln->p1 == *s[t] || *ln->p2 == *s[t];})) {
                while (s.size() > 1) {
                    line *nl = new line(v, *(s.begin()+1));
                    nl->is_temp = true;
                    triangulation.push_back(nl);
                    s.erase(s.begin());
                }
                s.push_back(v);
            } else {
                while (s.size() > 2) {
                    line *nl = new line(v, *(s.begin()+1));
                    nl->is_temp = true;
                    triangulation.push_back(nl);
                    s.erase(s.begin());
                }
                break;
            }
        }

        return triangulation;
    }

    // and new lines to triangulate chains
    // note: returned lines are temporary and must be freed with del
    std::vector<line*> triangulate_chains(std::vector<std::vector<point*>> &chains)
    {
        std::vector<line*> triangulation;
        for (auto leftCh = chains.begin(), rightCh = chains.begin() + 1;
        rightCh != chains.end(); leftCh++, rightCh++) {
            std::vector<point*> poligon;
            auto leftpts = (*leftCh).begin();
            auto rightpts = (*rightCh).begin();

            // fill poligon with sorted points from border chains
            while (leftpts != (*leftCh).end() && rightpts != (*rightCh).end()) {
                // first or last point of poligon
                if (**leftpts == **rightpts) {
                    if (poligon.size() > 1) {
                        // end of chain
                        (*leftpts)->upper_chain = true;
                        poligon.push_back(*leftpts);
                        std::vector<line*> newVer = triang_poligon(poligon);
                        triangulation.insert(triangulation.end(),
                        newVer.begin(), newVer.end());
                        poligon.erase(poligon.begin(), poligon.end());
                    } else {
                        
                        if (poligon.size() == 1)
                            poligon.erase(poligon.begin(), poligon.end());

                        // begin of new chain
                        (*leftpts)->upper_chain = false;
                        poligon.push_back(*leftpts);
                    }
                    leftpts++;
                    rightpts++;
                } else {
                    if ((*leftpts)->x < (*rightpts)->x) {
                        (*leftpts)->upper_chain = true;
                        poligon.push_back(*leftpts);
                        leftpts++;
                    } else {
                        (*leftpts)->upper_chain = false;
                        poligon.push_back(*rightpts);
                        rightpts++;
                    }
                }
            }
            if (poligon.size() > 3) {
                std::vector<line*> newVer = triang_poligon(poligon);
                triangulation.insert(triangulation.end(),
                newVer.begin(), newVer.end());
            }
        }

        return triangulation;
    }

} // ::triangulation 

namespace way {

    #define INF -1
    #define BLOCK -2

    /* A utility function to return square of distance
    * between p1 and p2 */
    int distSq(point p1, point p2)
    {
        return (p1.x - p2.x)*(p1.x - p2.x) +
            (p1.y - p2.y)*(p1.y - p2.y);
    }

    /* A function used by library function qsort() to sort an array of
    * points with respect to the first point */
    int compare(point *p0, point *p1, point *p2)
    {
        // Find orientation
        int o = orientation(*p0, *p1, *p2);
        if (o == 0)
            return (distSq(*p0, *p2) >= distSq(*p0, *p1))? -1 : 1;
        
        return (o == 2)? -1: 1;
    }

    long pdist(point *p1, point *p2)
    {
        return sqrt((p1->x - p2->x) * (p1->x - p2->x)
        + (p1->y - p2->y) * (p1->y - p2->y));
    }

    struct sway {
        std::vector<point*> way;
        long cost;
    };

    // right - on which side of line we arrived here
    sway roll_in(point *prev, point *finish, line *way, bool right)
    {
        sway minway;
        minway.cost = INFINITE;
        point *pcur = (*prev == *way->p1) ? way->p2 : way->p1;
        // owerwrite current line cost
        pcur->way_len = prev->way_len + pdist(pcur, prev);

        // check arrive at finish
        if (pcur == finish) {
            minway.cost = pcur->way_len;
            minway.way.push_back(pcur);
            return minway;
        }

        // sort incindent clockwise
        auto comp_c = [pcur](line *l1, line *l2)
        { 
            point *pfar1 = (pcur != l1->p1) ? l1->p1 : l1->p2;
            point *pfar2 = (pcur != l2->p1) ? l2->p1 : l2->p2;
            return (compare(pcur, pfar1, pfar2) == 1); 
        };
        auto linc = std::set<line*,decltype(comp_c)>(comp_c);
        for (auto cpinc : pcur->incindent)
            linc.insert(cpinc);

        auto it = linc.find(way);
        // try to find non temp line to start scan
        if (way->is_temp) {
            right = true;
            do {
                if (it == linc.begin())
                    it = linc.end();
                it--;
                // unfortunately we did not find non-temp line
                if (*it == way)
                    break;

            } while ((*it)->is_temp);
        }
        way = *it;

        // scan clockwise
        if (!right) {
            do {
                if (it == linc.begin())
                    it = linc.end();
                it--;
                if (*it == way)
                    break;
                point *pfar = (pcur != (*it)->p1) ? (*it)->p1 : (*it)->p2;

                // is sense to roll in *it line
                if (pfar->way_len != BLOCK && (pfar->way_len == INFINITE
                || pcur->way_len + pdist(pcur, pfar) < pfar->way_len)) {
                    sway wres = roll_in(pcur, finish, *it, false);
                    if (wres.cost != BLOCK
                    && (minway.cost == INF || wres.cost < minway.cost))
                        minway = wres;
                }

            } while ((*it)->is_temp);

        // scan non-clockwise
        } else {
            do {
                it++;
                if (it == linc.end())
                    it = linc.begin();

                if (*it == way)
                    break;
                point *pfar = (pcur != (*it)->p1) ? (*it)->p1 : (*it)->p2;

                if (pfar->way_len != BLOCK && (pfar->way_len == INFINITE
                || pcur->way_len + pdist(pcur, pfar) < pfar->way_len)) {
                    sway wres = roll_in(pcur, finish, *it, true);
                    if (wres.cost != BLOCK
                    && (minway.cost == INF || wres.cost < minway.cost))
                        minway = wres;
                }
            } while ((*it)->is_temp);

        }

        // route to target found
        if (minway.cost != INFINITE) {
            minway.cost += pcur->way_len;
            minway.way.push_back(pcur);
        // not found
        } else {
            pcur->way_len = BLOCK;
            minway.cost = BLOCK;
        }

        return minway;
    }

    
    std::vector<point*> build_way(std::set<point*, comp_pt> all_pts,
    point *start, point *finish)
    {
        sway minway;
        minway.cost = INFINITE;

        for (auto pt : all_pts)
            pt->way_len = INFINITE;
        start->way_len = 0;

        for (auto ln : start->incindent) {
            sway neww = roll_in(start, finish, ln, true);
            if (neww.cost != BLOCK
            && (minway.cost == INF || neww.cost < minway.cost))
                minway = neww;
        }

        if (minway.cost == INFINITE)
            throw std::logic_error("no route found");

        minway.way.push_back(start);
        return minway.way;
    }

} // ::way

} // ::algos
