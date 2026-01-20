/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2025 * Title           - mirror_raytracer *
 * Description     - * ATransformable *
 *                                                                                     *
 * ------------------------------------------------------------------------------------
 */

#ifndef RAYTRACER_ATRANSFORMABLE_HPP_
#define RAYTRACER_ATRANSFORMABLE_HPP_

#include "../core/Ray.hpp"
#include "../core/Vector3D.hpp"
#include "../interfaces/APrimitive.hpp"
#include "../utils/Debug.hpp"
#include <cmath>
#include <libconfig.h++>
#include <mutex>

namespace Raytracer {

class ATransformable : public APrimitive {
protected:
    mutable std::mutex transformMutex;
    Math::Vector3D translation;
    Math::Vector3D rotation;
    Math::Vector3D position;

    Math::Point3D applyRotationX(const Math::Point3D& point) const
    {
        double cosX = cos(rotation.x);
        double sinX = sin(rotation.x);
        return Math::Point3D(point.x, point.y * cosX - point.z * sinX,
            point.y * sinX + point.z * cosX);
    }

    Math::Point3D applyRotationY(const Math::Point3D& point) const
    {
        double cosY = cos(rotation.y);
        double sinY = sin(rotation.y);
        return Math::Point3D(point.x * cosY + point.z * sinY, point.y,
            -point.x * sinY + point.z * cosY);
    }

    Math::Point3D applyRotationZ(const Math::Point3D& point) const
    {
        double cosZ = cos(rotation.z);
        double sinZ = sin(rotation.z);
        return Math::Point3D(point.x * cosZ - point.y * sinZ,
            point.x * sinZ + point.y * cosZ, point.z);
    }

    Math::Vector3D applyRotationX(const Math::Vector3D& vec) const
    {
        double cosX = cos(rotation.x);
        double sinX = sin(rotation.x);
        return Math::Vector3D(vec.x, vec.y * cosX - vec.z * sinX,
            vec.y * sinX + vec.z * cosX);
    }

    Math::Vector3D applyRotationY(const Math::Vector3D& vec) const
    {
        double cosY = cos(rotation.y);
        double sinY = sin(rotation.y);
        return Math::Vector3D(vec.x * cosY + vec.z * sinY, vec.y,
            -vec.x * sinY + vec.z * cosY);
    }

    Math::Vector3D applyRotationZ(const Math::Vector3D& vec) const
    {
        double cosZ = cos(rotation.z);
        double sinZ = sin(rotation.z);
        return Math::Vector3D(vec.x * cosZ - vec.y * sinZ,
            vec.x * sinZ + vec.y * cosZ, vec.z);
    }

    Math::Point3D applyRotation(const Math::Point3D& point) const
    {
        return applyRotationX(applyRotationY(applyRotationZ(point)));
    }

    Math::Vector3D applyRotation(const Math::Vector3D& vec) const
    {
        return applyRotationX(applyRotationY(applyRotationZ(vec)));
    }

    Math::Point3D applyInverseRotation(const Math::Point3D& point) const
    {
        Math::Vector3D savedRotation = rotation;
        const_cast<Math::Vector3D&>(rotation) = Math::Vector3D(-rotation.x, -rotation.y, -rotation.z);
        Math::Point3D result = applyRotationZ(applyRotationY(applyRotationX(point)));
        const_cast<Math::Vector3D&>(rotation) = savedRotation;
        return result;
    }

    Math::Vector3D applyInverseRotation(const Math::Vector3D& vec) const
    {
        Math::Vector3D savedRotation = rotation;
        const_cast<Math::Vector3D&>(rotation) = Math::Vector3D(-rotation.x, -rotation.y, -rotation.z);
        Math::Vector3D result = applyRotationZ(applyRotationY(applyRotationX(vec)));
        const_cast<Math::Vector3D&>(rotation) = savedRotation;
        return result;
    }

    ATransformable()
        : APrimitive()
        , translation(Math::Vector3D(0, 0, 0))
        , rotation(Math::Vector3D(0, 0, 0))
        , position(Math::Vector3D(0, 0, 0))
    {
    }

