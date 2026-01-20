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

Camera::Camera(Math::Point3D origin, Math::Point3D screenOrigin, int resX,
    int resY)
    : origin(origin)
    , // TO DO: MUST MAKE THIS DYNAMIC
    screen(screenOrigin, { 1.0 * static_cast<double>(resX) / resY, 0, 0 },
        { 0, 1.0, 0 })
    , resolution({ resX, resY })
    , rotation(0.0, 0.0, 0.0)
    , fieldOfView(0.0)
{
}

Camera::Camera(const Camera& other)
    : origin(other.origin)
    , screen(other.screen)
    , resolution(other.resolution)
    , rotation(other.rotation)
    , fieldOfView(other.fieldOfView)
{
}

Camera& Camera::operator=(const Camera& other)
{
    if (this != &other) {
        origin = other.origin;
        screen = other.screen;
        resolution = other.resolution;
        rotation = other.rotation;
        fieldOfView = other.fieldOfView;
    }
    return *this;
}

void Camera::setPosition(double x, double y, double z)
{
    origin = Math::Point3D(x, y, z);
}

void Camera::setPosition(const Math::Point3D& position) { origin = position; }

Math::Vector3D Camera::updateRotation(const Math::Vector3D& vector) const
{
    double x = rotation.x * M_PI / 180.0;
    double y = rotation.y * M_PI / 180.0;
    double z = rotation.z * M_PI / 180.0;

    double cx = cos(x);
    double sx = sin(x);
    double cy = cos(y);
    double sy = sin(y);
    double cz = cos(z);
    double sz = sin(z);

    double x1 = vector.x * cz - vector.y * sz;
    double y1 = vector.x * sz + vector.y * cz;
    double z1 = vector.z;

    double x2 = x1 * cx + z1 * sx;
    double y2 = y1;
    double z2 = -x1 * sx + z1 * cx;

    double x3 = x2;
    double y3 = y2 * cy - z2 * sy;
    double z3 = y2 * sy + z2 * cy;

    return Math::Vector3D(x3, y3, z3);
}

void Camera::updateScreenOrientation()
{
    double fovRadians = fieldOfView * M_PI / 180.0;

    double aspectRatio = (double)resolution.width / resolution.height;
    double halfHeight = tan(fovRadians / 2.0);
    double halfWidth = aspectRatio * halfHeight;

    Math::Vector3D horizontal = Math::Vector3D(2.0 * halfWidth, 0.0, 0.0);
    Math::Vector3D vertical = Math::Vector3D(0.0, 2.0 * halfHeight, 0.0);
    Math::Vector3D forward = Math::Vector3D(0.0, 0.0, -1);

    horizontal = updateRotation(horizontal);
    vertical = updateRotation(vertical);
    forward = updateRotation(forward);

    screen.origin = origin + forward;

    screen.bottom_side = horizontal;
    screen.left_side = vertical;
}

void Camera::setRotation(double x, double y, double z)
{
    rotation = Math::Vector3D(x, y, z);
    updateScreenOrientation();
}

void Camera::setRotation(const Math::Vector3D& rot)
{
    setRotation(rot.x, rot.y, rot.z);
}

void Camera::setFieldOfView(double fov)
{
    fieldOfView = fov;
    updateScreenOrientation();
}

void Camera::setResolution(int width, int height)
{
    resolution = { width, height };

    updateScreenOrientation();
    // TODO: Adjust screen aspect ratio based on resolution
}

Ray Camera::ray(double u, double v) const
{
    double screen_u = 2.0 * u - 1.0;
    double screen_v = 1.0 - 2.0 * v;

    Math::Point3D target = screen.origin + screen.bottom_side * screen_u + screen.left_side * screen_v;

    Math::Vector3D direction = (target - origin).normalize();
    return Ray(origin, direction);
}

} // namespace Raytracer
