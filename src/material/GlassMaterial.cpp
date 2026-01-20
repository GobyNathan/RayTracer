/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2025                                                     *
 * Title           - mirror_raytracer [WSL: Debian]                                     *
 * Description     -                                                                    *
 *     GlassMaterial                                                                    *
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

#include "GlassMaterial.hpp"
#include "../utils/Debug.hpp"
#include <cmath>

namespace Raytracer {

GlassMaterial::GlassMaterial(const Math::Vector3D& color, double transparency, double reflectivity, double refractionIndex)
    : AMaterial(color)
    , transparency(transparency)
    , reflectivity(reflectivity)
    , refractionIndex(refractionIndex)
{
}

GlassMaterial::GlassMaterial(const libconfig::Setting& settings)
    : AMaterial(settings)
    , transparency(0.9)
    , reflectivity(0.1)
    , refractionIndex(1.5)
{
    settings.lookupValue("transparency", transparency);
    settings.lookupValue("reflectivity", reflectivity);
    settings.lookupValue("refractionIndex", refractionIndex);

    Debug::log("Created glass material with refractive index: ", refractionIndex);
    Debug::log("Transparency: ", transparency, ", Reflectivity: ", reflectivity);
}

bool GlassMaterial::isReflective() const
{
    return true;
}

bool GlassMaterial::isTransparent() const
{
    return true;
}

double GlassMaterial::getReflectivity() const
{
    return reflectivity;
}

double GlassMaterial::getTransparency() const
{
    return transparency;
}

double GlassMaterial::getRefractionIndex() const
{
    return refractionIndex;
}

std::unique_ptr<IMaterial> GlassMaterial::clone() const
{
    return std::make_unique<GlassMaterial>(color, transparency, reflectivity, refractionIndex);
}

// Calculate Schlick's approximation for Fresnel reflection factor
double GlassMaterial::schlickFresnelApproximation(double cosTheta, double n1, double n2) const
{
    // Calculate base reflectance at normal incidence
    double r0 = ((n1 - n2) / (n1 + n2)) * ((n1 - n2) / (n1 + n2));

    // Calculate angular dependence
    return r0 + (1.0 - r0) * pow(1.0 - cosTheta, 5);
}

Math::Vector3D GlassMaterial::computeInteraction(
    const Ray& incidentRay,
    const IntersectionInfo& intersection,
    std::function<Math::Vector3D(const Ray&, int)> traceFunc,
    int depth) const
{
    Debug::log("===== Glass material interaction =====");
    Debug::log("Material color: (", color.x, ", ", color.y, ", ", color.z, ")");
    Debug::log("Transparency: ", transparency, ", Refractive index: ", refractionIndex);
    Debug::log("Intersection at: (", intersection.hitPoint.x, ", ", intersection.hitPoint.y, ", ", intersection.hitPoint.z, ")");
    Debug::log("Current depth: ", depth);

    if (depth >= 4) {
        Debug::log("Max depth reached, returning base color");
        return color;
    }

    // Normalize incident direction
    Math::Vector3D unit_direction = incidentRay.direction.normalize();
    Math::Vector3D normal = intersection.normal;

    // Determine which side we're hitting from
    bool entering = intersection.frontFace;
    Debug::log("Entering material: ", entering ? "yes" : "no");
    Debug::log("Normal: (", normal.x, ", ", normal.y, ", ", normal.z, ")");
    Debug::log("Direction: (", unit_direction.x, ", ", unit_direction.y, ", ", unit_direction.z, ")");

    // Set up indices of refraction
    double etaI = 1.0; // Air
    double etaT = refractionIndex;

    if (!entering) {
        // We're exiting the material
        std::swap(etaI, etaT);
    }

    double ratio = etaI / etaT;

    // For normal dot direction, we need the raw value (can be negative)
    double cos_theta_raw = normal.dot(unit_direction);
    double cos_theta = std::abs(cos_theta_raw);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    Debug::log("eta_i: ", etaI, ", eta_t: ", etaT, ", ratio: ", ratio);
    Debug::log("Cos theta raw: ", cos_theta_raw, ", Cos theta abs: ", cos_theta);
    Debug::log("Sin theta: ", sin_theta);

    // Check for total internal reflection
    bool cannot_refract = ratio * sin_theta > 1.0;
    Debug::log("Total internal reflection: ", cannot_refract ? "yes" : "no");

    // Calculate reflection direction
    Math::Vector3D reflection_dir = unit_direction - normal * (2 * cos_theta_raw);
    reflection_dir = reflection_dir.normalize();

    // Create reflection ray with small offset to avoid self-intersection
    Ray reflection_ray(intersection.hitPoint + reflection_dir * 0.001, reflection_dir);
    Math::Vector3D reflection_color = traceFunc(reflection_ray, depth + 1);
    Debug::log("Reflection color: (", reflection_color.x, ", ", reflection_color.y, ", ", reflection_color.z, ")");

    // Calculate Fresnel term using Schlick's approximation
    double r0 = ((etaI - etaT) / (etaI + etaT)) * ((etaI - etaT) / (etaI + etaT));
    double schlick = r0 + (1.0 - r0) * std::pow(1.0 - cos_theta, 5);
    Debug::log("R0: ", r0, ", Schlick: ", schlick);

    Math::Vector3D refraction_color;

    // Handle total internal reflection or compute refraction
    if (cannot_refract) {
        Debug::log("Total internal reflection - using only reflection");
        refraction_color = reflection_color;
    } else {
        // Calculate refraction direction using Snell's law
        double cos_refracted = std::sqrt(1.0 - ratio * ratio * (1.0 - cos_theta * cos_theta));

        double normal_component = ratio * cos_theta - cos_refracted;
        if (cos_theta_raw < 0) {
            normal_component = ratio * cos_theta + cos_refracted;
        }

        Math::Vector3D refraction_dir = unit_direction * ratio + normal * normal_component;
        refraction_dir = refraction_dir.normalize();

        Debug::log("Refraction dir: (", refraction_dir.x, ", ", refraction_dir.y, ", ", refraction_dir.z, ")");

        // Create refraction ray with small offset in the refraction direction
        Ray refraction_ray(intersection.hitPoint + refraction_dir * 0.001, refraction_dir);
        refraction_color = traceFunc(refraction_ray, depth + 1);
        Debug::log("Refraction color: (", refraction_color.x, ", ", refraction_color.y, ", ", refraction_color.z, ")");
    }

    // Use Fresnel to blend reflection and refraction
    // Schlick approximation determines the reflectivity at this angle
    Math::Vector3D blend_color = reflection_color * schlick + refraction_color * (1.0 - schlick);

    // Apply material color as a tint, based on transparency
    Math::Vector3D final_color = color * (1.0 - transparency) + blend_color * transparency;

    Debug::log("Schlick (reflectivity): ", schlick);
    Debug::log("Final color: (", final_color.x, ", ", final_color.y, ", ", final_color.z, ")");
    Debug::log("==============================");

    return final_color;
}

} // namespace Raytracer