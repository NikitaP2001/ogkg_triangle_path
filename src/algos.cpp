#include <algorithm>
#include <stack>
#include <vector>
#include "algos.hpp"
#include "error.h"
#include "main.hpp"

static inline bool eqPoint(point p1, point p2)
{
    return p1.x == p2.x && p1.y == p2.y;
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
    
    static int orientation(point p, point q, point r)
    {
        int val = (q.y - p.y) * (r.x - q.x) -
                (q.x - p.x) * (r.y - q.y);
    
        if (val == 0) return 0; 
    
        return (val > 0)? 1: 2; 
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
    
    // A utility function to return square of distance
    // between p1 and p2
    int distSq(point p1, point p2)
    {
        return (p1.x - p2.x)*(p1.x - p2.x) +
            (p1.y - p2.y)*(p1.y - p2.y);
    }
    
    // To find orientation of ordered triplet (p, q, r).
    // The function returns following values
    // 0 --> p, q and r are collinear
    // 1 --> Clockwise
    // 2 --> Counterclockwise
    int orientation(point p, point q, point r)
    {
        int val = (q.y - p.y) * (r.x - q.x) -
                (q.x - p.x) * (r.y - q.y);
    
        if (val == 0) return 0;  // collinear
        return (val > 0)? 1: 2; // clock or counterclock wise
    }
    
    // A function used by library function qsort() to sort an array of
    // points with respect to the first point
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
							new_line = new line;
							new_line->p1 = pt;
							new_line->p2 = prevpt;
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
							new_line = new line;
							new_line->p1 = pt;
							new_line->p2 = prevpt;
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
        // monotonic interval scheduling by Ox
        for (auto it = points.rbegin(); it != points.rend(); ++it)
        {
            // check if this point already in sector
            if (sectors.size() > 0) {
                sector &prev = sectors.back();
                // if in prev sector, add to its points
                if (prev.left_border.front()->x == (*it)->x) {
                    prev.left_border.push_back(*it);
                    continue;
                }
            }

            sector sec;
            sec.left_border.push_back(*it);
            // set status lines
            for (auto ln : lines) {
                if ((ln->p1->x <= (*it)->x && ln->p2->x > (*it)->x)
                || (ln->p2->x <= (*it)->x && ln->p1->x > (*it)->x)
                || (ln->p2->x == (*it)->x && ln->p1->x == (*it)->x)) {
                    sec.status.push_back(ln);
                }
            }
            sectors.insert(sectors.begin(), sec);
            
        }

        top_to_down(res, sectors);

		down_to_top(res, sectors);

        return res;
    }


}


} // ::algos
