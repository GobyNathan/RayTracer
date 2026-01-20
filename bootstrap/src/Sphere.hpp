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

#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Ray.hpp"

namespace Raytracer {

class Sphere {
public:
    Math::Point3D center;
    double radius;

    Sphere(const Math::Point3D& center, double radius);

    bool hits(const Ray& ray) const;
};

}

#endif
