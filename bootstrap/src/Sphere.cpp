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

#include "Sphere.hpp"

namespace Raytracer {

Sphere::Sphere(const Math::Point3D& center, double radius)
    : center(center)
    , radius(radius)
{
}

bool Sphere::hits(const Ray& ray) const
{
    Math::Vector3D originToCenter(
        ray.origin.x - center.x,
        ray.origin.y - center.y,
        ray.origin.z - center.z);

    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * originToCenter.dot(ray.direction);
    double c = originToCenter.dot(originToCenter) - radius * radius;

    double discriminant = b * b - 4 * a * c;
    return discriminant >= 0.0;
}

}
