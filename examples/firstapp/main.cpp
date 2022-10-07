#include "window.hpp"

int main(int argc, char **argv) {
  // Create application instance
  abcg::Application app(argc, argv);

  // Create OpenGL window
  Window window;
  window.setWindowSettings({.title = "First App"});

  // Run application
  app.run(window);

  return 0;
}