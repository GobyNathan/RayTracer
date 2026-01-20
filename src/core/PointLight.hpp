/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Tue, May, 2025 * Title           - mirror_raytracer [WSL:
 * Debian]                                     * Description     - * PointLight
 * *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _| *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _| *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_| *
 *       _|        _|          _|        _|      _|        _|        _|    _| *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _| *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#ifndef _POINT_LIGHT_HPP
#define _POINT_LIGHT_HPP

#include "../core/Point3D.hpp"
#include "../core/Vector3D.hpp"
#include "../interfaces/ILight.hpp"
#include "../utils/Debug.hpp"
#include <libconfig.h++>
#include <stdexcept>

namespace Raytracer {

class PointLight : public ILight {
private:
    Math::Point3D _origin;
    float _intensity;
    float _attenuation; // Light attenuation (falloff with distance)
    ShadingModel _shadingModel;

public:
    PointLight(const Math::Point3D& origin, float intensity,
        float attenuation = 0.01)
        : _origin(origin)
        , _intensity(intensity)
        , _attenuation(attenuation)
    {
    }

    PointLight(const libconfig::Setting& settings)
    {
        try {
            double x, y, z;
            float intensity = 1.0;
            float attenuation = 0.01;

            settings.lookupValue("x", x);
            settings.lookupValue("y", y);
            settings.lookupValue("z", z);
            settings.lookupValue("intensity", intensity);
            settings.lookupValue("attenuation", attenuation);

            int shadingStore;
            settings.lookupValue("shading_model", shadingStore);
            _shadingModel = static_cast<ShadingModel>(shadingStore);

            _origin = Math::Point3D(x, y, z);
            _intensity = intensity;
            _attenuation = attenuation;

            Debug::log("Point light created at position: (", x, ", ", y, ", ", z,
                ")");
            Debug::log("Intensity: ", _intensity, ", Attenuation: ", _attenuation);
        } catch (const libconfig::SettingException& ex) {
            throw std::runtime_error(
                std::string("Error in point light parameters: ") + ex.what());
        }
    }

    Math::Point3D getOrigin() const override { return _origin; }
    float getIntensity() const override { return _intensity; }
    bool isDirectionalLight() const override { return false; }
    bool isAmbientLight() const override { return false; }
    ShadingModel getShadingModel() const override { return _shadingModel; };

    float getIntensityAt(double distance) const override
    {
        // Simple attenuation model: intensity / (1 + attenuation * distance²)
        return _intensity / (1.0 + _attenuation * distance * distance);
    }

    // For point lights, the direction depends on the point being illuminated
    Math::Vector3D getDirection() const override
    {
        // Default direction, will be overridden in the renderer
        return Math::Vector3D(0, 0, 1);
    }

    // Get the actual direction from light to a given point
    Math::Vector3D getDirectionTo(const Math::Point3D& point) const
    {
        return (point - _origin).normalize();
    }
};

} // namespace Raytracer

#endif /* _POINT_LIGHT_HPP */

/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * MIT License * Copyright (c) 2025 Anonymous *
 *                                                                                      *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * * of this software and associated documentation files (the "Software"), to
 * deal        * in the Software without restriction, including without
 * limitation the rights         * to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell            * copies of the Software, and
 * to permit persons to whom the Software is                * furnished to do
 * so, subject to the following conditions:                             *
 *                                                                                      *
 * The above copyright notice and this permission notice shall be included in
 * all       * copies or substantial portions of the Software. *
 *                                                                                      *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE        * SOFTWARE. *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */
