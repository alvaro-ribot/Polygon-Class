#include "Polygon.hh"
#include "Point.hh"
#include "Color.hh"

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

using vp = vector <Point>;


/* Constructor:
Creates a polygon with its vector of points (vertices) "points" and color "c".
The points vector is optional and defaults to an empty vector.
The c color is optional and defaults to (R, G, B) = (0, 0, 0). 
The vector of points is updated to its convex hull. */
Polygon::Polygon(const vp& points, Color c) 
:     points(points), c(c) {
   convexHull();
}


/* Comparison for sorting "points". */
struct Polygon::Comp {
    Point O;
    Comp(const Point &P) : O(P) {}  //Picks a reference point (leftmost and downmost one).
    //Sorts according to the slope with the reference point.
    bool operator() (const Point a, const Point b) {
        double a1 = a.getX() - O.getX();
        double a2 = a.getY() - O.getY();
        double b1 = b.getX() - O.getX();
        double b2 = b.getY() - O.getY();
        // In case of tie, sorts by distance.
        if(abs(a2*b1 - b2*a1) < 1e-12) return O.distance(a) < O.distance(b);
        return a2*b1 > b2*a1;
    }
};


/* Sorts the vector of points according to the slopes whith respect to the 
point with lower X (and with lower Y in case of ties). */
void Polygon::sorting_slope(vp& points) {
    Comp comp(points[0]);
    sort(points.begin(), points.end(), comp);
}


/* Checks whether three points are aligned. */
static bool aligned(Point a, Point b, Point c) {
    return abs((c.getY()-a.getY())*(b.getX()-a.getX()) - (b.getY()-a.getY())*(c.getX()-a.getX())) < 1e-12;
}


/* Returns true if c is at the left of ab. */
static bool leftof(Point a, Point b, Point c) {
    return (c.getY() - a.getY())*(b.getX()-a.getX()) >= (b.getY() - a.getY())*(c.getX()-a.getX());
}


/* Updates the vector of points to its convex hull. */
void Polygon::convexHull() {
    int n = points.size();
    if (n < 2) return;
    if (n == 2) {
        if (points[0].distance(points[1]) < 1e-12) points.pop_back();
    } else {
        // Pick the leftmost and downmost point.
        int left = 0;
        for (int i = 1; i < n; i++)
            if (points[i].getX() <= points[left].getX()){
                if (points[i].getX() < points[left].getX() or 
                points[i].getY() < points[left].getY()) left = i;
        }
        swap(points[0], points[left]);
        sorting_slope(points);
        vp hull = {points[0]};
        // Pick the farthest point aligned with the first different points.
        int l = 2;
        while (l < n and aligned(points[0], points[l-1], points[l])) ++l;
        hull.push_back(points[l-1]);
        if (l < n) hull.push_back(points[l]);
        int m = 2;
        for (int k = l+1; k < n; ++k) {
            while (leftof(hull[m-1], hull[m], points[k])) {
                --m;
                hull.pop_back();
            }
            ++m;
            hull.push_back(points[k]);  // Add point to the convex hull.
        }
        points = hull;
    }
}


/* Gets the vector of points of this polygon. */
vp Polygon::getPoints() const {
    return points;
}


/* Gets the color of this polygon. */
Color Polygon::getcol() const {
    return c;
}


/* Returns the area of this polygon. */
double Polygon::area() const {
    double sum = 0;
    int n = points.size();
    // We use shoelace formula.
    for (int i = 0; i < n-1; ++i) {
        sum += points[i].getX()*points[i+1].getY();
        sum -= points[i+1].getX()*points[i].getY();
    }
    sum += points[n-1].getX()*points[0].getY();
    sum -= points[0].getX()*points[n-1].getY(); 
    sum = abs(sum)/2;
    return sum;
}


/* Returns the perimeter of this polygon. */
double Polygon::perimeter() const {
    double sum = 0;
    int n = points.size();
    for (int i = 0; i < n; ++i) sum += points[i].distance(points[(i+1)%n]);
    return sum;
}


/* Returns the number of vertices of this polygon. */
int Polygon::vertices() const {
    return int(points.size());
}


/* Returns the centroid of this polygon. */
Point Polygon::centroid() const {
    Point Centroid(0, 0);
    int n = points.size();
    for (int i = 0; i < n; ++i) Centroid += points[i];
    double x = Centroid.getX()/n;
    double y = Centroid.getY()/n;
    return Point(x, y);
}


/* Returns the number of edges of this polygon. */
int Polygon::edges() const {
    int size = points.size();
    if (size == 1) return 0;
    if (size == 2) return 1;
    return size;
}


/* Check whether this polygon is regular. */
bool Polygon::regular() const {
    int n = points.size();
    if (n > 3){
        double d = points[n-1].distance(points[0]);
        for (int i = 0; i < n - 1; ++i) {
            if (points[i].distance(points[i+1]) - d > 1e-12) return false;
        }
    }
    return true;
}


/* Returns the width of this polygon. */
double Polygon::width() const {
    Polygon Box = *this;
    Box = Box.bbox();
    vp pBox = Box.getPoints();
    int n = pBox.size();
    if (n < 2) return 0;
    //Invariant: n == 4.
    return pBox[2].getX() - pBox[0].getX();
}


