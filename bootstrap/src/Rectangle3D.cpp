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

#include "Rectangle3D.hpp"

namespace Math {

Rectangle3D::Rectangle3D(const Point3D& origin, const Vector3D& bottom_side, const Vector3D& left_side)
    : origin(origin)
    , bottom_side(bottom_side)
    , left_side(left_side)
{
}

Point3D Rectangle3D::pointAt(double u, double v) const
{
    return Point3D(
        origin.x + u * bottom_side.x + v * left_side.x,
        origin.y + u * bottom_side.y + v * left_side.y,
        origin.z + u * bottom_side.z + v * left_side.z);
}

}
