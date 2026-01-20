/*
 * < Kawabonga ! >
 * ------------------------------
 *    \   ^__^
 *     \  (oo)\_______
 *        (__)\       )\/\
 *            ||----w |
 *            ||     ||
 *
 */

#include "src/Camera.hpp"
#include "src/Sphere.hpp"
#include <algorithm>
#include <iostream>

void writeColor(const Math::Vector3D& color)
{
    int r = (int)(255 * std::clamp(color.x, 0.0, 1.0));
    int g = (int)(255 * std::clamp(color.y, 0.0, 1.0));
    int b = (int)(255 * std::clamp(color.z, 0.0, 1.0));

    std::cout << r << " " << g << " " << b << std::endl;
}

int main()
{
    const int width = 100;
    const int height = 50;

    Raytracer::Camera camera;
    Raytracer::Sphere sphere(Math::Point3D(0, 0, -1), 0.5);

    std::cout << "P3" << std::endl
              << width << " " << height << std::endl
              << "255" << std::endl;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double u = (double)x / (width - 1);
            double v = (double)y / (height - 1);
            Raytracer::Ray ray = camera.ray(u, v);

            if (sphere.hits(ray)) {
                writeColor(Math::Vector3D(1.0, 0.0, 0.0));
            } else {
                writeColor(Math::Vector3D(0.0, 0.0, 1.0));
            }
        }
        std::cout << std::endl;
    }
    return 0;
}
