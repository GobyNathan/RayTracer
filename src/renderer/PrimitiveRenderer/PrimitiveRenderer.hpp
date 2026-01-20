/*
** EPITECH PROJECT, 2025
** mirror_raytracer
** File description:
** PrimitiveRenderer.hpp
*/
#ifndef PRIMITIVE_RENDERER_HPP
#define PRIMITIVE_RENDERER_HPP

#include "../../core/Ray.hpp"
#include "../../core/Vector3D.hpp"
#include "../../interfaces/IPrimitive.hpp"
#include <memory>
#include <vector>

namespace Raytracer {

class PrimitiveRenderer {
private:
    const std::vector<std::unique_ptr<IPrimitive>>& _primitives;

public:
    PrimitiveRenderer(const std::vector<std::unique_ptr<IPrimitive>>& primitives);

    IPrimitive* findClosestIntersection(const Ray& ray, IntersectionInfo& info);
};

} // namespace Raytracer

#endif /* PRIMITIVE_RENDERER_HPP */
