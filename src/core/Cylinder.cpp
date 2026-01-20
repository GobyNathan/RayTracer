/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, May, 2025                                                     *
 * Title           - mirror_raytracer [WSL: Debian]                                     *
 * Description     -                                                                    *
 *     Cylinder                                                                         *
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

#include "Cylinder.hpp"
#include "../factories/MaterialFactory.hpp"
#include "../utils/Debug.hpp"
#include <cmath>
#include <memory>
#include <stdexcept>

namespace Raytracer {

Cylinder::Cylinder(const Math::Point3D& center, const Math::Vector3D& axis,
    double radius, double height,
    std::unique_ptr<IMaterial> material)
    : center(center)
    , axis(axis.normalize())
    , radius(radius)
    , height(height)
    , limited(height > 0)
    , topCenter(limited ? center + axis * height : center)
{
    if (material) {
        this->material = std::move(material);
    }

    Debug::log("Created cylinder: center=(", center.x, ",", center.y, ",", center.z, ")");
    Debug::log("  axis=(", axis.x, ",", axis.y, ",", axis.z, "), radius=", radius, ", height=", height);
}

Cylinder::Cylinder(const libconfig::Setting& settings)
    : ATransformable()
{
    try {
        // Read cylinder base center point
        double x_int = 0, y_int = 0, z_int = 0;
        settings.lookupValue("x", x_int);
        settings.lookupValue("y", y_int);
        settings.lookupValue("z", z_int);
        position = Math::Vector3D(x_int, y_int, z_int);
        center = Math::Point3D(0, 0, 0);

        // Read cylinder axis direction (default to y-axis if not specified)
        double ax_val = 0.0, ay_val = 1.0, az_val = 0.0;
        if (settings.exists("axis")) {
            const libconfig::Setting& axisSetting = settings["axis"];

            int ax_int = 0, ay_int = 1, az_int = 0;
            axisSetting.lookupValue("x", ax_int);
            axisSetting.lookupValue("y", ay_int);
            axisSetting.lookupValue("z", az_int);

            ax_val = static_cast<double>(ax_int);
            ay_val = static_cast<double>(ay_int);
            az_val = static_cast<double>(az_int);

            axisSetting.lookupValue("x", ax_val);
            axisSetting.lookupValue("y", ay_val);
            axisSetting.lookupValue("z", az_val);
        }

        Math::Vector3D rawAxis(ax_val, ay_val, az_val);
        if (rawAxis.length() < 0.001) {
            Debug::log("Warning: cylinder axis too short, defaulting to Y-axis");
            axis = Math::Vector3D(0, 1, 0);
        } else {
            axis = rawAxis.normalize();
        }

        double r_val = 1.0;
        settings.lookupValue("r", r_val);
        radius = r_val;

        int r_int = 1;
        if (settings.lookupValue("r", r_int)) {
            radius = static_cast<double>(r_int);
        }

        // Read cylinder height (0 = infinite)
        double h_val = 0.0;
        settings.lookupValue("height", h_val);
        height = h_val;

        // Try to read as integer too
        int h_int = 0;
        if (settings.lookupValue("height", h_int)) {
            height = static_cast<double>(h_int);
        }

        limited = (height > 0.001);

        // Compute top center if limited
        if (limited) {
            topCenter = center + axis * height;
        } else {
            topCenter = center;
        }

        // Use the base class method to load material
        loadMaterial(settings);

        Debug::log("Cylinder created with center: (", center.x, ", ", center.y, ", ", center.z, ")");
        Debug::log("Axis: (", axis.x, ", ", axis.y, ", ", axis.z, "), radius: ", radius);
        Debug::log("Height: ", height, (limited ? " (limited)" : " (infinite)"));

        if (limited) {
            Debug::log("Top center: (", topCenter.x, ", ", topCenter.y, ", ", topCenter.z, ")");
        }
    } catch (const libconfig::SettingException& ex) {
        throw std::runtime_error(std::string("Error in cylinder parameters: ") + ex.what());
    }
}

bool Cylinder::isWithinHeight(const Math::Point3D& point) const
{
    if (!limited) {
        return true; // Infinite cylinder
    }

    // Calculate vector from center to the point
    Math::Vector3D cp = point - center;

    // Project this vector onto the axis to get the height coordinate
    double projection = cp.dot(axis);

    // Small epsilon for numerical stability
    constexpr double EPSILON = 0.0001;

    // Check if this projection is within [0, height] with a small epsilon for floating point errors
    return (projection >= -EPSILON && projection <= height + EPSILON);
}

Cylinder::HitResult Cylinder::hitSide(const Ray& ray) const
{
    // Calculate quadratic equation coefficients
    // Vector math to remove the component parallel to the cylinder axis
    Math::Vector3D oc = ray.origin - center;

    // For every vector v, we can decompose it into v = v_parallel + v_perpendicular
    // where v_parallel is parallel to the axis and v_perpendicular is perpendicular to the axis
    // v_parallel = (v·axis) * axis
    // v_perpendicular = v - v_parallel

    // Calculate perpendicular components of oc and ray.direction
    double oc_dot_axis = oc.dot(axis);
    Math::Vector3D oc_parallel = axis * oc_dot_axis;
    Math::Vector3D oc_perp = oc - oc_parallel;

    double dir_dot_axis = ray.direction.dot(axis);
    Math::Vector3D dir_parallel = axis * dir_dot_axis;
    Math::Vector3D dir_perp = ray.direction - dir_parallel;

    // Quadratic equation coefficients
    double a = dir_perp.dot(dir_perp);

    // Handle case where ray is parallel to cylinder
    constexpr double EPSILON = 0.0001;
    if (std::abs(a) < EPSILON) {
        // Ray is parallel to cylinder axis
        // Check if ray origin is inside the cylinder
        double distance_from_axis = oc_perp.length();

        if (std::abs(distance_from_axis - radius) < EPSILON) {
            // Ray is traveling along the cylinder surface
            // We consider this a miss to avoid numerical issues
            return HitResult(); // Invalid hit
        }

        if (distance_from_axis < radius) {
            // Ray is inside the cylinder and parallel to axis
            // It will never hit the side (would need to check caps separately)
            return HitResult(); // Invalid hit
        }

        // Ray is outside the cylinder and parallel to axis
        // It will never hit the cylinder
        return HitResult(); // Invalid hit
    }

    double b = 2 * oc_perp.dot(dir_perp);
    double c = oc_perp.dot(oc_perp) - radius * radius;

    // Calculate discriminant
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return HitResult();
    }

