/*
** EPITECH PROJECT, 2025
** mirror_raytracer
** File description:
** DisplayManager.cpp
*/

#include "DisplayManager.hpp"
#include "../builders/SceneBuilder.hpp"
#include "../builders/SceneLoader.hpp"
#include "../renderer/Renderer.hpp"
#include "EventManager/EventManager.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <filesystem>
#include <fstream>
#include <future>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace Interface {

DisplayManager::DisplayManager()
    : _win(sf::VideoMode(1920, 1080), "GobiTracer17") {
  _selectedParams = {
      {"refraction", "0"}, {"superSampling", "0"}, {"sceneFile", "NONE"}};
}

std::shared_ptr<sf::RectangleShape>
DisplayManager::getElement(const std::string &name) {
  auto it = _elements.find(name);
  if (it != _elements.end()) {
    return it->second;
  }
  return nullptr;
}

sf::Vector2f getRelativePosition(sf::Vector2f relative, float xPercent,
                                 float yPercent) {
  return sf::Vector2f((xPercent / 100.0f) * relative.x,
                      (yPercent / 100.0f) * relative.y);
}

std::shared_ptr<sf::RectangleShape>
DisplayManager::setupRectangle(Size sz, Coords xy, Rgba rgba) {
  auto rectangle = std::make_shared<sf::RectangleShape>();

  sf::Vector2f size = getRelativePosition(
      sz.relative ? sz.relative->getSize()
                  : static_cast<sf::Vector2f>(_win.getSize()),
      sz.x, sz.y);
  sf::Vector2f position = getRelativePosition(
      xy.relative ? xy.relative->getSize()
                  : static_cast<sf::Vector2f>(_win.getSize()),
      xy.x, xy.y);

  rectangle->setSize(size);
  rectangle->setPosition(position);
  rectangle->setFillColor(sf::Color(rgba.r, rgba.g, rgba.b, rgba.a));

  return rectangle;
}

sf::Text
DisplayManager::setupText(const std::string &content,
                          const std::shared_ptr<sf::RectangleShape> &relativeTo,
                          float xPercent, float yPercent) {
  sf::Text text;
  text.setFont(_font);
  text.setString(content);
  text.setCharacterSize(13);
  text.setFillColor(sf::Color::White);

  sf::Vector2f relativePos =
      getRelativePosition(relativeTo->getSize(), xPercent, yPercent);
  sf::Vector2f absolutePos = relativeTo->getPosition() + relativePos;

  text.setPosition(absolutePos);

  return text;
}

void DisplayManager::init() {
  _elements = {
      // std::shared_ptr<sf::RectangleShape> setupRectangle(Size sz, Coords xy,
      //                                                    Rgba rgba);
      {"selection",
       setupRectangle({nullptr, 20, 50}, {nullptr, 0, 0}, {0, 0, 0, 100})},
      {"params",
       setupRectangle({nullptr, 30, 5}, {nullptr, 70, 0}, {0, 0, 0, 100})},

      {"screen", setupRectangle({nullptr, 100, 100}, {nullptr, 0, 0},
                                {200, 200, 200, 255})},
  };

  if (!_font.loadFromFile("assets/fonts/Blazed.ttf")) {
    throw std::runtime_error("Failed to load font");
  }

  std::shared_ptr<sf::RectangleShape> menu = getElement("selection");

  _refractionText = setupText("r: " + _selectedParams["refraction"],
                              getElement("params"), 40.0f, 10.0f);

  _superSamplingText = setupText("s: " + _selectedParams["superSampling"],
                                 getElement("params"), 50.0f, 10.0f);

  float yOffset = 2.f;

  for (const auto &entry : std::filesystem::directory_iterator("scenes")) {
    if (entry.is_regular_file()) {
      _sceneLabels.push_back(setupText(entry.path().filename().string(),
                                       getElement("selection"), 0.0f, yOffset));
      yOffset += 5.f;
    }
  }
}

void DisplayManager::display() {
  EventManager eventManager(*this);

  while (_win.isOpen()) {
    eventManager.handleEvents();
    update();
    render();
  }
}

