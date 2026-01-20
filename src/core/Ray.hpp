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

#ifndef RAYTRACER_RAY_HPP
#define RAYTRACER_RAY_HPP

#include "Point3D.hpp"

namespace Raytracer {

class Ray {
public:
    Math::Point3D origin;
    Math::Vector3D direction;

    Ray() = default;
    Ray(const Math::Point3D& origin, const Math::Vector3D& direction);

    Math::Point3D at(double t);
    Math::Point3D at(double t) const;
};

} // namespace Raytracer

#endif