/* Returns the height of this polygon. */
double Polygon::height() const {
    Polygon Box = *this;
    Box = Box.bbox();
    vp pBox = Box.getPoints();
    int n = pBox.size();
    if (n < 2) return 0;
    //Invariant: n == 4.
    return pBox[1].getY() - pBox[0].getY();
}


/* Sets the Color of this polygon to c. */
void Polygon::setcol(Color col) {
    c = col; 
}


/* Checks whether point P is inside the polygon V. */
static bool in(const Point& P, const vp& v) {
    int n = v.size();
    for (int i = 0; i + 1 < n; ++i) {
        //srictly leftof
        if (leftof(v[i], v[i+1], P) and not aligned(v[i], v[i+1], P)) return false;
    }
    if (leftof(v[n-1], v[0], P)) return false;
    return true;
}


/* Adds the points of polygon P that are in polygon V, and viceversa, 
   to the vp inter (intersection). */
static void inter_point(const vp& ppoints, const vp& vpoints, vp& inter) {
    int n = ppoints.size();
    for (int i = 0; i < n; ++i) {
        if (in(ppoints[i], vpoints)) inter.push_back(ppoints[i]);
    }
    int m = vpoints.size();
    for (int i = 0; i < m; ++i) {
        if (in(vpoints[i], ppoints)) inter.push_back(vpoints[i]);
    }
}


/* Adds the intersection points of the edges of two polygons (P and V)
   to the vp inter. */
static void inter_seg(const vp& ppoints, const vp& vpoints, vp& inter) {
    int n = ppoints.size();
    int m = vpoints.size();
    for (int i = 0; i < n; ++i) {
        int i1 = (i+1 != n ? i+1 : 0);
        for (int j = 0; j < m; ++j) {
            int j1 = (j+1 != m ? j+1 : 0);
            // Compute the coefs of the cartesian equation of a line
            double a1 = ppoints[i1].getY() - ppoints[i].getY();
            double b1 = ppoints[i1].getX() - ppoints[i].getX();
            double c1 = b1*ppoints[i].getY() - a1*ppoints[i].getX();
            double a2 = vpoints[j1].getY() - vpoints[j].getY();
            double b2 = vpoints[j1].getX() - vpoints[j].getX();
            double c2 = b2*vpoints[j].getY() - a2*vpoints[j].getX();
            if (abs(a1*b2 - b1*a2) > 1e-12) { //Check whether the two segments are parallels.
                double x = (b1*c2 - b2*c1)/(a1*b2 - b1*a2);
                double y = (a1*c2 - a2*c1)/(a1*b2 - b1*a2);
                if (abs(x) < 1e-12) x = 0;
                if (abs(y) < 1e-12) y = 0;
                Point P(x, y);
                double d1 = ppoints[i].distance(ppoints[i1]);
                double d2 = vpoints[j].distance(vpoints[j1]);
                // Check whether the intersection point is really in between the two segments.
                if (ppoints[i].distance(P) < d1 and ppoints[i1].distance(P) < d1
                    and vpoints[j].distance(P) < d2 and vpoints[j1].distance(P) < d2) {
                    inter.push_back(P);
                }
            }
        }
    }
}


/* Returns the intersection of this polygon with polygon V. */
Polygon Polygon::intersection(const Polygon& V) const {
    vp ppoints = points;
    vp vpoints = V.getPoints();
    vp inter;
    inter_point(ppoints, vpoints, inter);
    inter_seg(ppoints, vpoints, inter);
    return Polygon(inter);
}


/* Returns the union of this polygon with polygon V. */
Polygon Polygon::union_(const Polygon& V) const {
    vp ppoints = points;
    vp vpoints = V.getPoints();
    int n = vpoints.size();
    for(int i = 0; i < n; ++i) ppoints.push_back(vpoints[i]);
    Polygon W(ppoints);
    return W;
}


/* Checks whether this polygon is inside polygon V. */
bool Polygon::inside(const Polygon& V) const{
    Polygon p1 = *this;
    Polygon W = p1.union_(V);
    vp w = W.getPoints();
    vp v = V.getPoints();
    if (w.size() != v.size()) return false;
    else {
        int n = w.size();
        for (int i = 0; i < n; ++i) if (w[i] != v[i]) return false;
    }
    return true;
}


/* Returns the bounding box of this polygon. */
Polygon Polygon::bbox() const {
    int n = points.size();
    if (n < 2) return Polygon(points, c);
    else {
        double max_x, min_x, max_y, min_y;
        max_x = min_x = points[0].getX();
        max_y = min_y = points[0].getY();
        for (int i = 1; i < n; ++i) {
            if(points[i].getX() > max_x) max_x = points[i].getX();
            if(points[i].getX() < min_x) min_x = points[i].getX();
            if(points[i].getY() > max_y) max_y = points[i].getY();
            if(points[i].getY() < min_y) min_y = points[i].getY();
        }
        Point A(min_x, min_y);
        Point B(min_x, max_y);
        Point C(max_x, max_y);
        Point D(max_x, min_y);
        vp p = {A, B, C, D};
        return Polygon(p);
    }
}
