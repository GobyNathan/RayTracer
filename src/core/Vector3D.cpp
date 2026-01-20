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

#include "Vector3D.hpp"

namespace Math {

Vector3D::Vector3D(double x, double y, double z)
    : x(x)
    , y(y)
    , z(z)
{
}

double Vector3D::length() const { return std::sqrt(x * x + y * y + z * z); }

double Vector3D::dot(const Vector3D& other) const
{
    return x * other.x + y * other.y + z * other.z;
}

Vector3D Vector3D::normalize() const
{
    if (length() == 0)
        return Vector3D(0, 0, 0);
    return Vector3D(x / length(), y / length(), z / length());
}

Vector3D Vector3D::operator+(const Vector3D& other) const
{
    return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::cross(const Vector3D& other) const
{
    return Vector3D(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x);
}

Vector3D& Vector3D::operator+=(const Vector3D& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3D Vector3D::operator-(const Vector3D& other) const
{
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D& Vector3D::operator-=(const Vector3D& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3D Vector3D::operator*(const Vector3D& other) const
{
    return Vector3D(x * other.x, y * other.y, z * other.z);
}

Vector3D& Vector3D::operator*=(const Vector3D& other)
{
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
}

Vector3D Vector3D::operator/(const Vector3D& other) const
{
    return Vector3D(x / other.x, y / other.y, z / other.z);
}

Vector3D& Vector3D::operator/=(const Vector3D& other)
{
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
}

Vector3D Vector3D::operator*(double scalar) const
{
    return Vector3D(x * scalar, y * scalar, z * scalar);
}

Vector3D& Vector3D::operator*=(double scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3D Vector3D::operator/(double scalar) const
{
    return Vector3D(x / scalar, y / scalar, z / scalar);
}

Vector3D& Vector3D::operator/=(double scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

Vector3D Vector3D::operator-() const { return Vector3D(-x, -y, -z); }

} // namespace Math
