/*
** EPITECH PROJECT, 2024
** mirror_raytracer
** File description:
** Plane.cpp
*/

#include "Plane.hpp"
#include "../utils/Debug.hpp"
#include <stdexcept>

namespace Raytracer {

Plane::Plane(const libconfig::Setting& settings)
{
    try {
        std::string axisStr;
        double pos;

        settings.lookupValue("axis", axisStr);
        settings.lookupValue("position", pos);

        if (axisStr != "X" && axisStr != "Y" && axisStr != "Z") {
            throw std::runtime_error("Invalid axis value: " + axisStr + " (must be X, Y, or Z)");
        }

        axis = axisStr;
        position = pos;

        // Use the base class method to load material
        loadMaterial(settings);

        Debug::log("Plane created on axis: ", axis, " at position: ", position);
    } catch (const libconfig::SettingException& ex) {
        throw std::runtime_error(std::string("Error in plane parameters: ") + ex.what());
    }
}

double Plane::hits(const Ray& ray) const
{
    double rayDirection;
    double rayOrigin;

    if (axis == "X") {
        rayDirection = ray.direction.x;
        rayOrigin = ray.origin.x;
    } else if (axis == "Y") {
        rayDirection = ray.direction.y;
        rayOrigin = ray.origin.y;
    } else {
        rayDirection = ray.direction.z;
        rayOrigin = ray.origin.z;
    }

    // Avoid division by zero
    if (std::abs(rayDirection) < 0.0001) {
        return -1.0;
    }

    double t = (position - rayOrigin) / rayDirection;

    if (t >= 0) {
        return t;
    }
    return -1.0;
}

Math::Vector3D Plane::getNormal(const Math::Point3D& point) const
{
    (void)point;
    if (axis == "X") {
        return Math::Vector3D(1, 0, 0);
    } else if (axis == "Y") {
        return Math::Vector3D(0, 1, 0);
    } else {
        return Math::Vector3D(0, 0, 1);
    }
}

std::unique_ptr<IMaterial> Plane::getMaterial() const
{
    if (material) {
        return material->clone();
    } else {
        return MaterialFactory::createDefaultMaterial();
    }
}

bool Plane::isPlane() const { return true; }

} // namespace Raytracer
