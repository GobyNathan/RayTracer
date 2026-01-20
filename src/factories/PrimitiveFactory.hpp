/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Apr, 2025                                                     *
 * Title           - mirror_raytracer                                                   *
 * Description     -                                                                    *
 *     PrimitiveFactory                                                                 *
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

#ifndef RAYTRACER_PRIMITIVEFACTORY_HPP_
#define RAYTRACER_PRIMITIVEFACTORY_HPP_

#include <functional>
#include <libconfig.h++>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace Raytracer {

class IPrimitive;

class PrimitiveFactory {
public:
    using Creator = std::function<std::unique_ptr<IPrimitive>(const libconfig::Setting&)>;

    template <typename T>
    static void registerType(const std::string& type);
    static std::unique_ptr<IPrimitive> createPrimitive(const std::string& type, const libconfig::Setting& settings);
    static std::vector<std::string> getRegisteredTypes();

private:
    static std::map<std::string, Creator> creators;
};

inline std::map<std::string, PrimitiveFactory::Creator> PrimitiveFactory::creators;

template <typename T>
void PrimitiveFactory::registerType(const std::string& type)
{
    creators[type] = [](const libconfig::Setting& settings) {
        return std::make_unique<T>(settings);
    };
}

inline std::unique_ptr<IPrimitive> PrimitiveFactory::createPrimitive(const std::string& type, const libconfig::Setting& settings)
{
    auto it = creators.find(type);
    if (it == creators.end())
        throw std::runtime_error("Unknown primitive type: " + type);
    return it->second(settings);
}

inline std::vector<std::string> PrimitiveFactory::getRegisteredTypes()
{
    std::vector<std::string> types;
    for (const auto& [type, _] : creators) {
        types.push_back(type);
    }
    return types;
}

} // namespace Raytracer

#endif /* !RAYTRACER_PRIMITIVEFACTORY_HPP_ */