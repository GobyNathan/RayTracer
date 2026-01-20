/*
** EPITECH PROJECT, 2025
** mirror_raytracer
** File description:
** PrimitiveRenderer.cpp
*/
#include "PrimitiveRenderer.hpp"
#include "../../utils/Debug.hpp"
#include <limits>

namespace Raytracer {

PrimitiveRenderer::PrimitiveRenderer(
    const std::vector<std::unique_ptr<IPrimitive>>& primitives)
    : _primitives(primitives)
{
}

IPrimitive* PrimitiveRenderer::findClosestIntersection(const Ray& ray,
    IntersectionInfo& info)
{
    double closest_hit = std::numeric_limits<double>::max();
    IPrimitive* hitPrim = nullptr;
    int hitCount = 0;

    for (auto& prim : _primitives) {
        double t = prim->hits(ray);
        if (t > 0 && t < closest_hit) {
            closest_hit = t;
            hitPrim = prim.get();
            hitCount++;
        }
    }

    Debug::log("Found ", hitCount, " intersections, closest at t=", closest_hit);

    if (hitPrim) {
        info.t = closest_hit;
        info.hitPoint = ray.at(closest_hit);

        Math::Vector3D outward_normal = hitPrim->getNormal(info.hitPoint);
        double dot_product = ray.direction.dot(outward_normal);

        info.frontFace = dot_product < 0;

        if (info.frontFace) {
            info.normal = outward_normal;
        } else {
            info.normal = -outward_normal;
        }

        Debug::log("Hit at point (", info.hitPoint.x, ", ", info.hitPoint.y, ", ",
            info.hitPoint.z, ")");
        Debug::log("Normal: (", info.normal.x, ", ", info.normal.y, ", ",
            info.normal.z, ")");
        Debug::log("Front face: ", info.frontFace ? "true" : "false");
        Debug::log("Dot product: ", dot_product);
    }

    return hitPrim;
}

} // namespace Raytracer
