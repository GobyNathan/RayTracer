/*
** EPITECH PROJECT, 2025
** mirror_raytracer
** File description:
** DisplayManager.hpp
*/
#ifndef DISPLAY_MANAGER_HPP
#define DISPLAY_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <future>
#include <memory>
#include <string>
#include <unordered_map>

namespace Interface {

class DisplayManager {

  // fyi size is written as percentage of the space that the window occupies

  struct Size {
    std::shared_ptr<sf::RectangleShape> relative;
    float x;
    float y;
  };

  struct Coords {
    std::shared_ptr<sf::RectangleShape> relative;
    float x;
    float y;
  };

  struct Rgba {
    int r;
    int g;
    int b;
    int a;
  };

private:
  sf::RenderWindow _win;

  std::unordered_map<std::string, std::shared_ptr<sf::RectangleShape>>
      _elements;

  sf::Font _font;
  std::vector<sf::Text> _sceneLabels;
  int _highlightedSceneIndex = -1;
  sf::Text _refractionText;
  sf::Text _superSamplingText;

  std::unordered_map<std::string, std::string> _selectedParams;

  void render();
  void update();
  void draw();

  std::shared_ptr<sf::RectangleShape> setupRectangle(Size sz, Coords xy,
                                                     Rgba rgba);
  sf::Text setupText(const std::string &content,
                     const std::shared_ptr<sf::RectangleShape> &relativeTo,
                     float xPercent, float yPercent);

  sf::Texture _renderedTexture;
  std::future<void> _renderFuture;
  bool _isRendering = false;

public:
  DisplayManager();

  void init();
  void display();

  sf::RenderWindow &getWindow();
  std::vector<sf::Text> &getSceneLabels();
  std::unordered_map<std::string, std::string> &getSelectedParams();
  std::shared_ptr<sf::RectangleShape> getElement(const std::string &name);

  void highlightScene(int index);
  void setSelectedParam(const std::string &key, const std::string &value);
  void renderScene(const std::string &sceneFile);
  void loadRenderToScreen();
};

} // namespace Interface

#endif /* DISPLAY_ENGINE_HPP */
