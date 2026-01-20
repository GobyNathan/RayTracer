#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace Interface {

class DisplayManager;

class EventManager {
public:
  EventManager(DisplayManager &dm);
  void handleEvents();

private:
  DisplayManager &_dm;
  sf::RenderWindow &_win;

  int _selectedIndex = 0;

  std::string _inputBuffer;
  bool _isEnteringValue = false;

  void navigate(int direction);
  void select();
  void incrementParam(const std::string &param);
};

} // namespace Interface
