/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2025                                                     *
 * Title           - mirror_raytracer [WSL: Debian]                                     *
 * Description     -                                                                    *
 *     MaterialFactory                                                                  *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef RAYTRACER_MATERIALFACTORY_HPP_
#define RAYTRACER_MATERIALFACTORY_HPP_

#include "../material/CompositeMaterial.hpp"
#include "../material/DiamondMaterial.hpp"
#include "../material/GlassMaterial.hpp"
#include "../material/IMaterial.hpp"
#include "../material/MatteMaterial.hpp"
#include "../material/MetalMaterial.hpp"
#include "../material/MirrorMaterial.hpp"
#include "../material/TranslucentMaterial.hpp"
#include "../utils/Debug.hpp"
#include <functional>
#include <libconfig.h++>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

namespace Raytracer {

class MaterialFactory {
public:
    using Creator = std::function<std::unique_ptr<IMaterial>(const libconfig::Setting&)>;

    static void registerAllMaterials();

    template <typename T>
    static void registerType(const std::string& type);

    static std::unique_ptr<IMaterial> createMaterial(const std::string& type, const libconfig::Setting& settings);

    static std::unique_ptr<IMaterial> createDefaultMaterial();

    static std::unique_ptr<CompositeMaterial> createCompositeMaterial(const Math::Vector3D& color = Math::Vector3D(1.0, 1.0, 1.0))
    {
        return std::make_unique<CompositeMaterial>(color);
    }

private:
    static std::map<std::string, Creator> creators;
};

inline std::map<std::string, MaterialFactory::Creator> MaterialFactory::creators;

template <typename T>
void MaterialFactory::registerType(const std::string& type)
{
    creators[type] = [](const libconfig::Setting& settings) {
        return std::make_unique<T>(settings);
    };
}

inline std::unique_ptr<IMaterial> MaterialFactory::createMaterial(const std::string& type, const libconfig::Setting& settings)
{
    auto it = creators.find(type);
    if (it == creators.end())
        throw std::runtime_error("Unknown material type: " + type);
    return it->second(settings);
}

inline std::unique_ptr<IMaterial> MaterialFactory::createDefaultMaterial()
{
    return std::make_unique<MatteMaterial>(Math::Vector3D(1.0, 0.0, 0.0));
}

inline void MaterialFactory::registerAllMaterials()
{
    Debug::log("Registering all material types");
    registerType<MatteMaterial>("matte");
    registerType<MirrorMaterial>("mirror");
    registerType<GlassMaterial>("glass");
    registerType<DiamondMaterial>("diamond");
    registerType<TranslucentMaterial>("translucent");
    registerType<MetalMaterial>("metal");
    registerType<CompositeMaterial>("composite");
}

} // namespace Raytracer

#endif /* RAYTRACER_MATERIALFACTORY_HPP_ */

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
