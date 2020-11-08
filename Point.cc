#include "Point.hh"

#include <cmath>
using namespace std;


/** Constructor. */
Point::Point(double x_coord, double y_coord):
    x(x_coord), y(y_coord) {}


/** Gets the x coordinate of this point. */
double Point::getX() const {
    return x;
}


/** Gets the y coordinate of this point. */
double Point::getY() const {
    return y;
}


/** Returns the distance to point p from this point. */
double Point::distance(const Point& p) const {
    double dx = getX() - p.getX();
    double dy = getY() - p.getY();
    return sqrt(dx*dx + dy*dy);
}


/** Returns the radius of this point (distance from the origin). */
double Point::radius() const {
    return sqrt(getX()*getX() + getY()*getY());
}


/** Returns the angle of the polar coordinate. */
double Point::angle() const {
    if (getX() == 0) {
        if (getY() == 0) return 0;
        return M_PI_2;
    }
    return atan(getY()/getX());
}


/** Compares this point to point p. */
bool Point::operator== (const Point& p) const {
    return getX() == p.getX() and getY() == p.getY();
}


/** Compares this point to point p. */
bool Point::operator!= (const Point& p) const {
    return not operator==(p);
}


/** Adds the coordinates of p to this point and returns this point. */
Point& Point::operator+= (const Point& p) {
    x += p.getX();
    y += p.getY();
    return *this;
}


/** Returns the point resulting of adding the coordinates of this point and p. */
Point Point::operator+ (const Point& p) const {
    return Point(p) += *this;
}