void DisplayManager::renderScene(const std::string &sceneFile) {
  if (_isRendering) {
    if (_renderFuture.valid()) {
      _renderFuture.wait();
    }
  }

  _isRendering = true;

  int superSampling = std::stoi(_selectedParams["superSampling"]);
  int refraction = std::stoi(_selectedParams["refraction"]);

  _renderFuture = std::async(
      std::launch::async, [this, sceneFile, superSampling, refraction]() {
        auto screenElement = getElement("screen");

        int width = static_cast<int>(screenElement->getSize().x);
        int height = static_cast<int>(screenElement->getSize().y);

        Raytracer::SceneBuilder builder;
        Raytracer::SceneLoader loader(builder);
        loader.loadSceneFromFile("scenes/" + sceneFile);

        Raytracer::Renderer renderer(std::move(builder), width, height,
                                     refraction + 1, superSampling);
        renderer.render();
      });
}

sf::Image loadPPM(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file)
    throw std::runtime_error("Failed to open PPM file: " + filePath);

  std::string format;
  file >> format;
  if (format != "P3")
    throw std::runtime_error("Invalid PPM format. Expected P3, got " + format);

  int width, height, maxVal;
  file >> width >> height >> maxVal;

  if (width <= 0 || height <= 0)
    throw std::runtime_error("Invalid image dimensions");

  std::vector<sf::Uint8> pixels(width * height * 4);

  for (int i = 0; i < width * height; ++i) {
    int r, g, b;

    file >> r >> g >> b;
    pixels[i * 4 + 0] = static_cast<sf::Uint8>(r);
    pixels[i * 4 + 1] = static_cast<sf::Uint8>(g);
    pixels[i * 4 + 2] = static_cast<sf::Uint8>(b);
    pixels[i * 4 + 3] = 255;
  }

  sf::Image image;
  image.create(width, height, pixels.data());

  return image;
}

void DisplayManager::loadRenderToScreen() {
  if (!_win.isOpen())
    return;

  if (!std::filesystem::exists("output.ppm"))
    throw std::runtime_error("Output file not found");

  sf::Image ppmImage = loadPPM("output.ppm");

  auto screenElement = getElement("screen");
  if (!screenElement)
    throw std::runtime_error("Screen element not found");

  if (!_renderedTexture.loadFromImage(ppmImage))
    throw std::runtime_error("Failed to create texture from image.");

  screenElement->setTexture(&_renderedTexture, true);
}
void DisplayManager::update() {
  _superSamplingText.setString("s: " + _selectedParams["superSampling"]);
  _refractionText.setString("r: " + _selectedParams["refraction"]);

  if (_isRendering && _renderFuture.valid() &&
      _renderFuture.wait_for(std::chrono::seconds(0)) ==
          std::future_status::ready) {
    loadRenderToScreen();
    _isRendering = false;
  }
}

void DisplayManager::render() {
  _win.clear(sf::Color::Black);
  draw();
  _win.display();
}

void DisplayManager::draw() {
  for (auto &elem : _elements) {
    _win.draw(*elem.second);
  }

  for (auto &label : _sceneLabels) {
    _win.draw(label);
  }

  _win.draw(_refractionText);
  _win.draw(_superSamplingText);
}

sf::RenderWindow &DisplayManager::getWindow() { return _win; }

std::vector<sf::Text> &DisplayManager::getSceneLabels() { return _sceneLabels; }

void DisplayManager::highlightScene(int index) {
  if (_highlightedSceneIndex >= 0 &&
      _highlightedSceneIndex < static_cast<int>(_sceneLabels.size())) {
    _sceneLabels[_highlightedSceneIndex].setFillColor(sf::Color::White);
  }

  if (index >= 0 && index < static_cast<int>(_sceneLabels.size())) {
    _sceneLabels[index].setFillColor(sf::Color::Yellow);
    _highlightedSceneIndex = index;
  }
}

void DisplayManager::setSelectedParam(const std::string &key,
                                      const std::string &value) {
  _selectedParams[key] = value;
}

std::unordered_map<std::string, std::string> &
DisplayManager::getSelectedParams() {
  return _selectedParams;
}

} // namespace Interface
