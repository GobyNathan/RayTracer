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

#include "Ray.hpp"

namespace Raytracer {

Ray::Ray(const Math::Point3D& origin, const Math::Vector3D& direction)
    : origin(origin)
    , direction(direction)
{
}

Math::Point3D Ray::at(double t)
{
    return origin + direction * t;
}

Math::Point3D Ray::at(double t) const { return origin + direction * t; }

} // namespace Raytracer
