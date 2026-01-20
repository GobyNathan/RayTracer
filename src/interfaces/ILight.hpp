/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Apr, 2025 * Title           - mirror_raytracer *
 * Description     - * ILight *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░ * ▒
 * ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓ * █
 * ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █ *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#ifndef RAYTRACER_ILIGHT_HPP_
#define RAYTRACER_ILIGHT_HPP_

#include "../core/Point3D.hpp"
#include "../core/Vector3D.hpp"
#include <memory>

namespace Raytracer {

class Vector3D;
class Point3D;

enum ShadingModel { NONE = 0,
    PHONG = 1 };

class ILight {
private:
    float intensity;
    Math::Vector3D direction;
    ShadingModel _shadingModel;

public:
    virtual ~ILight() = default;
    virtual float getIntensity() const = 0;
    virtual ShadingModel getShadingModel() const = 0;

    virtual Math::Vector3D getDirection() const = 0;
    virtual Math::Point3D getOrigin() const = 0;
    virtual bool isDirectionalLight() const = 0;
    virtual bool isAmbientLight() const = 0;

    virtual float getIntensityAt(double distance) const
    {
        if (distance <= 0.0) {
            return 0.0f;
        }

        return getIntensity();
    }
};

} // namespace Raytracer

#endif /* !RAYTRACER_ILIGHT_HPP_ */
