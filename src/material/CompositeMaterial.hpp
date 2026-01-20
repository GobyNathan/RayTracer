/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, May, 2025                                                     *
 * Title           - mirror_raytracer [WSL: Debian]                                     *
 * Description     -                                                                    *
 *     CompositeMaterial                                                                *
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

#ifndef RAYTRACER_COMPOSITEMATERIAL_HPP_
#define RAYTRACER_COMPOSITEMATERIAL_HPP_

#include "AMaterial.hpp"
#include <memory>
#include <utility>
#include <vector>

namespace Raytracer {

class CompositeMaterial : public AMaterial {
private:
    std::vector<std::pair<double, std::unique_ptr<IMaterial>>> materials;

public:
    CompositeMaterial(const Math::Vector3D& color)
        : AMaterial(color)
    {
    }
    CompositeMaterial(const libconfig::Setting& settings);

    void addMaterial(std::unique_ptr<IMaterial> material, double weight)
    {
        materials.push_back({ weight, std::move(material) });
    }

    void clearMaterials()
    {
        materials.clear();
    }

    size_t getMaterialCount() const
    {
        return materials.size();
    }

    // Get total weight of all materials
    double getTotalWeight() const
    {
        double total = 0.0;
        for (const auto& [weight, _] : materials) {
            total += weight;
        }
        return total;
    }

    // Normalize weights to sum to 1.0
    void normalizeWeights()
    {
        double total = getTotalWeight();
        if (total > 0.0) {
            for (auto& [weight, _] : materials) {
                weight /= total;
            }
        }
    }

    Math::Vector3D computeInteraction(
        const Ray& incidentRay,
        const IntersectionInfo& intersection,
        std::function<Math::Vector3D(const Ray&, int)> traceFunc, int depth) const override;

    std::unique_ptr<IMaterial> clone() const override;
};

} // namespace Raytracer

#endif /* RAYTRACER_COMPOSITEMATERIAL_HPP_ */

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
