/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, May, 2025                                                     *
 * Title           - mirror_raytracer                                                   *
 * Description     -                                                                    *
 *     Cone                                                                             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "Cone.hpp"
#include "../factories/MaterialFactory.hpp"
#include "../utils/Debug.hpp"
#include <cmath>
#include <memory>
#include <stdexcept>

namespace Raytracer {

Cone::Cone(const Math::Point3D& base, double radius, double height,
    const Math::Vector3D& direction, std::unique_ptr<IMaterial> material,
    double cut_height)
    : ATransformable()
    , base(base)
    , radius(radius)
    , height(height)
    , direction(direction.normalize())
    , cut_height(cut_height)
{
    if (height != -1 && height <= 0) {
        throw std::runtime_error("Height must be positive or -1 for infinite cone");
    }
    if (cut_height >= 0) {
        if (cut_height <= 0 || (height != -1 && cut_height >= height)) {
            throw std::runtime_error("Cut height must be greater than 0 and less than cone height");
        }
    }
    if (material) {
        this->material = std::move(material);
    }
}

Cone::Cone(const libconfig::Setting& settings)
    : ATransformable()
    , cut_height(-1)
{
    try {
        double x, y, z, r, h;
        double dx, dy, dz;

        settings.lookupValue("x", x);
        settings.lookupValue("y", y);
        settings.lookupValue("z", z);
        settings.lookupValue("r", r);
        position = Math::Vector3D(x, y, z);
        if (!settings.lookupValue("h", h)) {
            h = -1.0; // Default to infinite cone if height not specified
        }
        settings.lookupValue("dx", dx);
        settings.lookupValue("dy", dy);
        settings.lookupValue("dz", dz);

        base = Math::Point3D(0, 0, 0);
        radius = r;
        height = h;

        Math::Vector3D dir(dx, dy, dz);
        direction = dir.normalize();

        double cut_h;
        if (settings.exists("cut_height") && settings.lookupValue("cut_height", cut_h)) {
            cut_height = cut_h;
            if (cut_height <= 0 || (height != -1 && cut_height >= height)) {
                throw std::runtime_error("Cut height must be greater than 0 and less than cone height");
            }
        }

        loadMaterial(settings);
        loadTransforms(settings);
    } catch (const libconfig::SettingException& ex) {
        throw std::runtime_error(std::string("Error in cone parameters: ") + ex.what());
    }
}

double Cone::localHits(const Ray& ray) const
{
    double t_bottom = -1.0;
    double t_top = -1.0;

    // Only check base and top intersections for finite cones
    if (height != -1) {
        // Check bottom base circle intersection
        {
            Math::Vector3D oc = ray.origin - base;
            double denom = ray.direction.dot(-direction);
            if (std::abs(denom) > 1e-6) {
                double t = -oc.dot(-direction) / denom;
                if (t > 0 && isOnBase(ray.origin + ray.direction * t)) {
                    t_bottom = t;
                }
            }
        }

        // Check top base circle intersection for cut cones
        if (cut_height > 0) {
            Math::Point3D topCenter = base + direction * cut_height;
            Math::Vector3D oc = ray.origin - topCenter;
            double denom = ray.direction.dot(direction);
            if (std::abs(denom) > 1e-6) {
                double t = -oc.dot(direction) / denom;
                if (t > 0 && isOnTopBase(ray.origin + ray.direction * t)) {
                    t_top = t;
                }
            }
        }
    }

    // Check cone surface intersection
    double t_surface = -1.0;
    {
        // For infinite cones, the apex is the base point
        Math::Point3D apex = height == -1 ? base : base + direction * height;
        Math::Vector3D axis = -direction;
        Math::Vector3D oc = ray.origin - apex;

        double alpha = height == -1 ? std::atan(radius) : std::atan2(radius, height);
        double cosA = std::cos(alpha);
        double cosA2 = cosA * cosA;

        double a = ray.direction.dot(axis) * ray.direction.dot(axis) - cosA2 * ray.direction.dot(ray.direction);
        double b = 2.0 * (ray.direction.dot(axis) * oc.dot(axis) - cosA2 * ray.direction.dot(oc));
        double c = oc.dot(axis) * oc.dot(axis) - cosA2 * oc.dot(oc);

        double discriminant = b * b - 4.0 * a * c;
        if (discriminant >= 0) {
            double t1 = (-b - std::sqrt(discriminant)) / (2.0 * a);
            double t2 = (-b + std::sqrt(discriminant)) / (2.0 * a);

            if (t1 > 0) {
                Math::Point3D p = ray.origin + ray.direction * t1;
                Math::Vector3D v = p - base;
                double h = v.dot(direction);
                double h_max = cut_height > 0 ? cut_height : height;
                if (h >= 0 && (height == -1 || h <= h_max)) {
                    t_surface = t1;
                }
            }
            if (t2 > 0 && (t_surface < 0 || t2 < t_surface)) {
                Math::Point3D p = ray.origin + ray.direction * t2;
                Math::Vector3D v = p - base;
                double h = v.dot(direction);
                double h_max = cut_height > 0 ? cut_height : height;
                if (h >= 0 && (height == -1 || h <= h_max)) {
                    t_surface = t2;
                }
            }
        }
    }

    // Return the closest valid intersection
    double t = -1.0;
    if (t_bottom > 0) {
        t = t_bottom;
    }
    if (t_surface > 0 && (t < 0 || t_surface < t)) {
        t = t_surface;
    }
    if (t_top > 0 && (t < 0 || t_top < t)) {
        t = t_top;
    }
    return t;
}

Math::Vector3D Cone::localGetNormal(const Math::Point3D& point) const
{
    if (cut_height > 0 && isOnTopBase(point)) {
        return direction;
    }
    if (isOnBase(point)) {
        return -direction;
    }
    return getNormalOnSurface(point);
}

bool Cone::isOnBase(const Math::Point3D& point) const
{
    if (height == -1) {
        return false;
    }

    Math::Vector3D baseToPoint = point - base;
    double dotWithAxis = baseToPoint.dot(direction);

    if (std::abs(dotWithAxis) > 1e-6) {
        return false;
    }

    Math::Vector3D projectionOnBase = baseToPoint - direction * dotWithAxis;
    return projectionOnBase.length() <= radius;
}

bool Cone::isOnTopBase(const Math::Point3D& point) const
{
    if (height == -1 || cut_height <= 0) {
        return false;
    }

    Math::Point3D topCenter = base + direction * cut_height;
    Math::Vector3D topToPoint = point - topCenter;
    double dotWithAxis = topToPoint.dot(direction);

    if (std::abs(dotWithAxis) > 1e-6) {
        return false;
    }

    Math::Vector3D projectionOnTop = topToPoint - direction * dotWithAxis;
    return projectionOnTop.length() <= getTopRadius();
}

Math::Vector3D Cone::getNormalOnBase() const
{
    return -direction;
}

Math::Vector3D Cone::getNormalOnSurface(const Math::Point3D& point) const
{
    Math::Vector3D baseToPoint = point - base;
    double height_component = baseToPoint.dot(direction);
    Math::Vector3D radial = baseToPoint - direction * height_component;
    double radial_length = radial.length();

    if (radial_length < 1e-6) {
        if (std::abs(direction.x) > std::abs(direction.y)) {
            return Math::Vector3D(direction.z, 0, -direction.x).normalize();
        } else {
            return Math::Vector3D(0, direction.z, -direction.y).normalize();
        }
    }

    double max_height = cut_height > 0 ? cut_height : height;
    // For infinite cones, we use the radius to determine the angle directly
    double theta = height == -1 ? std::atan(radius) : std::atan2(radius, max_height);
    double sin_theta = std::sin(theta);
    double cos_theta = std::cos(theta);

    Math::Vector3D radial_normalized = radial.normalize();
    Math::Vector3D normal = radial_normalized * cos_theta + direction * sin_theta;
    return normal.normalize();
}

double Cone::getTopRadius() const
{
    if (cut_height <= 0) {
        return 0.0;
    }
    return radius * (1.0 - cut_height / height);
}

std::unique_ptr<IMaterial> Cone::getMaterial() const
{
    if (material) {
        return material->clone();
    }
    return nullptr;
}

bool Cone::isPlane() const
{
    return false;
}

} // namespace Raytracer
