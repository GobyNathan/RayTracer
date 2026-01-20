/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2025                                                     *
 * Title           - mirror_raytracer [WSL: Debian]                                     *
 * Description     -                                                                    *
 *     MatteMaterial                                                                    *
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

#include "MatteMaterial.hpp"

namespace Raytracer {

MatteMaterial::MatteMaterial(const Math::Vector3D& color)
    : AMaterial(color)
{
}

MatteMaterial::MatteMaterial(const libconfig::Setting& settings)
    : AMaterial(settings)
{
}

std::unique_ptr<IMaterial> MatteMaterial::clone() const
{
    return std::make_unique<MatteMaterial>(color);
}

Math::Vector3D MatteMaterial::computeInteraction(
    const Ray& incidentRay,
    const IntersectionInfo& intersection,
    std::function<Math::Vector3D(const Ray&, int)> traceFunc,
    int depth) const
{

    (void)incidentRay;
    (void)intersection;
    (void)traceFunc;
    (void)depth;

    return color;
}

} // namespace Raytracer