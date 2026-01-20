#ifndef _DIRECTIONAL_LIGHT_HPP
#define _DIRECTIONAL_LIGHT_HPP

#include "../interfaces/ILight.hpp"
#include "Point3D.hpp"
#include "Vector3D.hpp"
#include <stdexcept>

namespace Raytracer {

class DirectionalLight : public ILight {
private:
    Math::Vector3D _direction;
    float _intensity;
    ShadingModel _shadingModel;

public:
    DirectionalLight(const Math::Vector3D& direction, float intensity)
        : _direction(direction.normalize())
        , _intensity(intensity)
    {
    }

    DirectionalLight(const libconfig::Setting& settings)
    {
        try {
            double x, y, z;
            settings.lookupValue("x", x);
            settings.lookupValue("y", y);
            settings.lookupValue("z", z);
            _direction = Math::Vector3D(x, y, z);

            settings.lookupValue("intensity", _intensity);

            int shadingStore;
            settings.lookupValue("shading_model", shadingStore);
            _shadingModel = static_cast<ShadingModel>(shadingStore);

            // Debug::log("Directional light created with direction: ", x_int, " ",
            // y_int, " ", z_int, " and intensity: ", _intensity);
        } catch (const libconfig::SettingException& ex) {
            throw std::runtime_error(
                std::string("Error in directional light parameters: ") + ex.what());
        }
    }

    void setDirection(const Math::Vector3D& direction)
    {
        _direction = direction.normalize();
    }
    void setIntensity(float intensity) { _intensity = intensity; }

    Math::Vector3D getDirection() const override { return _direction; }
    float getIntensity() const override { return _intensity; }

    bool isDirectionalLight() const override { return true; }
    bool isAmbientLight() const override { return false; }
    ShadingModel getShadingModel() const override { return _shadingModel; };

    Math::Point3D getOrigin() const override { return Math::Point3D(); }
};

} // namespace Raytracer

#endif /* _DIRECTIONAL_LIGHT_HPP */
