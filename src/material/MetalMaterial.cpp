/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2025                                                     *
 * Title           - mirror_raytracer [WSL: Debian]                                     *
 * Description     -                                                                    *
 *     MetalMaterial                                                                    *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "MetalMaterial.hpp"
#include <algorithm>
#include <cmath>
#include <random>

namespace Raytracer {

MetalMaterial::MetalMaterial(const Math::Vector3D& color, double roughness)
    : AMaterial(color)
    , roughness(roughness)
    , reflectivity(0.8)
{
}

MetalMaterial::MetalMaterial(const libconfig::Setting& settings)
    : AMaterial(settings)
    , roughness(0.1)
    , reflectivity(0.8)
{
    settings.lookupValue("roughness", roughness);
    settings.lookupValue("reflectivity", reflectivity);

    // Clamp roughness to valid range
    roughness = std::max(0.0, std::min(1.0, roughness));
}

bool MetalMaterial::isReflective() const
{
    return true;
}

double MetalMaterial::getReflectivity() const
{
    return reflectivity;
}

double MetalMaterial::getRoughness() const
{
    return roughness;
}

std::unique_ptr<IMaterial> MetalMaterial::clone() const
{
    return std::make_unique<MetalMaterial>(color, roughness);
}

Math::Vector3D MetalMaterial::computeInteraction(
    const Ray& incidentRay,
    const IntersectionInfo& intersection,
    std::function<Math::Vector3D(const Ray&, int)> traceFunc,
    int depth) const
{

    Math::Vector3D incident = incidentRay.direction.normalize();
    Math::Vector3D normal = intersection.normal;

    // Calculate perfect reflection direction
    Math::Vector3D reflectionDir = incident - normal * (2 * incident.dot(normal));
    reflectionDir = reflectionDir.normalize();

    // Add randomness based on roughness
    if (roughness > 0) {
        reflectionDir = (reflectionDir + randomUnitVector() * roughness).normalize();
    }

    // Create reflection ray with small offset to avoid self-intersection
    Ray reflectionRay(intersection.hitPoint + reflectionDir * 0.001, reflectionDir);

    // Get reflected color by recursively tracing
    Math::Vector3D reflectionColor = traceFunc(reflectionRay, depth + 1);

    // Blend with base color according to reflectivity
    return color * (1.0 - reflectivity) + reflectionColor * reflectivity;
}

Math::Vector3D MetalMaterial::randomUnitVector() const
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    // Create a random vector and normalize
    Math::Vector3D v;
    do {
        v = Math::Vector3D(dis(gen), dis(gen), dis(gen));
    } while (v.length() >= 1.0);

    return v.normalize();
}

} // namespace Raytracer