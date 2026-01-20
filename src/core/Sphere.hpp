/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2025 * Title           - mirror_raytracer [WSL:
 * Debian]                                     * Description     - * Sphere *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗ *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║ * █████╗
 * ██████╔╝██║   ██║   █████╗  ██║     ███████║                     * ██╔══╝
 * ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║ *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝ *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#ifndef RAYTRACER_SPHERE_HPP
#define RAYTRACER_SPHERE_HPP

#include "../interfaces/ATransformable.hpp"

namespace Raytracer {

class Sphere : public ATransformable {
public:
    Math::Point3D center;
    double radius;

    Sphere(const Math::Point3D& center, double radius, std::unique_ptr<IMaterial> material = nullptr);
    Sphere(const libconfig::Setting& settings);

protected:
    double localHits(const Ray& ray) const override;
    Math::Vector3D localGetNormal(const Math::Point3D& point) const override;

public:
    bool isPlane() const override;
};

} // namespace Raytracer

#endif

/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * MIT License * Copyright (c) 2025 Anonymous *
 *                                                                                      *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * * of this software and associated documentation files (the "Software"), to
 * deal        * in the Software without restriction, including without
 * limitation the rights         * to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell            * copies of the Software, and
 * to permit persons to whom the Software is                * furnished to do
 * so, subject to the following conditions:                             *
 *                                                                                      *
 * The above copyright notice and this permission notice shall be included in
 * all       * copies or substantial portions of the Software. *
 *                                                                                      *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE        * SOFTWARE. *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */
