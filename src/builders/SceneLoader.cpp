/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2025                                                     *
 * Title           - mirror_raytracer                                                   *
 * Description     -                                                                    *
 *     SceneLoader                                                                      *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "SceneLoader.hpp"
#include "../core/AmbiantLight.hpp"
#include "../core/Cone.hpp"
#include "../core/Cube.hpp"
#include "../core/Cylinder.hpp"
#include "../core/DirectionalLight.hpp"
#include "../core/Plane.hpp"
#include "../core/PointLight.hpp"
#include "../core/Scene.hpp"
#include "../core/Sphere.hpp"
#include "../core/Triangle.hpp"
#include "../factories/LightFactory.hpp"
#include "../factories/MaterialFactory.hpp"
#include "../factories/PrimitiveFactory.hpp"
#include "../utils/Debug.hpp"
#include <array>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Raytracer {

SceneLoader::SceneLoader(SceneBuilder& builder)
    : builder(builder)
{
    // Register primitives types
    Raytracer::MaterialFactory::registerAllMaterials();
    PrimitiveFactory::registerType<Raytracer::Sphere>("sphere");
    PrimitiveFactory::registerType<Plane>("plane");
    PrimitiveFactory::registerType<Cylinder>("cylinder");
    PrimitiveFactory::registerType<Raytracer::Cone>("cone");
    PrimitiveFactory::registerType<Raytracer::Cube>("cube");
    PrimitiveFactory::registerType<Raytracer::Triangle>("triangle");

    // Register light types
    LightFactory::registerType<DirectionalLight>("directional");
    LightFactory::registerType<AmbientLight>("ambient");
    LightFactory::registerType<PointLight>("point");
}

std::unique_ptr<Scene> SceneLoader::loadSceneFromFile(const std::string& filename)
{
    Debug::log("Loading scene from file: ", filename);
    libconfig::Config cfg;
    try {
        cfg.readFile(filename.c_str());
        Debug::log("Successfully read config file");
    } catch (const libconfig::FileIOException& e) {
        throw std::runtime_error("Error reading config file: " + filename);
    } catch (const libconfig::ParseException& e) {
        throw std::runtime_error("Error parsing config file: " + filename);
    }

    const libconfig::Setting& root = cfg.getRoot();

    if (root.exists("camera"))
        loadCamera(root["camera"]);
    if (root.exists("primitives"))
        loadPrimitives(root["primitives"]);
    if (root.exists("objects"))
        loadObjects(root["objects"]);
    if (root.exists("lights"))
        loadLights(root["lights"]);

    return builder.build();
}

void SceneLoader::loadObjects(const libconfig::Setting& objects)
{
    Debug::log("Loading objects from OBJ files");

    for (int i = 0; i < objects.getLength(); ++i) {
        const auto& obj = objects[i];
        std::string filepath;
        if (!obj.lookupValue("file", filepath))
            throw std::runtime_error("Missing file path in object configuration");

        Math::Point3D position(0, 0, 0);
        if (obj.exists("position")) {
            double x = 0, y = 0, z = 0;
            obj["position"].lookupValue("x", x);
            obj["position"].lookupValue("y", y);
            obj["position"].lookupValue("z", z);
            position = Math::Point3D(x, y, z);
        }

        const libconfig::Setting* materialSettings = nullptr;
        if (obj.exists("material")) {
            materialSettings = &obj["material"];
        }

        loadObjFile(filepath, position, materialSettings);
    }
}

void SceneLoader::loadObjFile(const std::string& filepath,
    const Math::Point3D& position,
    const libconfig::Setting* materialSettings)
{
    Debug::log("Loading OBJ file: ", filepath);
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open OBJ file: " + filepath);
    }

    auto vertices = parseVertices(file);
    file.clear();
    file.seekg(0);
    auto faces = parseFaces(file);

    for (const auto& face : faces) {
        if (face[0] >= vertices.size() || face[1] >= vertices.size() || face[2] >= vertices.size()) {
            Debug::log("Invalid face indices in OBJ file");
            continue;
        }

        Math::Point3D v1(vertices[face[0]].x + position.x,
            vertices[face[0]].y + position.y,
            vertices[face[0]].z + position.z);
        Math::Point3D v2(vertices[face[1]].x + position.x,
            vertices[face[1]].y + position.y,
            vertices[face[1]].z + position.z);
        Math::Point3D v3(vertices[face[2]].x + position.x,
            vertices[face[2]].y + position.y,
            vertices[face[2]].z + position.z);

        std::unique_ptr<IMaterial> material = nullptr;
        if (materialSettings) {
            std::string materialType = "matte";
            materialSettings->lookupValue("type", materialType);
            material = MaterialFactory::createMaterial(materialType, *materialSettings);
        }
        auto triangle = std::make_unique<Triangle>(v1, v2, v3, std::move(material));
        builder.addPrimitive(std::move(triangle));
    }
}

