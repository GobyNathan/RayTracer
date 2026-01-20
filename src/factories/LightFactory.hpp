/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Apr, 2025 * Title           - mirror_raytracer *
 * Description     - * LightFactory *
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

#ifndef RAYTRACER_LIGHTFACTORY_HPP_
#define RAYTRACER_LIGHTFACTORY_HPP_

#include <functional>
#include <iostream>
#include <libconfig.h++>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

namespace Raytracer {

class ILight;

class LightFactory {
public:
    using Creator = std::function<std::unique_ptr<ILight>(const libconfig::Setting&)>;

    template <typename T>
    static void registerType(const std::string& type);
    static std::unique_ptr<ILight>
    createLight(const std::string& type, const libconfig::Setting& settings);
    static std::vector<std::string> getRegisteredTypes()
    {
        std::vector<std::string> types;
        for (const auto& [type, _] : creators) {
            types.push_back(type);
        }
        return types;
    }

private:
    static std::map<std::string, Creator> creators;
};

inline std::map<std::string, LightFactory::Creator> LightFactory::creators;

template <typename T>
void LightFactory::registerType(const std::string& type)
{
    creators[type] = [](const libconfig::Setting& settings) {
        return std::make_unique<T>(settings);
    };
}

inline std::unique_ptr<ILight>
LightFactory::createLight(const std::string& type,
    const libconfig::Setting& settings)
{
    auto it = creators.find(type);
    if (it == creators.end())
        throw std::runtime_error("Unknown light type: " + type);
    return it->second(settings);
}

} // namespace Raytracer

#endif /* !RAYTRACER_LIGHTFACTORY_HPP_ */
