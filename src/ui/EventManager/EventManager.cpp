#include "EventManager.hpp"
#include "../DisplayManager.hpp"

namespace Interface {

EventManager::EventManager(DisplayManager &dm) : _dm(dm), _win(dm.getWindow()) {
  _dm.highlightScene(_selectedIndex);
}

void EventManager::handleEvents() {
  sf::Event event;

  while (_win.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      _win.close();

    if (event.type == sf::Event::KeyPressed) {
      switch (event.key.code) {
      case sf::Keyboard::Up:
        navigate(-1);
        break;
      case sf::Keyboard::Down:
        navigate(1);
        break;
      case sf::Keyboard::Enter:
        select();
        break;
      case sf::Keyboard::Left:
        incrementParam("refraction");
        break;
      case sf::Keyboard::Right:
        incrementParam("superSampling");
        break;
      default:
        break;
      }
    }
  }
}

void EventManager::navigate(int direction) {
  int count = _dm.getSceneLabels().size();
  _selectedIndex = (_selectedIndex + direction + count) % count;
  _dm.highlightScene(_selectedIndex);
}

void EventManager::select() {
  const auto &label = _dm.getSceneLabels()[_selectedIndex];
  std::string sceneName = label.getString().toAnsiString();

  _dm.renderScene(sceneName);
}

void EventManager::incrementParam(const std::string &param) {
  std::string valueStr = _dm.getSelectedParams().at(param);
  int value = std::stoi(valueStr);
  if (value < 10) {
    value++;
    _dm.setSelectedParam(param, std::to_string(value));
  } else {
    value = 0;
    _dm.setSelectedParam(param, std::to_string(value));
  }
}

} // namespace Interface
