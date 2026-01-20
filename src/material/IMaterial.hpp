/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2025 * Title           - mirror_raytracer [WSL:
 * Debian]                                     * Description     - * IMaterial *
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

#ifndef RAYTRACER_IMATERIAL_HPP_
#define RAYTRACER_IMATERIAL_HPP_

#include "../core/Point3D.hpp"
#include "../core/Ray.hpp"
#include "../core/Vector3D.hpp"
#include "../interfaces/IMaterialInteraction.hpp"
#include <functional>
#include <memory>

namespace Raytracer {

class IMaterial : public IMaterialInteraction {
public:
    virtual ~IMaterial() = default;

    virtual Math::Vector3D getColor() const = 0;
    virtual bool isReflective() const = 0;
    virtual bool isTransparent() const = 0;
    virtual double getReflectivity() const = 0;
    virtual double getTransparency() const = 0;
    virtual double getRefractionIndex() const = 0;

    // Default implementation for material interaction
    virtual Math::Vector3D computeInteraction(
        const Ray& incidentRay,
        const IntersectionInfo& intersection,
        std::function<Math::Vector3D(const Ray&, int)> traceFunc,
        int depth) const override;

    // Clone method for copying materials
    virtual std::unique_ptr<IMaterial> clone() const = 0;
};

} // namespace Raytracer

#endif /* RAYTRACER_IMATERIAL_HPP_ */

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
