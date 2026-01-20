/*
** EPITECH PROJECT, 2025
** mirror_raytracer
** File description:
** LightRenderer.cpp
*/
#include "LightRenderer.hpp"
#include "../../core/Ray.hpp"
#include "../../utils/Debug.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

namespace Raytracer {

LightRenderer::LightRenderer(
    const std::vector<std::unique_ptr<ILight>>& lights,
    const std::vector<std::unique_ptr<IPrimitive>>& primitives,
    Math::Point3D cameraPosition)
    : _lights(lights)
    , _primitives(primitives)
    , _cameraPosition(cameraPosition)
{
}

Math::Vector3D LightRenderer::computeLight(Math::Point3D hitPoint,
    IPrimitive* prim)
{
    Math::Vector3D totalLight(0, 0, 0);
    Math::Vector3D normal = prim->getNormal(hitPoint);
    Math::Vector3D viewDir = (_cameraPosition - hitPoint).normalize();

    totalLight = Math::Vector3D(0.1, 0.1, 0.1);

    for (const auto& lightPtr : _lights) {
        const ILight& light = *lightPtr;
        float lightIntensity = light.getIntensity();

        if (light.isAmbientLight()) {
            totalLight += Math::Vector3D(lightIntensity, lightIntensity, lightIntensity);
            continue;
        }

        Math::Vector3D lightDir;
        if (light.isDirectionalLight()) {
            lightDir = -light.getDirection().normalize();
        } else {
            lightDir = (light.getOrigin() - hitPoint).normalize();
        }

        float dot = std::max(0.0, normal.dot(lightDir));
        float shadow = computeShadow(hitPoint, light);
        float diffuse = lightIntensity * dot * shadow;

        Math::Vector3D contribution(diffuse, diffuse, diffuse);

        if (light.getShadingModel() == ShadingModel::PHONG) {
            Math::Vector3D reflectDir = (normal * (2 * normal.dot(lightDir)) - lightDir).normalize();
            float specStrength = 0.5f;
            int shininess = 32;

            float specAngle = std::max(0.0, reflectDir.dot(viewDir));
            float specular = specStrength * std::pow(specAngle, shininess) * lightIntensity * shadow;

            contribution += Math::Vector3D(specular, specular, specular);
        }

        totalLight += contribution;
    }

    return Math::Vector3D(std::min(1.0, totalLight.x),
        std::min(1.0, totalLight.y),
        std::min(1.0, totalLight.z));
}

float LightRenderer::computeShadow(const Math::Point3D& hitPoint,
    const ILight& light)
{
    Math::Vector3D lightDir;
    double maxDist = std::numeric_limits<double>::infinity();

    if (light.isDirectionalLight()) {
        lightDir = -light.getDirection().normalize();
    } else {
        lightDir = (light.getOrigin() - hitPoint);
        maxDist = lightDir.length();
        lightDir = lightDir.normalize();
    }

    Math::Point3D shadowOrigin = hitPoint + lightDir * 0.001;
    Ray shadowRay(shadowOrigin, lightDir);

    for (const auto& prim : _primitives) {
        double t = prim->hits(shadowRay);
        if (t > 0 && t < maxDist) {
            return 0.0f;
        }
    }

    return 1.0f;
}

} // namespace Raytracer
