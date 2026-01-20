/*
** EPITECH PROJECT, 2025
** mirror_raytracer
** File description:
** Renderer.cpp
*/
#include "Renderer.hpp"
#include "../core/Camera.hpp"
#include "../core/Ray.hpp"
#include "../interfaces/IPrimitive.hpp"
#include "../utils/Debug.hpp"
#include "LightRenderer/LightRenderer.hpp"
#include "PrimitiveRenderer/PrimitiveRenderer.hpp"
#include <algorithm>
#include <atomic>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <ostream>
#include <sstream>
#include <thread>

namespace Raytracer {

Renderer::Renderer(SceneBuilder scene, int width, int height, int maxDepth,
                   int samples)
    : _scene(std::move(scene)), _width(width), _height(height),
      _maxDepth(maxDepth), _samples(samples), _backgroundColor(0, 0, 1),
      renderResult(_width * _height) {

  // Initialize specialized renderers
  _lightRenderer = std::make_unique<LightRenderer>(
      _scene.getLights(), _scene.getPrimitives(),
      _scene.getCamera().getPosition());

  _primitiveRenderer =
      std::make_unique<PrimitiveRenderer>(_scene.getPrimitives());

  Debug::log("Renderer initialized with background color: ", _backgroundColor.x,
             ", ", _backgroundColor.y, ", ", _backgroundColor.z);
}

void Renderer::render() {
  renderTimer.start();
  Debug::log("Starting render of ", _width, "x", _height, " image with ",
             _samples, " samples per pixel");

  int totalPixels = _width * _height;
  std::atomic<int> raysCast(0);
  std::atomic<int> pixelsCompleted(0);
  std::vector<std::thread> threads;
  int numThreads = std::thread::hardware_concurrency();
  std::mutex resultMutex;

  renderResult.resize(totalPixels);

  int rowsPerThread = (_height + numThreads - 1) / numThreads;

  for (int i = 0; i < numThreads; i++) {
    threads.emplace_back([&, i]() {
      int startRow = i * rowsPerThread;
      int endRow = std::min(startRow + rowsPerThread, _height);

      for (int y = startRow; y < endRow; y++) {
        for (int x = 0; x < _width; x++) {
          Math::Vector3D pixelColor(0, 0, 0);

          if (_samples > 1) {
            for (int s = 0; s < _samples; s++) {
              for (int t = 0; t < _samples; t++) {
                double u = (x + (s + 0.5) / _samples) / (_width - 1);
                double v = (y + (t + 0.5) / _samples) / (_height - 1);
                Ray ray = _scene.getCamera().ray(u, v);
                pixelColor += traceRay(ray, 0);
                raysCast++;
              }
            }
            pixelColor = pixelColor / (_samples * _samples);
          } else {
            double u = (double)x / (_width - 1);
            double v = (double)y / (_height - 1);
            Ray ray = _scene.getCamera().ray(u, v);
            pixelColor = traceRay(ray, 0);
            raysCast++;
          }

          renderResult[y * _width + x] = drawPixel(pixelColor);
          pixelsCompleted++;
        }
      }
    });
  }

  while (pixelsCompleted < totalPixels) {
    int percent = (100 * pixelsCompleted) / totalPixels;

    std::cerr << "\rProgress: " << percent << "%" << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  for (auto &thread : threads) {
    thread.join();
  }

  double renderTime = renderTimer.elapsedSeconds();
  double pixelsPerSecond = totalPixels / renderTime;
  double raysPerSecond = raysCast / renderTime;

  Debug::log("Render completed in ", renderTimer.elapsedString());
  std::cerr << "\n===== Render Statistics =====" << std::endl;
  std::cerr << "Resolution: " << _width << "x" << _height << " (" << totalPixels
            << " pixels)" << std::endl;
  std::cerr << "Samples per pixel: " << _samples << " ("
            << (_samples * _samples) << " rays per pixel)" << std::endl;
  std::cerr << "Maximum ray depth: " << _maxDepth << std::endl;
  std::cerr << "Total rays cast: " << raysCast << std::endl;
  std::cerr << "Total render time: " << renderTimer.elapsedString()
            << std::endl;
  std::cerr << "Performance: " << std::fixed << std::setprecision(2)
            << pixelsPerSecond << " pixels/sec" << std::endl;
  std::cerr << "Ray throughput: " << std::fixed << std::setprecision(0)
            << raysPerSecond << " rays/sec" << std::endl;
  std::cerr << "Average: " << std::fixed << std::setprecision(3)
            << (renderTime / totalPixels * 1000) << " ms per pixel"
            << std::endl;
  std::cerr << "============================" << std::endl;

  std::ofstream file("output.ppm");
  if (!file)
    throw std::runtime_error("Failed to open output.ppm for writing");

  file << "P3\n" << _width << " " << _height << "\n255\n";
  for (const auto &pixel : renderResult)

    file << pixel;
  file.close();
}

Math::Vector3D Renderer::traceRay(Ray &ray, int depth) {
  if (depth >= _maxDepth) {
    return _backgroundColor;
  }

  IntersectionInfo intersection;
  IPrimitive *hitPrim =
      _primitiveRenderer->findClosestIntersection(ray, intersection);

  if (hitPrim) {
    auto material = hitPrim->getMaterial();
    Math::Vector3D materialColor = material->getColor();

    Debug::log("Material type: ", typeid(*material).name(), " Color: (",
               material->getColor().x, ", ", material->getColor().y, ", ",
               material->getColor().z, ")");

    Math::Vector3D lightCoefficient =
        _lightRenderer->computeLight(intersection.hitPoint, hitPrim);

    auto traceFunc = [this](const Ray &r, int d) -> Math::Vector3D {
      Debug::log("Tracing recursive ray at depth ", d);
      return this->traceRay(const_cast<Ray &>(r), d);
    };

    Math::Vector3D finalColor =
        material->computeInteraction(ray, intersection, traceFunc, depth);

    Debug::log("Final color: (", materialColor.x * lightCoefficient.x, ", ",
               materialColor.y * lightCoefficient.y, ", ",
               materialColor.z * lightCoefficient.z, ")");
    return Math::Vector3D(finalColor.x * lightCoefficient.x,
                          finalColor.y * lightCoefficient.y,
                          finalColor.z * lightCoefficient.z);
  } else {
    return _backgroundColor;
  }
}

std::string Renderer::drawPixel(const Math::Vector3D &color) {
  int r = static_cast<int>(255 * std::min(1.0, std::max(0.0, color.x)));
  int g = static_cast<int>(255 * std::min(1.0, std::max(0.0, color.y)));
  int b = static_cast<int>(255 * std::min(1.0, std::max(0.0, color.z)));
  std::ostringstream oss;

  oss << r << " " << g << " " << b << "\n";
  return oss.str();
}

} // namespace Raytracer
