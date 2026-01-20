/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2025                                                     *
 * Title           - mirror_raytracer [WSL: Debian]                                     *
 * Description     -                                                                    *
 *     TranslucentMaterial                                                              *
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

#include "TranslucentMaterial.hpp"
#include <cmath>

namespace Raytracer {

TranslucentMaterial::TranslucentMaterial(const Math::Vector3D& color, double transparency,
    double refractionIndex)
    : AMaterial(color)
    , transparency(transparency)
    , refractionIndex(refractionIndex)
{
}

TranslucentMaterial::TranslucentMaterial(const libconfig::Setting& settings)
    : AMaterial(settings)
    , transparency(0.3)
    , refractionIndex(1.2)
{
    settings.lookupValue("transparency", transparency);
    settings.lookupValue("refractionIndex", refractionIndex);
}

bool TranslucentMaterial::isTransparent() const
{
    return true;
}

double TranslucentMaterial::getTransparency() const
{
    return transparency;
}

double TranslucentMaterial::getRefractionIndex() const
{
    return refractionIndex;
}

std::unique_ptr<IMaterial> TranslucentMaterial::clone() const
{
    return std::make_unique<TranslucentMaterial>(color, transparency, refractionIndex);
}

Math::Vector3D TranslucentMaterial::computeInteraction(
    const Ray& incidentRay,
    const IntersectionInfo& intersection,
    std::function<Math::Vector3D(const Ray&, int)> traceFunc,
    int depth) const
{

    Math::Vector3D unit_direction = incidentRay.direction.normalize();
    Math::Vector3D normal = intersection.normal;

    // Determine refraction indices based on whether we're entering or exiting
    double etaI = 1.0; // Air
    double etaT = refractionIndex;

    if (!intersection.frontFace) {
        // We're exiting the material
        std::swap(etaI, etaT);
    }

    double eta_ratio = etaI / etaT;

    // Calculate the cosine of the angle between the normal and incident ray
    double cos_theta = fmin(-unit_direction.dot(normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    // Check for total internal reflection
    if (eta_ratio * sin_theta > 1.0) {
        // Total internal reflection - use reflection
        Math::Vector3D reflectionDir = unit_direction - normal * (2 * unit_direction.dot(normal));
        reflectionDir = reflectionDir.normalize();

        Ray reflectionRay(intersection.hitPoint + reflectionDir * 0.001, reflectionDir);
        Math::Vector3D reflectionColor = traceFunc(reflectionRay, depth + 1);

        return color * (1.0 - transparency) + reflectionColor * transparency;
    } else {
        // Calculate refraction direction
        double cos_refracted = sqrt(1.0 - eta_ratio * eta_ratio * (1.0 - cos_theta * cos_theta));
        Math::Vector3D refractionDir = unit_direction * eta_ratio + normal * (eta_ratio * cos_theta - cos_refracted);
        refractionDir = refractionDir.normalize();

        // Create refraction ray
        Ray refractionRay(intersection.hitPoint + refractionDir * 0.001, refractionDir);

        // Get refraction color
        Math::Vector3D refractionColor = traceFunc(refractionRay, depth + 1);

        // Blend with base color based on transparency
        return color * (1.0 - transparency) + refractionColor * transparency;
    }
}

} // namespace Raytracer