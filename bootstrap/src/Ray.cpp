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

}
