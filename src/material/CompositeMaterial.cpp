/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, May, 2025                                                     *
 * Title           - mirror_raytracer [WSL: Debian]                                     *
 * Description     -                                                                    *
 *     CompositeMaterial                                                                *
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

#include "CompositeMaterial.hpp"
#include "../factories/MaterialFactory.hpp"
#include "../utils/Debug.hpp"

namespace Raytracer {

CompositeMaterial::CompositeMaterial(const libconfig::Setting& settings)
    : AMaterial(settings)
{
    try {
        if (settings.exists("materials") && settings["materials"].isList()) {
            const libconfig::Setting& materialsList = settings["materials"];

            for (int i = 0; i < materialsList.getLength(); i++) {
                const libconfig::Setting& matEntry = materialsList[i];

                std::string type = "matte";
                double weight = 1.0;

                matEntry.lookupValue("type", type);
                matEntry.lookupValue("weight", weight);

                std::unique_ptr<IMaterial> mat = MaterialFactory::createMaterial(type, matEntry);
                addMaterial(std::move(mat), weight);

                Debug::log("Added material of type '", type, "' with weight ", weight, " to composite");
            }
        } else {
            Debug::log("Warning: Composite material has no sub-materials defined");
        }
    } catch (const libconfig::SettingException& ex) {
        throw std::runtime_error(std::string("Error in composite material parameters: ") + ex.what());
    }
}

Math::Vector3D CompositeMaterial::computeInteraction(
    const Ray& incidentRay,
    const IntersectionInfo& intersection,
    std::function<Math::Vector3D(const Ray&, int)> traceFunc,
    int depth) const
{

    Math::Vector3D result(0, 0, 0);
    double totalWeight = 0.0;

    for (const auto& [weight, material] : materials) {
        result += material->computeInteraction(
                      incidentRay, intersection, traceFunc, depth)
            * weight;
        totalWeight += weight;
    }

    if (totalWeight > 0) {
        result = result / totalWeight;
    } else {
        result = color;
    }

    return result;
}

std::unique_ptr<IMaterial> CompositeMaterial::clone() const
{
    auto copy = std::make_unique<CompositeMaterial>(color);
    for (const auto& [weight, material] : materials) {
        copy->addMaterial(material->clone(), weight);
    }
    return copy;
}

} // namespace Raytracer