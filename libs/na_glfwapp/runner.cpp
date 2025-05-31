module;

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <na_error/macros.hpp>

export module na_glfwapp:runner;
import na_error;
import :application;

namespace na {
export VoidResult runGlfwApplication(const GlfwApplicationConfig &config,
                                     GlfwApplication &app) {
  if (!glfwInit()) {
    return SimpleError("Failed to initialize GLFW");
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

  auto window = glfwCreateWindow(config.width, config.height,
                                 config.title.c_str(), nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return SimpleError("Failed to create GLFW window");
  }
  glfwMakeContextCurrent(window);
  gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

  while (!glfwWindowShouldClose(window)) {
    GlfwApplicationState state{
        .width = config.width,
        .height = config.height,
    };
    CHECK_RESULT(app.onUpdate(state));
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return {};
}
}; // namespace na