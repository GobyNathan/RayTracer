/*
** EPITECH PROJECT, 2025
** mirror_raytracer
** File description:
** Renderer.hpp
*/
#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "../builders/SceneBuilder.hpp"
#include "../interfaces/IMaterialInteraction.hpp"
#include "../utils/Debug.hpp"
#include "../utils/Timer.hpp"
#include "LightRenderer/LightRenderer.hpp"
#include "PrimitiveRenderer/PrimitiveRenderer.hpp"
#include <random>
#include <unordered_map>
#include <vector>

namespace Raytracer {

class Renderer {
private:
    SceneBuilder _scene;
    int _width;
    int _height;
    int _maxDepth;
    int _samples;
    Math::Vector3D _backgroundColor;
    Timer renderTimer;
    std::vector<std::string> renderResult;

    std::unique_ptr<LightRenderer> _lightRenderer;
    std::unique_ptr<PrimitiveRenderer> _primitiveRenderer;

    // Method for random numbers (for metal roughness)
    double randomDouble(double min, double max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(min, max);
        return dis(gen);
    }

public:
    Renderer(SceneBuilder scene, int width, int height, int maxDepth = 5,
        int samples = 50);

    void render();
    Math::Vector3D traceRay(Ray& ray, int depth);
    std::string drawPixel(const Math::Vector3D& color);
};

} // namespace Raytracer
#endif /* RENDERER_HPP */
