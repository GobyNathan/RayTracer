/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2025                                                     *
 * Title           - mirror_raytracer [WSL: Debian]                                     *
 * Description     -                                                                    *
 *     DiamondMaterial                                                                  *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "DiamondMaterial.hpp"
#include <cmath>

namespace Raytracer {

DiamondMaterial::DiamondMaterial(const Math::Vector3D& color, double transparency,
    double reflectivity, double refractionIndex)
    : AMaterial(color)
    , transparency(transparency)
    , reflectivity(reflectivity)
    , refractionIndex(refractionIndex)
    , dispersion(0.044)
{
}

DiamondMaterial::DiamondMaterial(const libconfig::Setting& settings)
    : AMaterial(settings)
    , transparency(0.95)
    , reflectivity(0.2)
    , refractionIndex(2.42)
    , dispersion(0.044)
{
    settings.lookupValue("transparency", transparency);
    settings.lookupValue("reflectivity", reflectivity);
    settings.lookupValue("refractionIndex", refractionIndex);
    settings.lookupValue("dispersion", dispersion);
}

bool DiamondMaterial::isReflective() const
{
    return true;
}

bool DiamondMaterial::isTransparent() const
{
    return true;
}

double DiamondMaterial::getReflectivity() const
{
    return reflectivity;
}

double DiamondMaterial::getTransparency() const
{
    return transparency;
}

double DiamondMaterial::getRefractionIndex() const
{
    return refractionIndex;
}

std::unique_ptr<IMaterial> DiamondMaterial::clone() const
{
    return std::make_unique<DiamondMaterial>(color, transparency, reflectivity, refractionIndex);
}

Math::Vector3D DiamondMaterial::computeInteraction(
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

    // Calculate reflection direction
    Math::Vector3D reflectionDir = unit_direction - normal * (2 * unit_direction.dot(normal));
    reflectionDir = reflectionDir.normalize();

    // Create reflection ray
    Ray reflectionRay(intersection.hitPoint + reflectionDir * 0.001, reflectionDir);

    // Get reflection color
    Math::Vector3D reflectionColor = traceFunc(reflectionRay, depth + 1);

    // Calculate Fresnel effect (Schlick's approximation)
    double r0 = ((etaI - etaT) / (etaI + etaT)) * ((etaI - etaT) / (etaI + etaT));
    double fresnel = r0 + (1 - r0) * pow((1 - cos_theta), 5);

    // Increase Fresnel effect to simulate diamond sparkle
    fresnel = std::min(1.0, fresnel * 1.5);

    // Check for total internal reflection
    if (eta_ratio * sin_theta > 1.0) {
        // Total internal reflection - use reflection only
        return reflectionColor;
    } else {
        // Calculate refraction direction
        double cos_refracted = sqrt(1.0 - eta_ratio * eta_ratio * (1.0 - cos_theta * cos_theta));
        Math::Vector3D refractionDir = unit_direction * eta_ratio + normal * (eta_ratio * cos_theta - cos_refracted);
        refractionDir = refractionDir.normalize();

        // Create refraction ray
        Ray refractionRay(intersection.hitPoint + refractionDir * 0.001, refractionDir);

        // Get refraction color
        Math::Vector3D refractionColor = traceFunc(refractionRay, depth + 1);

        // Blend reflection and refraction using Fresnel
        Math::Vector3D resultColor = reflectionColor * fresnel + refractionColor * (1.0 - fresnel);

        // Apply a subtle color tint to simulate diamond dispersion
        if (depth < 2) { // Limit dispersion depth for performance
            double disperseAmount = sin_theta * dispersion;
            resultColor.x *= (1.0 + disperseAmount);
            resultColor.z *= (1.0 - disperseAmount * 0.5);
            resultColor = Math::Vector3D(
                std::min(1.0, resultColor.x),
                std::min(1.0, resultColor.y),
                std::min(1.0, resultColor.z));
        }

        // Apply color tint
        return color * (1.0 - transparency) + resultColor * transparency;
    }
}

} // namespace Raytracer