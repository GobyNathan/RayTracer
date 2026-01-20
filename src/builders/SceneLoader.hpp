#ifndef RAYTRACER_SCENELOADER_HPP_
#define RAYTRACER_SCENELOADER_HPP_

#include "SceneBuilder.hpp"
#include <libconfig.h++>
#include <memory>
#include <string>

namespace Raytracer {

class Scene;

class SceneLoader {
public:
    SceneLoader(SceneBuilder& builder);
    ~SceneLoader() = default;

    std::unique_ptr<Scene> loadSceneFromFile(const std::string& filename);

private:
    void loadPrimitives(const libconfig::Setting& primitives);
    void loadLights(const libconfig::Setting& lights);
    void loadCamera(const libconfig::Setting& camera);
    void loadObjects(const libconfig::Setting& objects);

private:
    void loadObjFile(const std::string& filepath,
        const Math::Point3D& position,
        const libconfig::Setting* materialSettings = nullptr);
    std::vector<Math::Point3D> parseVertices(std::ifstream& file);
    std::vector<std::array<size_t, 3>> parseFaces(std::ifstream& file);
    SceneBuilder& builder;
};

}

#endif /* !RAYTRACER_SCENELOADER_HPP_ */
