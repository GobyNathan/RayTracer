/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, May, 2025                                                     *
 * Title           - mirror_raytracer [WSL: Debian]                                     *
 * Description     -                                                                    *
 *     Cylinder                                                                         *
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

#ifndef RAYTRACER_CYLINDER_HPP
#define RAYTRACER_CYLINDER_HPP

#include "../core/Point3D.hpp"
#include "../core/Ray.hpp"
#include "../core/Vector3D.hpp"
#include "../interfaces/ATransformable.hpp"
#include <libconfig.h++>

namespace Raytracer {

class Cylinder : public ATransformable {
private:
    Math::Point3D center;
    Math::Vector3D axis;
    double radius;
    double height;
    bool limited;

    Math::Point3D topCenter;

    // Ray intersection results for individual cylinder components
    struct HitResult {
        double t; // Parameter where ray hits
        bool isValid;
        int component; // 0 = side, 1 = bottom cap, 2 = top cap

        HitResult()
            : t(std::numeric_limits<double>::max())
            , isValid(false)
            , component(-1)
        {
        }
        HitResult(double t, bool isValid, int component)
            : t(t)
            , isValid(isValid)
            , component(component)
        {
        }
    };

    bool isWithinHeight(const Math::Point3D& point) const; // Calculate if a point is within the height limits of the cylinder
    HitResult hitSide(const Ray& ray) const;
    HitResult hitBottomCap(const Ray& ray) const;
    HitResult hitTopCap(const Ray& ray) const;
    HitResult getClosestHit(const HitResult& h1, const HitResult& h2, const HitResult& h3) const;

public:
    const Math::Point3D& getCenter() const { return center; }
    const Math::Vector3D& getAxis() const { return axis; }
    double getRadius() const { return radius; }
    double getHeight() const { return height; }
    bool isLimited() const { return limited; }
    Cylinder(const Math::Point3D& center, const Math::Vector3D& axis, double radius, double height = 0.0, std::unique_ptr<IMaterial> material = nullptr);
    Cylinder(const libconfig::Setting& settings);

    double localHits(const Ray& ray) const override;
    Math::Vector3D localGetNormal(const Math::Point3D& point) const override;
    bool isPlane() const override;
};

} // namespace Raytracer

#endif /* RAYTRACER_CYLINDER_HPP */

/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * MIT License                                                                          *
 * Copyright (c) 2025 Anonymous                                                         *
 *                                                                                      *
 * Permission is hereby granted, free of charge, to any person obtaining a copy         *
 * of this software and associated documentation files (the "Software"), to deal        *
 * in the Software without restriction, including without limitation the rights         *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell            *
 * copies of the Software, and to permit persons to whom the Software is                *
 * furnished to do so, subject to the following conditions:                             *
 *                                                                                      *
 * The above copyright notice and this permission notice shall be included in all       *
 * copies or substantial portions of the Software.                                      *
 *                                                                                      *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR           *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,             *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE          *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER               *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,        *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE        *
 * SOFTWARE.                                                                            *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */
