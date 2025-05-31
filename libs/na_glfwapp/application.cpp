module;

#include <string>

export module na_glfwapp:application;
import na_error;

namespace na {

export struct GlfwApplicationConfig {
  std::string title;
  int width;
  int height;
};

export struct GlfwApplicationState {
  int width;
  int height;
};

export class GlfwApplication {
public:
  virtual ~GlfwApplication() = default;
  virtual VoidResult onUpdate(const GlfwApplicationState &) { return {}; }
};
}; // namespace na