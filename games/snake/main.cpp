#include <glad/glad.h>
#include <iostream>

import na_glfwapp;
import na_error;

class SnakeApp : public na::GlfwApplication {
public:
  na::VoidResult onUpdate(const na::GlfwApplicationState &state) override {
    glViewport(0, 0, state.width, state.height);
    glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return {};
  }
};

int main() {
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