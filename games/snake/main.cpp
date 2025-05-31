#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <na_error/macros.hpp>
#include <string>

import na_error;
import na_glfwapp;
import na_gl;
import na_gl_render_common;

const std::string SPRITE_VERTEX_SHADER_SRC = R"(
#version 450 core

struct Instance {
  mat4 local;
  vec4 tint;
};

layout (binding = 0) uniform Shared {
  mat4 view;
  mat4 projection;
};

layout (binding = 0, std430) buffer Instances {
  Instance instances[];
};

vec2 vertices[4] = vec2[](
  vec2(-0.5, -0.5),
  vec2(0.5, -0.5),
  vec2(0.5, 0.5),
  vec2(-0.5, 0.5)
);

layout (location = 0) out vec4 outTint;

void main() {
  Instance instance = instances[gl_InstanceID];
  vec4 position = instance.local * vec4(vertices[gl_VertexID], 0.0, 1.0);
  outTint = instance.tint;
  gl_Position = projection * view * position;
}

)";

const std::string SPRITE_FRAGMENT_SHADER_SRC = R"(
#version 450 core

layout (location = 0) in vec4 inTint;
layout (location = 0) out vec4 outColor;

void main() {
  outColor = inTint;
}

)";

class SnakeApp : public na::GlfwApplication {
  std::unique_ptr<na::gl::ShaderProgram> _spriteProgram;

public:
  na::VoidResult onInit(const na::GlfwApplicationState &) noexcept override {
    UNIQUE_RESULT(spriteVertexShader,
                  na::gl::ShaderStage::create(GL_VERTEX_SHADER, "sprite_vs",
                                              SPRITE_VERTEX_SHADER_SRC));
    UNIQUE_RESULT(spriteFragmentShader,
                  na::gl::ShaderStage::create(GL_FRAGMENT_SHADER, "sprite_fs",
                                              SPRITE_FRAGMENT_SHADER_SRC));
    UNIQUE_RESULT(spriteProgram,
                  na::gl::ShaderProgram::create(
                      {&spriteVertexShader, &spriteFragmentShader}, "sprite"));
    _spriteProgram =
        std::make_unique<na::gl::ShaderProgram>(std::move(spriteProgram));
    return {};
  }

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