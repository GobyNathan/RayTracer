/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, May, 2025                                                     *
 * Title           - mirror_raytracer                                                   *
 * Description     -                                                                    *
 *     Triangle                                                                         *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef RAYTRACER_TRIANGLE_HPP
#define RAYTRACER_TRIANGLE_HPP

#include "../interfaces/ATransformable.hpp"

namespace Raytracer {

class Triangle : public ATransformable {
public:
    Math::Point3D v1; // First vertex
    Math::Point3D v2; // Second vertex
    Math::Point3D v3; // Third vertex

    Triangle(const Math::Point3D& v1, const Math::Point3D& v2, const Math::Point3D& v3,
        std::unique_ptr<IMaterial> material = nullptr);
    Triangle(const libconfig::Setting& settings);

protected:
    double localHits(const Ray& ray) const override;
    Math::Vector3D localGetNormal(const Math::Point3D& point) const override;

public:
    bool isPlane() const override;
};

} // namespace Raytracer

#endif /* RAYTRACER_TRIANGLE_HPP */
