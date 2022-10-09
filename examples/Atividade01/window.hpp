#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  GLuint m_vao{};
  GLuint m_vboVertices{};
  GLuint m_vboColors{};
  GLuint m_program{};

  glm::ivec2 m_viewportSize{};

  GLint m_translationLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  float m_rotation{0.0f};
  float m_scale{1.0f};
  glm::vec2 m_translation{glm::vec2(0.0f, 0.0f)};

  glm::vec4 m_clearColor{0.906f, 0.910f, 0.918f, 1.00f};
};

#endif