void SceneLoader::loadPrimitives(const libconfig::Setting& primitives)
{
    for (const auto& type : PrimitiveFactory::getRegisteredTypes()) {
        std::string configKey = type + "s";

        if (primitives.exists(configKey)) {
            const libconfig::Setting& elements = primitives[configKey.c_str()];
            Debug::log("Found ", elements.getLength(), " ", type, "s in config");

            for (int i = 0; i < elements.getLength(); ++i) {
                Debug::log("Creating ", type, " ", i + 1);
                builder.addPrimitive(PrimitiveFactory::createPrimitive(type, elements[i]));
            }
        }
    }
}

std::vector<Math::Point3D> SceneLoader::parseVertices(std::ifstream& file)
{
    std::vector<Math::Point3D> vertices;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        char type;
        iss >> type;

        if (type == 'v') {
            double x, y, z;
            if (iss >> x >> y >> z) {
                vertices.emplace_back(x, y, z);
            }
        }
    }
    return vertices;
}

std::vector<std::array<size_t, 3>> SceneLoader::parseFaces(std::ifstream& file)
{
    std::vector<std::array<size_t, 3>> faces;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        char type;
        iss >> type;

        if (type == 'f') {
            std::string v1_full, v2_full, v3_full, v4_full;
            if (!(iss >> v1_full >> v2_full >> v3_full)) {
                continue;
            }
            bool isQuad = false;
            if (iss >> v4_full) {
                isQuad = true;
            }

            auto getVertexIndex = [](const std::string& vertex_str) -> size_t {
                return std::stoul(vertex_str.substr(0, vertex_str.find('/'))) - 1;
            };

            size_t v1 = getVertexIndex(v1_full);
            size_t v2 = getVertexIndex(v2_full);
            size_t v3 = getVertexIndex(v3_full);

            faces.push_back({ v1, v2, v3 });

            if (isQuad) {
                size_t v4 = getVertexIndex(v4_full);
                faces.push_back({ v1, v3, v4 });
            }
        }
    }
    return faces;
}

void SceneLoader::loadLights(const libconfig::Setting& lights)
{
    for (const auto& type : LightFactory::getRegisteredTypes()) {
        if (lights.exists(type)) {
            const libconfig::Setting& elements = lights[type.c_str()];
            Debug::log("Found ", elements.getLength(), " ", type, " lights");
            for (int i = 0; i < elements.getLength(); ++i) {
                Debug::log("Creating ", type, " light ", i + 1);
                builder.addLight(LightFactory::createLight(type, elements[i]));
            }
        }
    }
}

void SceneLoader::loadCamera(const libconfig::Setting& camera)
{
    Debug::log("Loading camera configuration");

    // Default values
    int posX = 0.0, posY = 0.0, posZ = 0.0;
    int rotX = 0.0, rotY = 0.0, rotZ = 0.0;
    int width = 1920, height = 1080;
    double fov = 72.0;

    try {
        if (camera.exists("position")) {
            const libconfig::Setting& pos = camera["position"];
            pos.lookupValue("x", posX);
            pos.lookupValue("y", posY);
            pos.lookupValue("z", posZ);
            Debug::log("Found camera position (", posX, ", ", posY, ", ", posZ, ")");
        }

        if (camera.exists("resolution")) {
            const libconfig::Setting& res = camera["resolution"];
            res.lookupValue("width", width);
            res.lookupValue("height", height);
            Debug::log("Found camera resolution ", width, "x", height);
        }

        if (camera.exists("rotation")) {
            const libconfig::Setting& rot = camera["rotation"];
            rot.lookupValue("x", rotX);
            rot.lookupValue("y", rotY);
            rot.lookupValue("z", rotZ);
            Debug::log("Found camera rotation (", rotX, ", ", rotY, ", ", rotZ, ")");
        }

        camera.lookupValue("fieldOfView", fov);
    } catch (const libconfig::SettingNotFoundException& e) {
        Debug::log("Warning: Some camera settings not found, using defaults");
    } catch (const libconfig::SettingTypeException& e) {
        Debug::log("Warning: Invalid type in camera settings, using defaults");
    }

    Camera& cam = builder.setCamera(Math::Point3D(posX, posY, posZ));
    builder.setScreen(width, height);
    cam.setRotation(rotX, rotY, rotZ);
    cam.setFieldOfView(fov);

    Debug::log("Camera configuration completed");
}

} // namespace Raytracer
