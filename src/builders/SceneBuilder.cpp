/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Apr, 2025 * Title           - mirror_raytracer *
 * Description     - * SceneBuilder *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄ * ▐
 * ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀ * █▄▄▄▄▄  ▐
 * █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             * █    ▌
 * █          █        █        █    ▌    █         █   █             * ▄▀▄▄▄▄
 * ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             * █    ▐
 * █         █       █ █          █    ▐   █     ▐   █   █               * ▐ ▐
 * ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#include "SceneBuilder.hpp"
#include "../utils/Debug.hpp"
#include <stdexcept>
#include <vector>

namespace Raytracer {

SceneBuilder&
SceneBuilder::addPrimitive(std::unique_ptr<IPrimitive> primitive)
{
    if (!primitive)
        throw std::invalid_argument("Cannot add null primitive");
    primitives.push_back(std::move(primitive));
    Debug::log("Primitive added to scene");
    return *this;
}

SceneBuilder& SceneBuilder::addLight(std::unique_ptr<ILight> light)
{
    if (!light)
        throw std::invalid_argument("Cannot add null light");
    lights.push_back(std::move(light));
    return *this;
}

Camera& SceneBuilder::setCamera(const Math::Point3D& position)
{
    camera->setPosition(position.x, position.y, position.z);
    return *camera;
}

Camera& SceneBuilder::setScreen(int width, int height)
{
    camera->setResolution(width, height);
    return *camera;
}

std::unique_ptr<Scene> SceneBuilder::build()
{
    if (!camera)
        throw std::runtime_error("Cannot build scene without camera");

    auto scene = std::make_unique<Scene>();
    // TODO: Initialize scene with camera, screen, primitives and lights
    // Implementation will be added when Scene class is created
    return scene;
}

std::vector<std::unique_ptr<IPrimitive>>& SceneBuilder::getPrimitives()
{
    return primitives;
}

std::vector<std::unique_ptr<ILight>>& SceneBuilder::getLights()
{
    return lights;
}

} // namespace Raytracer
