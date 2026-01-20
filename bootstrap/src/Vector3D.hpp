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

#ifndef VECTOR3D_HPP
#define VECTOR3D_HPP

#include <cmath>

namespace Math {

class Vector3D {
public:
    double x;
    double y;
    double z;

    Vector3D(double x = 0, double y = 0, double z = 0);

    double length() const;
    double dot(const Vector3D& other) const;

    Vector3D operator+(const Vector3D& other) const;
    Vector3D& operator+=(const Vector3D& other);
    Vector3D operator-(const Vector3D& other) const;
    Vector3D& operator-=(const Vector3D& other);
    Vector3D operator*(const Vector3D& other) const;
    Vector3D& operator*=(const Vector3D& other);
    Vector3D operator/(const Vector3D& other) const;
    Vector3D& operator/=(const Vector3D& other);

    Vector3D operator*(double scalar) const;
    Vector3D& operator*=(double scalar);
    Vector3D operator/(double scalar) const;
    Vector3D& operator/=(double scalar);
};

}

#endif
