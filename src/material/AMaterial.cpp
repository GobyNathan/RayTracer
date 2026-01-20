/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2025 * Title           - mirror_raytracer [WSL:
 * Debian]                                     * Description     - * AMaterial *
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

#include "AMaterial.hpp"
#include "../utils/Debug.hpp"
#include <stdexcept>

namespace Raytracer {

AMaterial::AMaterial(const Math::Vector3D& color)
    : color(color)
{
}

AMaterial::AMaterial(const libconfig::Setting& settings)
{
    try {
        int r = 255, g = 255, b = 255; // Default white

        if (settings.exists("color")) {
            const libconfig::Setting& colorSetting = settings["color"];
            colorSetting.lookupValue("r", r);
            colorSetting.lookupValue("g", g);
            colorSetting.lookupValue("b", b);
        }

        // Normalize to 0.0-1.0 range
        color = Math::Vector3D(r / 255.0, g / 255.0, b / 255.0);
        Debug::log("Material color set to (", r, ", ", g, ", ", b, ")");
    } catch (const libconfig::SettingException& ex) {
        throw std::runtime_error(std::string("Error in material parameters: ") + ex.what());
    }
}

Math::Vector3D AMaterial::getColor() const { return color; }

bool AMaterial::isReflective() const { return false; }

bool AMaterial::isTransparent() const { return false; }

double AMaterial::getReflectivity() const { return 0.0; }

double AMaterial::getTransparency() const { return 0.0; }

double AMaterial::getRefractionIndex() const { return 1.0; }

Math::Vector3D AMaterial::computeInteraction(const Ray& incidentRay, const IntersectionInfo& intersection, std::function<Math::Vector3D(const Ray&, int)> traceFunc, int depth) const
{
    (void)incidentRay;
    (void)intersection;
    (void)traceFunc;
    (void)depth;

    return color;
}

} // namespace Raytracer