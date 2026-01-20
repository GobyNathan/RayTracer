/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2025                                                     *
 * Title           - mirror_raytracer [WSL: Debian]                                     *
 * Description     -                                                                    *
 *     MirrorMaterial                                                                   *
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

#include "MirrorMaterial.hpp"
#include "../utils/Debug.hpp"
#include <cmath>

namespace Raytracer {

MirrorMaterial::MirrorMaterial(const Math::Vector3D& color, double reflectivity)
    : AMaterial(color)
    , reflectivity(reflectivity)
{
}

MirrorMaterial::MirrorMaterial(const libconfig::Setting& settings)
    : AMaterial(settings)
    , reflectivity(0.9)
{
    settings.lookupValue("reflectivity", reflectivity);
    Debug::log("Created mirror material with color: ", color.x, ", ", color.y, ", ", color.z);
    Debug::log("Reflectivity: ", reflectivity);
}

bool MirrorMaterial::isReflective() const
{
    return true;
}

double MirrorMaterial::getReflectivity() const
{
    return reflectivity;
}

std::unique_ptr<IMaterial> MirrorMaterial::clone() const
{
    return std::make_unique<MirrorMaterial>(color, reflectivity);
}

Math::Vector3D MirrorMaterial::computeInteraction(
    const Ray& incidentRay,
    const IntersectionInfo& intersection,
    std::function<Math::Vector3D(const Ray&, int)> traceFunc,
    int depth) const
{
    // Calculate reflection direction
    Math::Vector3D incident = incidentRay.direction.normalize();
    Math::Vector3D normal = intersection.normal;

    // Reflect = incident - 2(incident·normal)normal
    double dot = incident.dot(normal);
    Math::Vector3D reflectionDir = incident - normal * (2.0 * dot);
    reflectionDir = reflectionDir.normalize();

    // Add offset to avoid self-intersection
    Ray reflectionRay(intersection.hitPoint + reflectionDir * 0.001, reflectionDir);

    // Trace reflection ray
    Math::Vector3D reflectionColor = traceFunc(reflectionRay, depth + 1);

    // Calculate Fresnel factor based on angle
    double fresnel = reflectivity;
    double cosTheta = fabs(incident.dot(normal));
    if (cosTheta < 0.9) {
        // Increase reflectivity at glancing angles
        fresnel = reflectivity + (1.0 - reflectivity) * pow(1.0 - cosTheta, 3);
    }

    // Apply material color tint and reflectivity
    return color * (1.0 - fresnel) + reflectionColor * fresnel;
}

} // namespace Raytracer