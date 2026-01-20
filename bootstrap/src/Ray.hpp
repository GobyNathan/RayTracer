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

#ifndef RAY_HPP
#define RAY_HPP

#include "Point3D.hpp"

namespace Raytracer {

class Ray {
public:
    Math::Point3D origin;
    Math::Vector3D direction;

    Ray() = default;
    Ray(const Math::Point3D& origin, const Math::Vector3D& direction);
};

}

#endif
