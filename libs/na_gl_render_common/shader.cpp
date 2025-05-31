module;

#include <glad/glad.h>
#include <na_error/macros.hpp>
#include <string>
#include <string_view>

export module na_gl_render_common:shader;

import na_error;
import na_gl;

namespace na::gl {

export class ShaderStage {
  GLuint _id;
  std::string _name;
  ShaderStage(GLuint id, std::string name) : _id(id), _name(std::move(name)) {}

public:
  ShaderStage(const ShaderStage &) = delete;
  ShaderStage &operator=(const ShaderStage &) = delete;
  ShaderStage &operator=(ShaderStage &&) noexcept = delete;

  ShaderStage(ShaderStage &&other) noexcept
      : _id(other._id), _name(std::move(other._name)) {
    other._id = 0;
  }

  ~ShaderStage() noexcept {
    if (_id != 0) {
      GL::instance().deleteShader(_id);
    }
  }

  std::string_view name() const noexcept { return {_name}; }

  static Result<ShaderStage> create(GLenum type, std::string name,
                                    std::string_view source) noexcept {
    AUTO_RESULT(shaderId, GL::instance().createShader(type));
    GLint sourceLens[] = {static_cast<GLint>(source.size())};
    const GLchar *sources[] = {source.data()};
    CHECK_RESULT(GL::instance().shaderSource(shaderId, 1, sources, sourceLens));
    CHECK_RESULT(GL::instance().compileShader(shaderId));
    return ShaderStage(shaderId, std::move(name));
  }

  VoidResult attachToProgram(GLuint programId) const noexcept {
    return GL::instance().attachShader(programId, _id);
  }
};

export class ShaderProgram {
  GLuint _id{};
  std::string _name{};
  ShaderProgram(GLuint id, std::string name) noexcept
      : _id(id), _name(std::move(name)) {}

public:
  ShaderProgram(const ShaderProgram &) = delete;
  ShaderProgram &operator=(const ShaderProgram &) = delete;
  ShaderProgram &operator=(ShaderProgram &&) noexcept = delete;

  ShaderProgram(ShaderProgram &&other) noexcept
      : _id(other._id), _name(std::move(other._name)) {
    other._id = 0;
  }

  ~ShaderProgram() noexcept {
    if (_id != 0) {
      GL::instance().deleteProgram(_id);
    }
  }

  std::string_view name() const noexcept { return {_name}; }

  template <typename TIt>
  static Result<ShaderProgram> create(TIt begin, TIt end,
                                      std::string name) noexcept {
    AUTO_RESULT(programId, GL::instance().createProgram());
    for (auto it = begin; it != end; ++it) {
      CHECK_RESULT((*it)->attachToProgram(programId));
    }
    CHECK_RESULT(GL::instance().linkProgram(programId));
    return ShaderProgram(programId, std::move(name));
  }

  template <typename T>
  static Result<ShaderProgram> create(std::initializer_list<T *> stages,
                                      std::string name) noexcept {
    return create(stages.begin(), stages.end(), std::move(name));
  }
};

} // namespace na::gl