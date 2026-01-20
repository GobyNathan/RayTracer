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

#include "Camera.hpp"

namespace Raytracer {

Camera::Camera()
    : origin(0.0, 0.0, 0.0)
    , screen(
          Math::Point3D(-1.0, -0.5, -1.0),
          Math::Vector3D(2.0, 0.0, 0.0),
          Math::Vector3D(0.0, 1.0, 0.0))
{
}

Camera::Camera(const Camera& other)
    : origin(other.origin)
    , screen(other.screen)
{
}

Camera& Camera::operator=(const Camera& other)
{
    if (this != &other) {
        origin = other.origin;
        screen = other.screen;
    }
    return *this;
}

Ray Camera::ray(double u, double v) const
{
    Math::Point3D target = screen.pointAt(u, v);
    Math::Vector3D direction(
        target.x - origin.x,
        target.y - origin.y,
        target.z - origin.z);
    return Ray(origin, direction);
}

}
