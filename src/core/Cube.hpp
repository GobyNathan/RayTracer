/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, May, 2025                                                     *
 * Title           - mirror_raytracer                                                   *
 * Description     -                                                                    *
 *     Cube                                                                             *
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

#ifndef RAYTRACER_CUBE_HPP
#define RAYTRACER_CUBE_HPP

#include "../interfaces/ATransformable.hpp"

namespace Raytracer {

class Cube : public ATransformable {
public:
    Math::Point3D center;
    double side;

    Cube(const Math::Point3D& center, double side,
        std::unique_ptr<IMaterial> material = nullptr);
    Cube(const libconfig::Setting& settings);

protected:
    double localHits(const Ray& ray) const override;
    Math::Vector3D localGetNormal(const Math::Point3D& point) const override;

public:
    std::unique_ptr<IMaterial> getMaterial() const override;
    bool isPlane() const override;
};

} // namespace Raytracer

#endif /* RAYTRACER_CUBE_HPP */
