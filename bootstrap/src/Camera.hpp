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

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Ray.hpp"
#include "Rectangle3D.hpp"

namespace Raytracer {

class Camera {
public:
    Math::Point3D origin;
    Math::Rectangle3D screen;

    Camera();
    Camera(const Camera& other);
    ~Camera() = default;

    Camera& operator=(const Camera& other);

    Ray ray(double u, double v) const;
};

}

#endif
