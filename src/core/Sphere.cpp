/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2025 * Title           - mirror_raytracer [WSL:
 * Debian]                                     * Description     - * Sphere *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _| *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _| *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_| *
 *       _|        _|          _|        _|      _|        _|        _|    _| *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _| *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

// Sphere.cpp
#include "Sphere.hpp"
#include "../factories/MaterialFactory.hpp"
#include "../utils/Debug.hpp"
#include "Point3D.hpp"
#include "Vector3D.hpp"
#include <cmath>
#include <memory>
#include <stdexcept>

namespace Raytracer {

Sphere::Sphere(const Math::Point3D& center, double radius,
    std::unique_ptr<IMaterial> material)
    : ATransformable()
    , center(center)
    , radius(radius)
{
    // Set the material if provided, otherwise the base class will use default
    if (material) {
        this->material = std::move(material);
    }
}

Sphere::Sphere(const libconfig::Setting& settings)
    : ATransformable()
{
    try {
        double x, y, z, r;

        settings.lookupValue("x", x);
        settings.lookupValue("y", y);
        settings.lookupValue("z", z);
        settings.lookupValue("r", r);

        center = Math::Point3D(0, 0, 0);
        position = Math::Vector3D(x, y, z);
        radius = r;

        // Use the base class method to load material
        loadMaterial(settings);
        loadTransforms(settings);

        Debug::log("Sphere created with center: (", center.x, ", ", center.y, ", ",
            center.z, ") and radius: ", radius);
    } catch (const libconfig::SettingException& ex) {
        throw std::runtime_error(std::string("Error in sphere parameters: ") + ex.what());
    }
}

double Sphere::localHits(const Ray& localRay) const
{
    Math::Vector3D oc = localRay.origin - center;
    double a = localRay.direction.dot(localRay.direction);
    double b = 2.0 * oc.dot(localRay.direction);
    double c = oc.dot(oc) - radius * radius;

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return -1;
    } else {
        double t1 = (-b - std::sqrt(discriminant)) / (2.0 * a);
        double t2 = (-b + std::sqrt(discriminant)) / (2.0 * a);

        if (t1 > 0)
            return t1;
        else if (t2 > 0)
            return t2;
        else
            return -1;
    }
}

Math::Vector3D Sphere::localGetNormal(const Math::Point3D& localPoint) const
{
    Math::Vector3D normal(localPoint.x - center.x, localPoint.y - center.y,
        localPoint.z - center.z);
    double length = normal.length();
    return Math::Vector3D(normal.x / length, normal.y / length,
        normal.z / length);
}

bool Sphere::isPlane() const { return false; }

} // namespace Raytracer
