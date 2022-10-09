#include "window.hpp"

int main(int argc, char **argv) {
  try {
    // Create application instance
    abcg::Application app(argc, argv);

    // Create OpenGL window
    Window window;
    window.setOpenGLSettings({.profile = abcg::OpenGLProfile::Core,
                              .majorVersion = 4,
                              .minorVersion = 1});
    window.setWindowSettings(
        {.width = 600, .height = 600, .title = "Atividade01"});

    // Run application
    app.run(window);
  } catch (std::exception const &exception) {
    fmt::print(stderr, "{}", exception.what());
    return -1;
  }
  return 0;
}