    ATransformable(const ATransformable& other)
        : APrimitive(other)
        , translation(other.translation)
        , rotation(other.rotation)
        , position(other.position)
    {
    }

    ATransformable& operator=(const ATransformable& other)
    {
        if (this != &other) {
            APrimitive::operator=(other);
            translation = other.translation;
            rotation = other.rotation;
            position = other.position;
        }
        return *this;
    }

    virtual void loadTransforms(const libconfig::Setting& settings)
    {
        std::lock_guard<std::mutex> lock(transformMutex);

        try {
            if (settings.exists("transforms")) {
                const libconfig::Setting& transforms = settings["transforms"];
                if (transforms.exists("translation")) {
                    const libconfig::Setting& trans = transforms["translation"];
                    double x = 0, y = 0, z = 0;
                    trans.lookupValue("x", x);
                    trans.lookupValue("y", y);
                    trans.lookupValue("z", z);
                    translation = Math::Vector3D(x, y, z);
                }
                if (transforms.exists("rotation")) {
                    const libconfig::Setting& rot = transforms["rotation"];
                    double x = 0, y = 0, z = 0;
                    rot.lookupValue("x", x);
                    rot.lookupValue("y", y);
                    rot.lookupValue("z", z);
                    rotation = Math::Vector3D(x * M_PI / 180.0, y * M_PI / 180.0,
                        z * M_PI / 180.0);
                }
            }
        } catch (const libconfig::SettingException& ex) {
            Debug::log("Error loading transforms: ", ex.what());
        }
    }

    virtual Math::Point3D applyTransforms(const Math::Point3D& point) const
    {
        Math::Vector3D localTranslation, localRotation, localPosition;

        {
            std::lock_guard<std::mutex> lock(transformMutex);
            localTranslation = translation;
            localRotation = rotation;
            localPosition = position;
        }

        double cosZ = cos(localRotation.z);
        double sinZ = sin(localRotation.z);
        Math::Point3D rotZ(point.x * cosZ - point.y * sinZ,
            point.x * sinZ + point.y * cosZ, point.z);

        double cosY = cos(localRotation.y);
        double sinY = sin(localRotation.y);
        Math::Point3D rotY(rotZ.x * cosY + rotZ.z * sinY, rotZ.y,
            -rotZ.x * sinY + rotZ.z * cosY);

        double cosX = cos(localRotation.x);
        double sinX = sin(localRotation.x);
        Math::Point3D rotated(rotY.x, rotY.y * cosX - rotY.z * sinX,
            rotY.y * sinX + rotY.z * cosX);

        return Math::Point3D(rotated.x + localTranslation.x + localPosition.x,
            rotated.y + localTranslation.y + localPosition.y,
            rotated.z + localTranslation.z + localPosition.z);
    }

    virtual Math::Point3D reverseTransforms(const Math::Point3D& point) const
    {
        Math::Vector3D localTranslation, localRotation, localPosition;

        {
            std::lock_guard<std::mutex> lock(transformMutex);
            localTranslation = translation;
            localRotation = rotation;
            localPosition = position;
        }

        Math::Point3D translated(point.x - localTranslation.x - localPosition.x,
            point.y - localTranslation.y - localPosition.y,
            point.z - localTranslation.z - localPosition.z);

        Math::Vector3D invRotation(-localRotation.x, -localRotation.y,
            -localRotation.z);

        double cosX = cos(invRotation.x);
        double sinX = sin(invRotation.x);
        Math::Point3D rotX(translated.x, translated.y * cosX - translated.z * sinX,
            translated.y * sinX + translated.z * cosX);

        double cosY = cos(invRotation.y);
        double sinY = sin(invRotation.y);
        Math::Point3D rotY(rotX.x * cosY + rotX.z * sinY, rotX.y,
            -rotX.x * sinY + rotX.z * cosY);

        double cosZ = cos(invRotation.z);
        double sinZ = sin(invRotation.z);
        return Math::Point3D(rotY.x * cosZ - rotY.y * sinZ,
            rotY.x * sinZ + rotY.y * cosZ, rotY.z);
    }

