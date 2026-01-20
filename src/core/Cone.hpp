/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, May, 2025                                                     *
 * Title           - mirror_raytracer                                                   *
 * Description     -                                                                    *
 *     Cone                                                                             *
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

#ifndef RAYTRACER_CONE_HPP
#define RAYTRACER_CONE_HPP

#include "../core/Point3D.hpp"
#include "../core/Ray.hpp"
#include "../core/Vector3D.hpp"
#include "../interfaces/ATransformable.hpp"
#include <libconfig.h++>

namespace Raytracer {

class Cone : public ATransformable {
public:
    Math::Point3D base; // Base center point
    double radius; // Base radius (ignored for infinite cones)
    double height; // Height of cone (-1 for infinite cone)
    Math::Vector3D direction; // Direction vector (normalized)
    double cut_height; // Optional cut height (-1 means no cut)

    Cone(const Math::Point3D& base, double radius, double height = -1,
        const Math::Vector3D& direction = Math::Vector3D(0, -1, 0),
        std::unique_ptr<IMaterial> material = nullptr,
        double cut_height = -1);
    Cone(const libconfig::Setting& settings);

    std::unique_ptr<IMaterial> getMaterial() const override;
    bool isPlane() const override;

protected:
    double localHits(const Ray& localRay) const override;
    Math::Vector3D localGetNormal(const Math::Point3D& localPoint) const override;

    bool isOnBase(const Math::Point3D& point) const;
    bool isOnTopBase(const Math::Point3D& point) const;
    Math::Vector3D getNormalOnBase() const;
    Math::Vector3D getNormalOnSurface(const Math::Point3D& point) const;
    double getTopRadius() const;
};

} // namespace Raytracer

#endif
