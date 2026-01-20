#include "builders/SceneBuilder.hpp"
#include "builders/SceneLoader.hpp"
#include "renderer/Renderer.hpp"
#include "ui/DisplayManager.hpp"
#include "utils/Debug.hpp"
#include <iostream>
#include <stdexcept>

static void printUsage(const char *progName) {
  std::cerr << "USAGE: " << progName << " [OPTIONS] <scene_file>" << std::endl;
  std::cerr << "  -h    Display this help message" << std::endl;
  std::cerr << "  -d    Enable debug mode" << std::endl;
  std::cerr << "  -s    Set samples per pixel (default: 1)" << std::endl;
  std::cerr << "  -r    Set maximum ray depth (default: 5)" << std::endl;
}

static Raytracer::SceneBuilder loadScene(const std::string &filename) {
  try {
    Raytracer::SceneBuilder builder;
    Raytracer::SceneLoader loader(builder);
    auto scene = loader.loadSceneFromFile(filename);

    return builder;
  } catch (const std::exception &e) {
    throw std::runtime_error(e.what());
  }
}
void launchUserInterface() {
  Interface::DisplayManager dispManager;
  dispManager.init();
  dispManager.display();
}

void launchDefault(int argc, char *argv[]) {
  if (argc < 2 || std::string(argv[1]) == "-h") {
    printUsage(argv[0]);
    return;
  }

  std::string filename = argv[argc - 1];
  bool debugMode = false;
  int samples = 1;
  int maxDepth = 5;

  for (int i = 1; i < argc - 1; i++) {
    std::string arg = argv[i];
    if (arg == "-d") {
      debugMode = true;
    } else if (arg == "-s" && i + 1 < argc - 1) {
      samples = std::stoi(argv[++i]);
    } else if (arg == "-r" && i + 1 < argc - 1) {
      maxDepth = std::stoi(argv[++i]);
    }
  }

  Raytracer::Debug::setEnabled(debugMode);

  Raytracer::Renderer renderer(loadScene(filename), 1920, 1080, maxDepth,
                               samples);
  renderer.render();
}

int main(int argc, char *argv[]) {
  bool debugMode = false;

  Raytracer::Debug::setEnabled(debugMode);

  if (argc == 1) {
    printUsage(argv[0]);
    return 84;
  }

  if (argv[1] == std::string("-i")) {
    try {
      launchUserInterface();
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 84;
    }
  } else {
    try {
      launchDefault(argc, argv);
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 84;
    }
  }
}