    // Calculate the two potential intersection points
    double sqrt_disc = std::sqrt(discriminant);
    double t1 = (-b - sqrt_disc) / (2 * a);
    double t2 = (-b + sqrt_disc) / (2 * a);

    // Get the nearest positive intersection
    double t = -1;
    if (t1 > EPSILON) {
        t = t1;
    } else if (t2 > EPSILON) {
        t = t2;
    } else {
        return HitResult(); // Both intersections are behind the ray origin or too close
    }

    // For infinite cylinder
    if (!limited) {
        return HitResult(t, true, 0);
    }

    // For limited cylinder, we need to check if hit point is within height range
    Math::Point3D hit_point = ray.at(t);

    if (isWithinHeight(hit_point)) {
        return HitResult(t, true, 0);
    }

    // Hit is outside height limits
    return HitResult(); // Invalid hit
}

Cylinder::HitResult Cylinder::hitBottomCap(const Ray& ray) const
{
    if (!limited) {
        return HitResult(); // No caps for infinite cylinder
    }

    // Calculate intersection with bottom cap plane
    constexpr double EPSILON = 0.0001;
    double denom = ray.direction.dot(axis);

    // If ray is parallel to cap plane
    if (std::abs(denom) < EPSILON) {
        return HitResult();
    }

    // Calculate t value for plane intersection
    Math::Vector3D oc = ray.origin - center;
    double t = -oc.dot(axis) / denom;

    // If intersection is behind the ray origin
    if (t < EPSILON) {
        return HitResult();
    }

    Math::Point3D hit_point = ray.at(t);
    Math::Vector3D v = hit_point - center;

    double v_dot_axis = v.dot(axis);
    Math::Vector3D v_radial = v - axis * v_dot_axis;

    if (v_radial.length() <= radius + EPSILON) {
        return HitResult(t, true, 1); // Valid hit on bottom cap
    }

    return HitResult();
}

