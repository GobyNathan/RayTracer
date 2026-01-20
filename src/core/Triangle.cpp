/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Tue, May, 2025 * Title           - mirror_raytracer *
 * Description     - * Triangle *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗ *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║ * █████╗
 * ██████╔╝██║   ██║   █████╗  ██║     ███████║                     * ██╔══╝
 * ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║ *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝ *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#include "Triangle.hpp"
#include "../utils/Debug.hpp"
#include <iostream>

namespace Raytracer {

Triangle::Triangle(const Math::Point3D &v1, const Math::Point3D &v2,
                   const Math::Point3D &v3, std::unique_ptr<IMaterial> material)
    : v1(v1), v2(v2), v3(v3) {
  if (material) {
    this->material = std::move(material);
  }
}

Triangle::Triangle(const libconfig::Setting &settings) : ATransformable() {
  try {
    const libconfig::Setting &vertices = settings["vertices"];
    const libconfig::Setting &v1Settings = vertices["v1"];
    const libconfig::Setting &v2Settings = vertices["v2"];
    const libconfig::Setting &v3Settings = vertices["v3"];

    double x = 0, y = 0, z = 0;

    Math::Point3D w1, w2, w3;

    v1Settings.lookupValue("x", x);
    v1Settings.lookupValue("y", y);
    v1Settings.lookupValue("z", z);
    w1 = Math::Point3D(x, y, z);

    v2Settings.lookupValue("x", x);
    v2Settings.lookupValue("y", y);
    v2Settings.lookupValue("z", z);
    w2 = Math::Point3D(x, y, z);

    v3Settings.lookupValue("x", x);
    v3Settings.lookupValue("y", y);
    v3Settings.lookupValue("z", z);
    w3 = Math::Point3D(x, y, z);

    Math::Point3D centroid((w1.x + w2.x + w3.x) / 3.0,
                           (w1.y + w2.y + w3.y) / 3.0,
                           (w1.z + w2.z + w3.z) / 3.0);

    position = Math::Vector3D(centroid.x, centroid.y, centroid.z);

    v1 = Math::Point3D(w1.x - centroid.x, w1.y - centroid.y, w1.z - centroid.z);
    v2 = Math::Point3D(w2.x - centroid.x, w2.y - centroid.y, w2.z - centroid.z);
    v3 = Math::Point3D(w3.x - centroid.x, w3.y - centroid.y, w3.z - centroid.z);

    loadMaterial(settings);
    loadTransforms(settings);
  } catch (const libconfig::SettingException &ex) {
    Debug::log("Error loading triangle: ", ex.what());
    throw;
  }
}

double Triangle::localHits(const Ray &ray) const {
  const double EPSILON = 0.0000001;

  Math::Vector3D edge1 = v2 - v1;
  Math::Vector3D edge2 = v3 - v1;

  Math::Vector3D pvec = ray.direction.cross(edge2);

  double det = edge1.dot(pvec);
  if (std::abs(det) < EPSILON)
    return -1;

  double invDet = 1.0 / det;

  Math::Vector3D tvec = ray.origin - v1;

  double u = tvec.dot(pvec) * invDet;
  if (u < 0.0 || u > 1.0)
    return -1;

  Math::Vector3D qvec = tvec.cross(edge1);

  double v = ray.direction.dot(qvec) * invDet;
  if (v < 0.0 || u + v > 1.0)
    return -1;

  double t = edge2.dot(qvec) * invDet;

  return t > EPSILON ? t : -1;
}

Math::Vector3D Triangle::localGetNormal(const Math::Point3D &) const {
  Math::Vector3D edge1 = v2 - v1;
  Math::Vector3D edge2 = v3 - v1;
  Math::Vector3D normal = edge1.cross(edge2);

  double length = normal.length();
  if (length > 0) {
    normal =
        Math::Vector3D(normal.x / length, normal.y / length, normal.z / length);
  }

  return normal;
}

bool Triangle::isPlane() const { return false; }

} // namespace Raytracer
