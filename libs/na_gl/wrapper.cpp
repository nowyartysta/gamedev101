module;

#include <glad/glad.h>

export module na_gl:wrapper;

import na_error;

#define CHECK_GL_ERROR(msg)                                                    \
  if (auto error = glGetError(); error != GL_NO_ERROR) {                       \
    return SimpleError(msg " failed, err={}", error);                          \
  }

namespace na {
export class GL {
public:
  static GL &instance() noexcept {
    static GL instance;
    return instance;
  }

  VoidResult clear(GLbitfield mask) noexcept {
    glClear(mask);
    CHECK_GL_ERROR("glClear");
    return {};
  }

  void clearColor(float r, float g, float b, float a) noexcept {
    glClearColor(r, g, b, a);
  }

  VoidResult viewport(int x, int y, int width, int height) noexcept {
    glViewport(x, y, width, height);
    CHECK_GL_ERROR("glViewport");
    return {};
  }

private:
  GL() = default;
};
} // namespace na