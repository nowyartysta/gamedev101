#include <glad/glad.h>
#include <iostream>
#include <na_error/macros.hpp>

import na_error;
import na_glfwapp;
import na_gl;

class SnakeApp : public na::GlfwApplication {
public:
  na::VoidResult
  onUpdate(const na::GlfwApplicationState &state) noexcept override {
    CHECK_RESULT(na::GL::instance().viewport(0, 0, state.width, state.height));
    na::GL::instance().clearColor(0.0f, 0.0f, 0.5f, 1.0f);
    CHECK_RESULT(
        na::GL::instance().clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    return {};
  }
};

int main() noexcept {
  SnakeApp snakeApp{};
  na::GlfwApplicationConfig config{
      .title = "Gamedev 101: Snake",
      .width = 1024,
      .height = 1024,
  };
  auto result = na::runGlfwApplication(config, snakeApp);
  if (!result.ok()) {
    std::cerr << "Error: " << result.error().message() << std::endl;
  }
  return 0;
}