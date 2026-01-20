#ifndef RAYTRACER_SCENEBUILDER_HPP_
#define RAYTRACER_SCENEBUILDER_HPP_

#include "../core/Camera.hpp"
#include "../core/Rectangle3D.hpp"
#include "../core/Scene.hpp"
#include "../interfaces/ILight.hpp"
#include "../interfaces/IPrimitive.hpp"
#include <memory>
#include <vector>

namespace Raytracer {

class SceneBuilder {
public:
    SceneBuilder()
        : camera(std::make_unique<Camera>(Math::Point3D(0, 0, 0),
              Math::Point3D(0, 0, -1), 1920, 1080))
    {
    }
    ~SceneBuilder() = default;

    // Delete copy constructor/assignment
    SceneBuilder(const SceneBuilder&) = delete;
    SceneBuilder& operator=(const SceneBuilder&) = delete;

    // Implement move operations
    SceneBuilder(SceneBuilder&&) noexcept = default;
    SceneBuilder& operator=(SceneBuilder&&) noexcept = default;

    SceneBuilder& addPrimitive(std::unique_ptr<IPrimitive> primitive);
    SceneBuilder& addLight(std::unique_ptr<ILight> light);

    SceneBuilder& setCamera(std::unique_ptr<Raytracer::Camera> camera);
    SceneBuilder& setScreen(std::unique_ptr<Math::Rectangle3D> screen);

    std::vector<std::unique_ptr<IPrimitive>>& getPrimitives();
    std::vector<std::unique_ptr<ILight>>& getLights();

    Camera& setCamera(const Math::Point3D& position);
    Camera& setScreen(int width, int height);
    Camera& getCamera() { return *camera; }
    std::unique_ptr<Scene> build();

private:
    std::vector<std::unique_ptr<IPrimitive>> primitives;
    std::vector<std::unique_ptr<ILight>> lights;
    std::unique_ptr<Raytracer::Camera> camera;
    std::unique_ptr<Math::Rectangle3D> screen;
};

} // namespace Raytracer

#endif /* !RAYTRACER_SCENEBUILDER_HPP_ */
