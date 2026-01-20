/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, May, 2025                                                     *
 * Title           - mirror_raytracer [WSL: Debian]                                     *
 * Description     -                                                                    *
 *     CylinderOptimizations                                                            *
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

#ifndef RAYTRACER_CYLINDER_OPTIMIZATIONS_HPP
#define RAYTRACER_CYLINDER_OPTIMIZATIONS_HPP

#include "Cylinder.hpp"

namespace Raytracer {

// Bounding Box Structure
struct AABB {
    Math::Point3D min; // Minimum coordinates
    Math::Point3D max; // Maximum coordinates

    AABB() = default;

    AABB(const Math::Point3D& min, const Math::Point3D& max)
        : min(min)
        , max(max)
    {
    }

    bool intersect(const Ray& ray) const
    {
        // Track the smallest and largest t values along each dimension
        double tx_min, tx_max, ty_min, ty_max, tz_min, tz_max;

        // Calculate inverse ray direction for optimization
        double inv_dx = 1.0 / ray.direction.x;
        double inv_dy = 1.0 / ray.direction.y;
        double inv_dz = 1.0 / ray.direction.z;

        // Calculate t values for x-planes
        if (inv_dx >= 0) {
            tx_min = (min.x - ray.origin.x) * inv_dx;
            tx_max = (max.x - ray.origin.x) * inv_dx;
        } else {
            tx_min = (max.x - ray.origin.x) * inv_dx;
            tx_max = (min.x - ray.origin.x) * inv_dx;
        }

        // Calculate t values for y-planes
        if (inv_dy >= 0) {
            ty_min = (min.y - ray.origin.y) * inv_dy;
            ty_max = (max.y - ray.origin.y) * inv_dy;
        } else {
            ty_min = (max.y - ray.origin.y) * inv_dy;
            ty_max = (min.y - ray.origin.y) * inv_dy;
        }

        // If we miss along any dimension, we miss the box
        if (tx_min > ty_max || ty_min > tx_max) {
            return false;
        }

        // Update tmin and tmax
        double t_min = (tx_min > ty_min) ? tx_min : ty_min;
        double t_max = (tx_max < ty_max) ? tx_max : ty_max;

        // Calculate t values for z-planes
        if (inv_dz >= 0) {
            tz_min = (min.z - ray.origin.z) * inv_dz;
            tz_max = (max.z - ray.origin.z) * inv_dz;
        } else {
            tz_min = (max.z - ray.origin.z) * inv_dz;
            tz_max = (min.z - ray.origin.z) * inv_dz;
        }

        // If we miss along the z dimension, we miss the box
        if (t_min > tz_max || tz_min > t_max) {
            return false;
        }

        // Update tmin and tmax
        t_min = (t_min > tz_min) ? t_min : tz_min;
        t_max = (t_max < tz_max) ? t_max : tz_max;

        // Check if the box is behind the ray
        return (t_max > 0);
    }
};

class OptimizedCylinder : public Cylinder {
private:
    AABB boundingBox;
    bool boundingBoxComputed = false;

public:
    using Cylinder::Cylinder;

    // Compute the axis-aligned bounding box for this cylinder
    AABB computeBoundingBox()
    {
        if (boundingBoxComputed) {
            return boundingBox;
        }

        const Math::Point3D& c = getCenter();
        const Math::Vector3D& a = getAxis();
        double r = getRadius();
        double h = getHeight();
        bool isLim = isLimited();

        if (!isLim) {
            // Use a very large but finite value for the bounding box
            double large_value = 1000000000.0;

            Math::Vector3D u, v;

            // Find perpendicular vectors to axis
            if (std::abs(a.x) < std::abs(a.y) && std::abs(a.x) < std::abs(a.z)) {
                // x is smallest, so use x-axis for reference
                u = Math::Vector3D(0, -a.z, a.y).normalize();
            } else if (std::abs(a.y) < std::abs(a.z)) {
                // y is smallest, so use y-axis for reference
                u = Math::Vector3D(-a.z, 0, a.x).normalize();
            } else {
                // z is smallest, so use z-axis for reference
                u = Math::Vector3D(-a.y, a.x, 0).normalize();
            }

            // v = axis × u
            v = Math::Vector3D(
                a.y * u.z - a.z * u.y,
                a.z * u.x - a.x * u.z,
                a.x * u.y - a.y * u.x)
                    .normalize();

            // Compute points that define the bounding box extent
            Math::Point3D min_pt(
                c.x - r * (std::abs(u.x) + std::abs(v.x)) - large_value * std::abs(a.x),
                c.y - r * (std::abs(u.y) + std::abs(v.y)) - large_value * std::abs(a.y),
                c.z - r * (std::abs(u.z) + std::abs(v.z)) - large_value * std::abs(a.z));

            Math::Point3D max_pt(
                c.x + r * (std::abs(u.x) + std::abs(v.x)) + large_value * std::abs(a.x),
                c.y + r * (std::abs(u.y) + std::abs(v.y)) + large_value * std::abs(a.y),
                c.z + r * (std::abs(u.z) + std::abs(v.z)) + large_value * std::abs(a.z));

            boundingBox = AABB(min_pt, max_pt);
        } else {
            // Limited cylinder - more precise bounding box
            Math::Point3D top = c + a * h;

            // Create tight bounds for the cylinder
            Math::Point3D min_pt(
                std::min(c.x - r, top.x - r),
                std::min(c.y - r, top.y - r),
                std::min(c.z - r, top.z - r));

            Math::Point3D max_pt(
                std::max(c.x + r, top.x + r),
                std::max(c.y + r, top.y + r),
                std::max(c.z + r, top.z + r));

            boundingBox = AABB(min_pt, max_pt);
        }

        boundingBoxComputed = true;
        return boundingBox;
    }

    // Check bounding box for early rejection
    double hits(const Ray& ray) const override
    {
        // Check if bounding box has been computed
        if (!boundingBoxComputed) {
            // Compute it if not already done
            const_cast<OptimizedCylinder*>(this)->computeBoundingBox();
        }

        // First check if ray hits the bounding box
        if (!boundingBox.intersect(ray)) {
            return -1; // No intersection with bounding box
        }

        // If ray hits bounding box, do the full cylinder intersection
        return Cylinder::hits(ray);
    }
};

} // namespace Raytracer

#endif /* RAYTRACER_CYLINDER_OPTIMIZATIONS_HPP */

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
