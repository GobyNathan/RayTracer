/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2025                                                     *
 * Title           - mirror_raytracer [WSL: Debian]                                     *
 * Description     -                                                                    *
 *     APrimitive                                                                       *
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

#ifndef RAYTRACER_APRIMITIVE_HPP_
#define RAYTRACER_APRIMITIVE_HPP_

#include "../factories/MaterialFactory.hpp"
#include "../interfaces/IPrimitive.hpp"
#include "../utils/Debug.hpp"
#include <libconfig.h++>

namespace Raytracer {

class APrimitive : public IPrimitive {
protected:
    std::unique_ptr<IMaterial> material;

    APrimitive()
        : material(MaterialFactory::createDefaultMaterial())
    {
    }

    APrimitive(const APrimitive& other)
        : material(other.material ? other.material->clone() : nullptr)
    {
    }

    void loadMaterial(const libconfig::Setting& settings)
    {
        try {
            if (settings.exists("material")) {
                // Handle case where material is explicitly defined
                const libconfig::Setting& materialSettings = settings["material"];
                std::string materialType = "matte";
                materialSettings.lookupValue("type", materialType);
                material = MaterialFactory::createMaterial(materialType, materialSettings);

                Debug::log("Created material of type '", materialType, "' from material section");
            } else if (settings.exists("color")) {
                // Handle case where color is directly defined in the primitive
                int r = 0, g = 0, b = 0;
                const libconfig::Setting& colorSetting = settings["color"];
                colorSetting.lookupValue("r", r);
                colorSetting.lookupValue("g", g);
                colorSetting.lookupValue("b", b);

                Debug::log("Creating matte material with color from primitive: (", r, ", ", g, ", ", b, ")");
                material = std::make_unique<MatteMaterial>(
                    Math::Vector3D(r / 255.0, g / 255.0, b / 255.0));
            } else {
                Debug::log("No material or color specified, using default material");
                material = MaterialFactory::createDefaultMaterial();
            }
        } catch (const libconfig::SettingException& ex) {
            Debug::log("Error loading material: ", ex.what());
            material = MaterialFactory::createDefaultMaterial();
        }
    }

public:
    virtual ~APrimitive() = default;

    APrimitive& operator=(const APrimitive& other)
    {
        if (this != &other) {
            material = other.material ? other.material->clone() : nullptr;
        }
        return *this;
    }

    std::unique_ptr<IMaterial> getMaterial() const override
    {
        if (material) {
            auto clonedMaterial = material->clone();
            Math::Vector3D color = clonedMaterial->getColor();
            Debug::log("APrimitive returning material with color: (", color.x, ", ", color.y, ", ", color.z, ")");
            return clonedMaterial;
        } else {
            Debug::log("WARNING: APrimitive has no material, using default RED material");
            return MaterialFactory::createDefaultMaterial();
        }
    }
};

} // namespace Raytracer

#endif /* RAYTRACER_APRIMITIVE_HPP_ */

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