    Ray transformRay(const Ray& ray) const
    {
        Math::Vector3D localTranslation, localRotation, localPosition;

        {
            std::lock_guard<std::mutex> lock(transformMutex);
            localTranslation = translation;
            localRotation = rotation;
            localPosition = position;
        }

        Math::Point3D translatedOrigin(
            ray.origin.x - localTranslation.x - localPosition.x,
            ray.origin.y - localTranslation.y - localPosition.y,
            ray.origin.z - localTranslation.z - localPosition.z);

        Math::Vector3D invRotation(-localRotation.x, -localRotation.y,
            -localRotation.z);

        double cosX = cos(invRotation.x);
        double sinX = sin(invRotation.x);
        Math::Point3D rotXOrigin(
            translatedOrigin.x,
            translatedOrigin.y * cosX - translatedOrigin.z * sinX,
            translatedOrigin.y * sinX + translatedOrigin.z * cosX);
        Math::Vector3D rotXDir(ray.direction.x,
            ray.direction.y * cosX - ray.direction.z * sinX,
            ray.direction.y * sinX + ray.direction.z * cosX);

        double cosY = cos(invRotation.y);
        double sinY = sin(invRotation.y);
        Math::Point3D rotYOrigin(rotXOrigin.x * cosY + rotXOrigin.z * sinY,
            rotXOrigin.y,
            -rotXOrigin.x * sinY + rotXOrigin.z * cosY);
        Math::Vector3D rotYDir(rotXDir.x * cosY + rotXDir.z * sinY, rotXDir.y,
            -rotXDir.x * sinY + rotXDir.z * cosY);

        double cosZ = cos(invRotation.z);
        double sinZ = sin(invRotation.z);
        Math::Point3D finalOrigin(rotYOrigin.x * cosZ - rotYOrigin.y * sinZ,
            rotYOrigin.x * sinZ + rotYOrigin.y * cosZ,
            rotYOrigin.z);
        Math::Vector3D finalDir(rotYDir.x * cosZ - rotYDir.y * sinZ,
            rotYDir.x * sinZ + rotYDir.y * cosZ, rotYDir.z);

        double len = finalDir.length();
        if (len != 0.0) {
            finalDir = Math::Vector3D(finalDir.x / len, finalDir.y / len, finalDir.z / len);
        }

        return Ray(finalOrigin, finalDir);
    }

    virtual double localHits(const Ray& localRay) const = 0;

    virtual Math::Vector3D
    localGetNormal(const Math::Point3D& localPoint) const
        = 0;

public:
    virtual ~ATransformable() = default;

    double hits(const Ray& ray) const override final
    {
        Ray localRay = transformRay(ray);
        return localHits(localRay);
    }

    Math::Vector3D getNormal(const Math::Point3D& point) const override final
    {
        Math::Vector3D localRotation;

        Math::Point3D localPoint = reverseTransforms(point);

        Math::Vector3D localNormal = localGetNormal(localPoint);

        {
            std::lock_guard<std::mutex> lock(transformMutex);
            localRotation = rotation;
        }

        double cosZ = cos(localRotation.z);
        double sinZ = sin(localRotation.z);
        Math::Vector3D rotZ(localNormal.x * cosZ - localNormal.y * sinZ,
            localNormal.x * sinZ + localNormal.y * cosZ,
            localNormal.z);

        double cosY = cos(localRotation.y);
        double sinY = sin(localRotation.y);
        Math::Vector3D rotY(rotZ.x * cosY + rotZ.z * sinY, rotZ.y,
            -rotZ.x * sinY + rotZ.z * cosY);

        double cosX = cos(localRotation.x);
        double sinX = sin(localRotation.x);
        return Math::Vector3D(rotY.x, rotY.y * cosX - rotY.z * sinX,
            rotY.y * sinX + rotY.z * cosX);
    }
};

} // namespace Raytracer

#endif /* RAYTRACER_ATRANSFORMABLE_HPP_ */