Cylinder::HitResult Cylinder::hitTopCap(const Ray& ray) const
{
    if (!limited) {
        return HitResult();
    }

    // Calculate intersection with top cap plane
    constexpr double EPSILON = 0.0001;
    double denom = ray.direction.dot(axis);

    // If ray is parallel to cap plane
    if (std::abs(denom) < EPSILON) {
        return HitResult(); // Invalid hit
    }

    // Calculate t value for plane intersection
    Math::Vector3D oc = ray.origin - topCenter;
    double t = -oc.dot(axis) / denom;

    // If intersection is behind the ray origin
    if (t < EPSILON) {
        return HitResult();
    }

    // Check if intersection point is within the cap circle
    Math::Point3D hit_point = ray.at(t);
    Math::Vector3D v = hit_point - topCenter;

    // Remove the component along the axis to get radial vector
    double v_dot_axis = v.dot(axis);
    Math::Vector3D v_radial = v - axis * v_dot_axis;

    // Check if radial distance is less than radius
    if (v_radial.length() <= radius + EPSILON) {
        return HitResult(t, true, 2);
    }

    return HitResult();
}

Cylinder::HitResult Cylinder::getClosestHit(const HitResult& h1, const HitResult& h2, const HitResult& h3) const
{
    HitResult closestHit;
    closestHit.t = std::numeric_limits<double>::max();
    closestHit.isValid = false;

    if (h1.isValid && h1.t < closestHit.t) {
        closestHit = h1;
    }

    if (h2.isValid && h2.t < closestHit.t) {
        closestHit = h2;
    }

    if (h3.isValid && h3.t < closestHit.t) {
        closestHit = h3;
    }

    return closestHit;
}

double Cylinder::localHits(const Ray& ray) const
{
    HitResult sideHit = hitSide(ray);
    HitResult bottomCapHit = hitBottomCap(ray);
    HitResult topCapHit = hitTopCap(ray);

    HitResult closestHit = getClosestHit(sideHit, bottomCapHit, topCapHit);

    if (closestHit.isValid) {
        return closestHit.t;
    }

    return -1;
}

Math::Vector3D Cylinder::localGetNormal(const Math::Point3D& point) const
{
    constexpr double EPSILON = 0.0001;

    // First, determine if we hit a cap or the curved surface
    if (limited) {
        // Check bottom cap - calculate distance from point to base plane
        double dist_to_base = (point - center).dot(axis);
        if (std::abs(dist_to_base) < EPSILON) {
            // We're on the bottom cap
            return -axis; // Normal points toward negative axis direction
        }

        // Check top cap - calculate distance from point to top plane
        double dist_to_top = (point - topCenter).dot(axis);
        if (std::abs(dist_to_top) < EPSILON) {
            // We're on the top cap
            return axis; // Normal points toward positive axis direction
        }
    }

    // If not a cap hit, it's the curved surface
    // Project the point onto the axis to find the center of the circle at that height
    Math::Vector3D cp = point - center;
    double projection = cp.dot(axis);
    Math::Point3D circle_center = center + axis * projection;

    Math::Vector3D normal = point - circle_center;

    // Check if normal length is very small (numerical error)
    double normal_length = normal.length();
    if (normal_length < EPSILON) {
        // Fallback to a perpendicular vector to axis
        Math::Vector3D fallback_vec;
        if (std::abs(axis.x) < 0.9) {
            // axis is not aligned with X, so we can use X axis to create perpendicular
            fallback_vec = Math::Vector3D(1, 0, 0);
        } else {
            // axis is aligned with X, so use Y axis
            fallback_vec = Math::Vector3D(0, 1, 0);
        }

        // Create a perpendicular vector to axis
        // Cross product between two vectors creates a vector perpendicular to both
        Math::Vector3D perp1 = Math::Vector3D(
            axis.y * fallback_vec.z - axis.z * fallback_vec.y,
            axis.z * fallback_vec.x - axis.x * fallback_vec.z,
            axis.x * fallback_vec.y - axis.y * fallback_vec.x);

        Debug::log("Warning: Using fallback normal for cylinder hit at (", point.x, ", ", point.y, ", ", point.z, ")");

        return perp1.normalize();
    }

    return normal.normalize();
}

bool Cylinder::isPlane() const
{
    return false;
}

} // namespace Raytracer