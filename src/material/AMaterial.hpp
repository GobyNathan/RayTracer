/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2025 * Title           - mirror_raytracer [WSL:
 * Debian]                                     * Description     - * AMaterial *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _| *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _| *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_| *
 *       _|        _|          _|        _|      _|        _|        _|    _| *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _| *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#ifndef RAYTRACER_AMATERIAL_HPP_
#define RAYTRACER_AMATERIAL_HPP_

#include "IMaterial.hpp"
#include <libconfig.h++>

namespace Raytracer {

class AMaterial : public IMaterial {
protected:
    Math::Vector3D color;

public:
    AMaterial(const Math::Vector3D& color);
    AMaterial(const libconfig::Setting& settings);
    virtual ~AMaterial() = default;

    Math::Vector3D getColor() const override;

    bool isReflective() const override;
    bool isTransparent() const override;
    double getReflectivity() const override;
    double getTransparency() const override;
    double getRefractionIndex() const override;

    // Base implementation of material interaction
    Math::Vector3D computeInteraction(const Ray& incidentRay, const IntersectionInfo& intersection, std::function<Math::Vector3D(const Ray&, int)> traceFunc, int depth) const override;
};

} // namespace Raytracer

#endif /* RAYTRACER_AMATERIAL_HPP_ */

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
