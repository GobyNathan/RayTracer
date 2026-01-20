/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Apr, 2025 * Title           - mirror_raytracer *
 * Description     - * Camera *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░ * ▒
 * ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓ * █
 * ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █ *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#ifndef RAYTRACER_CAMERA_HPP_
#define RAYTRACER_CAMERA_HPP_

#include "Ray.hpp"
#include "Rectangle3D.hpp"

namespace Raytracer {

struct Resolution {
    int width;
    int height;
};

class Camera {
public:
    Math::Point3D origin;
    Math::Rectangle3D screen;

    Camera(Math::Point3D origin, Math::Point3D screenOrigin, int resX, int resY);
    Camera(const Camera& other);
    ~Camera() = default;

    Camera& operator=(const Camera& other);

    void setPosition(double x, double y, double z);
    void setPosition(const Math::Point3D& position);

    Math::Vector3D updateRotation(const Math::Vector3D& vector) const;
    void updateScreenOrientation();
    void setRotation(double x, double y, double z);
    void setRotation(const Math::Vector3D& rotation);

    void setFieldOfView(double fov);
    void setResolution(int width, int height);

    const Resolution& getResolution() const { return resolution; }
    const Math::Point3D& getPosition() const { return origin; }
    const Math::Vector3D& getRotation() const { return rotation; }
    double getFieldOfView() const { return fieldOfView; }

    Ray ray(double u, double v) const;

private:
    Resolution resolution;
    Math::Vector3D rotation;
    double fieldOfView;
};

} // namespace Raytracer

#endif /* !RAYTRACER_CAMERA_HPP_ */
