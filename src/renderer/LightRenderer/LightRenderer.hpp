/*
** EPITECH PROJECT, 2025
** mirror_raytracer
** File description:
** LightRenderer.hpp
*/
#ifndef LIGHT_RENDERER_HPP
#define LIGHT_RENDERER_HPP

#include "../../core/Point3D.hpp"
#include "../../core/Ray.hpp"
#include "../../core/Vector3D.hpp"
#include "../../interfaces/ILight.hpp"
#include "../../interfaces/IPrimitive.hpp"
#include <memory>
#include <vector>

namespace Raytracer {

class LightRenderer {
private:
    const std::vector<std::unique_ptr<ILight>>& _lights;
    const std::vector<std::unique_ptr<IPrimitive>>& _primitives;
    Math::Point3D _cameraPosition;

public:
    LightRenderer(const std::vector<std::unique_ptr<ILight>>& lights,
        const std::vector<std::unique_ptr<IPrimitive>>& primitives,
        Math::Point3D cameraPosition);

    Math::Vector3D computeLight(Math::Point3D hitPoint, IPrimitive* prim);
    float computeShadow(const Math::Point3D& hitPoint, const ILight& light);
};

} // namespace Raytracer

#endif /* LIGHT_RENDERER_HPP */
