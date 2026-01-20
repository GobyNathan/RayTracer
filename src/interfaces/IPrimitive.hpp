/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Apr, 2025 * Title           - mirror_raytracer *
 * Description     - * IPrimitive *
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

#ifndef RAYTRACER_IPRIMITIVE_HPP_
#define RAYTRACER_IPRIMITIVE_HPP_

#include "../core/Point3D.hpp"
#include "../core/Ray.hpp"
#include "../core/Vector3D.hpp"
#include "../material/IMaterial.hpp"
#include <memory>

namespace Raytracer {

class IMaterial;

class IPrimitive {
public:
    virtual ~IPrimitive() = default;
    virtual double hits(const Ray& ray) const = 0;
    virtual Math::Vector3D getNormal(const Math::Point3D& point) const = 0;
    virtual std::unique_ptr<IMaterial> getMaterial() const = 0;
    virtual bool isPlane() const = 0;
};

} // namespace Raytracer

#endif /* !RAYTRACER_IPRIMITIVE_HPP_ */
