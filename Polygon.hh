#ifndef Polygon_hh
#define Polygon_hh


#include "Point.hh"
#include "Color.hh"

#include <vector>
using namespace std;


/* The Polygon class stores a two dimensional polygon in the plane,
 * formed by two dimensional points (cfc. class Point),and provides 
 * some usefull operations for it.
*/

class Polygon {

    public:

    /* Constructor:
       Creates a polygon with its vector of points (vertices) "points" and color "c".
       The points vector is optional and defaults to an empty vector.
       The c color is optional and defaults to (R, G, B) = (0, 0, 0). 
       The vector of points is updated to its convex hull. */
    Polygon(const vector <Point>& points = {}, Color c = {0, 0, 0});

    /* Gets the vector of points of this polygon. */
    vector <Point> getPoints() const;

    /* Gets the color of this polygon. */
    Color getcol() const;

    /* Returns the area of this polygon. */
    double area() const;

    /* Returns the perimeter of this polygon. */
    double perimeter() const;

    /* Returns the number of vertices of this polygon. */
    int vertices() const;

    /* Returns the centroid of this polygon. */
    Point centroid() const;

    /* Returns the number of edges of this polygon. */
    int edges() const;

    /* Check whether this polygon is regular. */
    bool regular() const;

    /* Returns the width of this polygon. */
    double width() const;

    /* Returns the height of this polygon. */
    double height() const;

    /* Sets the Color of this polygon to c. */
    void setcol(Color c);

    /* Returns the intersection of this polygon with polygon V. */
    Polygon intersection(const Polygon& V) const;

    /* Returns the union of this polygon with polygon V. */
    Polygon union_(const Polygon& V) const;

    /* Checks whether this polygon is inside polygon V. */
    bool inside(const Polygon& V) const;

    /* Returns the bounding box of this polygon. */
    Polygon bbox() const;

    private:

    /* Vector of points of the polygon. */
    vector <Point> points;

    /* Color of the polygon. */
    Color c;

    /* Comparison for sorting "points". */
    struct Comp;

    /* Sorts the vector of points according to the slopes whith respect to the 
       point with lower X (and with lower Y in case of ties). */
    void sorting_slope(vector <Point>& points);

    /* Updates the vector of points to its convex hull. */
    void convexHull ();

};


#endif
