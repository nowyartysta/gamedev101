module;

#include <glad/glad.h>
#include <string>
#include <vector>

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

  VoidResult attachShader(GLuint program, GLuint shader) noexcept {
    glAttachShader(program, shader);
    CHECK_GL_ERROR("glAttachShader");
    return {};
  }

  VoidResult clear(GLbitfield mask) noexcept {
    glClear(mask);
    CHECK_GL_ERROR("glClear");
    return {};
  }

  Result<GLuint> createShader(GLenum type) noexcept {
    auto id = glCreateShader(type);
    CHECK_GL_ERROR("glCreateShader");
    if (id == 0) {
      return SimpleError("glCreateShader failed, id=0");
    }
    return id;
  }

  Result<GLuint> createProgram() noexcept {
    auto id = glCreateProgram();
    if (id == 0) {
      return SimpleError("glCreateProgram failed, id=0");
    }
    return id;
  }

  void clearColor(float r, float g, float b, float a) noexcept {
    glClearColor(r, g, b, a);
  }

  VoidResult compileShader(GLuint shader) noexcept {
    glCompileShader(shader);
    CHECK_GL_ERROR("glCompileShader");
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
      GLint infoLogLen;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
      std::vector<GLchar> infoLog(infoLogLen);
      glGetShaderInfoLog(shader, infoLogLen, nullptr, infoLog.data());
      infoLog.push_back('\0');
      return SimpleError("glCompileShader, err={}",
                         std::string(infoLog.data()));
    }
    return {};
  }

  VoidResult deleteProgram(GLuint program) noexcept {
    glDeleteProgram(program);
    CHECK_GL_ERROR("glDeleteProgram");
    return {};
  }

  VoidResult deleteShader(GLuint shader) noexcept {
    glDeleteShader(shader);
    CHECK_GL_ERROR("glDeleteShader");
    return {};
  }

  VoidResult linkProgram(GLuint program) noexcept {
    glLinkProgram(program);
    CHECK_GL_ERROR("glLinkProgram");
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
      GLint infoLogLen;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
      std::vector<GLchar> infoLog(infoLogLen);
      glGetProgramInfoLog(program, infoLogLen, nullptr, infoLog.data());
      infoLog.push_back('\0');
      return SimpleError("glLinkProgram, err={}", std::string(infoLog.data()));
    }
    return {};
  }

  VoidResult shaderSource(GLuint shader, GLsizei count,
                          const GLchar *const *string,
                          const GLint *length) noexcept {
    glShaderSource(shader, count, string, length);
    CHECK_GL_ERROR("glShaderSource");
    return {};
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