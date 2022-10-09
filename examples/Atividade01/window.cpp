#include "window.hpp"
#include "abcgOpenGLShader.hpp"

void Window::onCreate() {
  // Enable Z-buffer test
  abcg::glEnable(GL_DEPTH_TEST);

  // Create shader program
  auto const path{abcg::Application::getAssetsPath()};
  m_program =
      abcg::createOpenGLProgram({{.source = path + "UnlitVertexColor.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = path + "UnlitVertexColor.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  // clang-format off
  std::array const vertices{glm::vec2(0.0f, 0.5f),
                            glm::vec2(0.5f, -0.5f),
                            glm::vec2(-0.5f, -0.5f)};

  // clang-format on
  // Adding the rotation, scale e translation configurations
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Generate a new VBO and get the associated ID
  abcg::glGenBuffers(1, &m_vboVertices);
  // Bind VBO in order to use it
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
  // Upload data to VBO
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                     GL_STATIC_DRAW);
  // Unbinding the VBO is allowed (data can be released now)
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Window::onPaint() {
  // Set the clear color
  abcg::glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b,
                     m_clearColor.a);
  // Clear the color buffer and Z-buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Adjust viewport
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  // Start using the shader program
  abcg::glUseProgram(m_program);
  // Start using the VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform1f(m_rotationLoc, m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
  // Render a triangle
  abcg::glDrawArrays(GL_TRIANGLES, 0, 3);

  // End using the VAO
  abcg::glBindVertexArray(0);
  // End using the shader program
  abcg::glUseProgram(0);
}

void Window::onPaintUI() {
  // Parent class will show fullscreen button and FPS meter
  abcg::OpenGLWindow::onPaintUI();

  // Our own ImGui widgets go below
  {
    // If this is the first frame, set initial position of our window
    static auto firstTime{true};
    if (firstTime) {
      ImGui::SetNextWindowPos(ImVec2(5, 75));
      firstTime = false;
    }

    // Window begin
    ImGui::Begin("Movimente o triângulo através das barras:");

    // Static text
    ImGui::Text(
        "Você pode alterar o tamanho, rotação e translação do triângulo.");
    ImGui::Text(
        "A cor de background pode ser alterada conforme sua preferência");

    // Sliders for manipulation of the new properties
    ImGui::SliderFloat("Tamanho", &m_scale, 0.0f, 4.0f);
    ImGui::SliderFloat("Rotação", &m_rotation, 0.0f, 2 * 3.1415f);
    ImGui::SliderFloat("Translação eixo X", &m_translation.x, 0.0f, 1.0f);
    ImGui::SliderFloat("Translação eixo Y", &m_translation.y, 0.0f, 1.0f);

    // Edit background color
    ImGui::ColorEdit3("Background", &m_clearColor.r);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // End of window
    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) { m_viewportSize = size; }

void Window::onDestroy() {
  // Release OpenGL resources
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_vboVertices);
  abcg::glDeleteVertexArrays(1, &m_vao);
}