/*
 * < Kawabonga ! >
 * ------------------------------
 *    \   ^__^
 *     \  (oo)\_______
 *        (__)\       )\/\
 *            ||----w |
 *            ||     ||
 *
 */

#ifndef MATH_POINT3D_HPP
#define MATH_POINT3D_HPP

#include "Vector3D.hpp"

namespace Math {

class Point3D {
public:
    double x;
    double y;
    double z;

    Point3D(double x = 0, double y = 0, double z = 0);

    Point3D operator+(const Vector3D& vector) const;
    Point3D& operator+=(const Vector3D& vector);
    Vector3D operator-(const Point3D& other) const;
};

}

#endif
