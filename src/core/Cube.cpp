/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, May, 2025                                                     *
 * Title           - mirror_raytracer                                                   *
 * Description     -                                                                    *
 *     Cube                                                                             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "Cube.hpp"
#include "../factories/MaterialFactory.hpp"
#include "../utils/Debug.hpp"
#include "Point3D.hpp"
#include "Vector3D.hpp"
#include <algorithm>
#include <cmath>
#include <memory>
#include <stdexcept>

namespace Raytracer {

Cube::Cube(const Math::Point3D& center, double side,
    std::unique_ptr<IMaterial> material)
    : ATransformable()
    , center(center)
    , side(side)
{
    if (material) {
        this->material = std::move(material);
    }
}

Cube::Cube(const libconfig::Setting& settings)
    : ATransformable()
{
    try {
        double x, y, z, s;

        settings.lookupValue("x", x);
        settings.lookupValue("y", y);
        settings.lookupValue("z", z);
        settings.lookupValue("side", s);
        position = Math::Vector3D(x, y, z);
        center = Math::Point3D(0, 0, 0);

        side = s;

        loadMaterial(settings);
        loadTransforms(settings);

        Debug::log("Cube created with center: (", center.x, ", ", center.y, ", ",
            center.z, ") and side: ", side);
    } catch (const libconfig::SettingException& ex) {
        throw std::runtime_error(std::string("Error in cube parameters: ") + ex.what());
    }
}

double Cube::localHits(const Ray& localRay) const
{
    double halfSide = side / 2.0;
    Math::Point3D min(center.x - halfSide, center.y - halfSide, center.z - halfSide);
    Math::Point3D max(center.x + halfSide, center.y + halfSide, center.z + halfSide);

    double tx1 = (min.x - localRay.origin.x) / localRay.direction.x;
    double tx2 = (max.x - localRay.origin.x) / localRay.direction.x;
    double ty1 = (min.y - localRay.origin.y) / localRay.direction.y;
    double ty2 = (max.y - localRay.origin.y) / localRay.direction.y;
    double tz1 = (min.z - localRay.origin.z) / localRay.direction.z;
    double tz2 = (max.z - localRay.origin.z) / localRay.direction.z;

    if (tx1 > tx2)
        std::swap(tx1, tx2);
    if (ty1 > ty2)
        std::swap(ty1, ty2);
    if (tz1 > tz2)
        std::swap(tz1, tz2);

    double tmin = std::max(std::max(tx1, ty1), tz1);
    double tmax = std::min(std::min(tx2, ty2), tz2);

    if (tmax < 0)
        return -1;
    if (tmin > tmax)
        return -1;
    if (tmin < 0)
        return tmax;

    return tmin;
}

Math::Vector3D Cube::localGetNormal(const Math::Point3D& localPoint) const
{
    double halfSide = side / 2.0;
    double epsilon = 1e-4;

    Math::Vector3D dir(localPoint.x - center.x, localPoint.y - center.y, localPoint.z - center.z);

    if (std::abs(std::abs(dir.x) - halfSide) < epsilon)
        return Math::Vector3D(dir.x > 0 ? 1 : -1, 0, 0);
    if (std::abs(std::abs(dir.y) - halfSide) < epsilon)
        return Math::Vector3D(0, dir.y > 0 ? 1 : -1, 0);
    if (std::abs(std::abs(dir.z) - halfSide) < epsilon)
        return Math::Vector3D(0, 0, dir.z > 0 ? 1 : -1);

    return dir.normalize();
}

std::unique_ptr<IMaterial> Cube::getMaterial() const
{
    if (material) {
        return material->clone();
    }
    return MaterialFactory::createDefaultMaterial();
}

bool Cube::isPlane() const
{
    return false;
}

} // namespace Raytracer
