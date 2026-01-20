#ifndef _AMBIENT_LIGHT_HPP
#define _AMBIENT_LIGHT_HPP

#include "../interfaces/ILight.hpp"
#include "../utils/Debug.hpp"
#include "Point3D.hpp"
#include <stdexcept>

namespace Raytracer {

class AmbientLight : public ILight {
private:
    Math::Point3D _origin;
    float _intensity;
    ShadingModel _shadingModel;

public:
    AmbientLight(const Math::Point3D& origin, float intensity)
        : _origin(origin)
        , _intensity(intensity)
    {
    }
    AmbientLight(const libconfig::Setting& settings)
    {
        try {
            double x, y, z;
            settings.lookupValue("x", x);
            settings.lookupValue("y", y);
            settings.lookupValue("z", z);
            _origin = Math::Point3D(x, y, z);

            settings.lookupValue("intensity", _intensity);

            int shadingStore;
            settings.lookupValue("shading_model", shadingStore);
            _shadingModel = static_cast<ShadingModel>(shadingStore);

            // Debug::log("Ambient light created with origin: ", x_int, " ", y_int, "
            // ", z_int, " and intensity: ", _intensity);
        } catch (const libconfig::SettingException& ex) {
            throw std::runtime_error(
                std::string("Error in ambient light parameters: ") + ex.what());
        }
    }

    void setOrigin(const Math::Point3D& origin) { _origin = origin; }
    void setIntensity(float intensity) { _intensity = intensity; }

    Math::Point3D getOrigin() const override { return _origin; }
    float getIntensity() const override { return _intensity; }

    bool isAmbientLight() const override { return true; }
    bool isDirectionalLight() const override { return false; }
    ShadingModel getShadingModel() const override { return _shadingModel; };

    Math::Vector3D getDirection() const override
    {
        return Math::Vector3D(0, 1, 0);
    }
};

} // namespace Raytracer

#endif /* _AMBIENT_LIGHT_HPP */
