/*
** EPITECH PROJECT, 2024
** mirror_raytracer
** File description:
** Plane.hpp
*/

#ifndef RAYTRACER_PLANE_HPP
#define RAYTRACER_PLANE_HPP

#include "../core/Point3D.hpp"
#include "../core/Ray.hpp"
#include "../core/Vector3D.hpp"
#include "../interfaces/APrimitive.hpp"
#include <libconfig.h++>
#include <string>

namespace Raytracer {

class Plane : public APrimitive {
public:
    std::string axis;
    double position;

    Plane(const libconfig::Setting& settings);

    double hits(const Ray& ray) const override;
    Math::Vector3D getNormal(const Math::Point3D& point) const override;
    std::unique_ptr<IMaterial> getMaterial() const override;
    bool isPlane() const override;
};

} // namespace Raytracer

#endif
