#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Peridot/RenderCalls.h"
#include "Peridot/Shader.h"
#include "Peridot/Utils.h"

namespace Peridot {

namespace Utils {

static GLenum GLShaderType(const ShaderType shaderType) {
  switch (shaderType) {
    case ShaderType::VertexShader:
      return GL_VERTEX_SHADER;
    case ShaderType::FragmentShader:
      return GL_FRAGMENT_SHADER;
    default:
      throw std::runtime_error("Invalid shader");
  }
  return GL_FALSE;
}

static bool CompileShader(const uint32_t shader) {
  glCompileShader(shader);
  int32_t isCompiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

  if (isCompiled == GL_FALSE) {
    int32_t maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    std::string infoLog;
    infoLog.resize(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());
    spdlog::error(infoLog);
    return false;
  }

  return true;
}

static bool LinkProgram(const uint32_t program) {
  glLinkProgram(program);
  int32_t isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

  if (isLinked == GL_FALSE) {
    int32_t maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
    std::string infoLog;
    infoLog.resize(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
    spdlog::error(infoLog);
    return false;
  }
  return true;
}

}  // namespace Utils

std::shared_ptr<Shader> Shader::Create(
    const std::vector<ShaderSpecification>& shaderSpecs) {
  spdlog::trace(__FUNCTION__);
  auto shaderObj = std::make_shared<Shader>();
  for (const auto& shaderSpec : shaderSpecs) {
    uint32_t shader =
        glCreateShader(Utils::GLShaderType(shaderSpec.shaderType));
    std::string shaderSource =
        Utils::ReadFileIntoStringBuffer(shaderSpec.filePath);
    auto shaderSourcePtr = shaderSource.c_str();
    glShaderSource(shader, 1, &shaderSourcePtr, nullptr);

    if (!Utils::CompileShader(shader)) {
      shaderObj->ShaderCleanup();
      return nullptr;
    }
    shaderObj->mShaderHandles.emplace(shaderSpec.shaderType, shader);
  }

  shaderObj->mProgramId = glCreateProgram();

  for (auto&& [_, shaderId] : shaderObj->mShaderHandles) {
    glAttachShader(shaderObj->mProgramId, shaderId);
  }

  if (!Utils::LinkProgram(shaderObj->mProgramId)) {
    shaderObj->ShaderCleanup();
    shaderObj->ProgramCleanup();
    return nullptr;
  }

  for (auto&& [_, shaderId] : shaderObj->mShaderHandles) {
    glDetachShader(shaderObj->mProgramId, shaderId);
  }

  return shaderObj;
}

void Shader::Bind() const { glUseProgram(mProgramId); }

void Shader::Unbind() const { glUseProgram(0); }

template <>
void Shader::SetUniform<glm::vec3>(const char* name,
                                   const glm::vec3& vec) const {
  auto location = glGetUniformLocation(mProgramId, name);
  glUniform3fv(location, vec.length(), glm::value_ptr(vec));
}

template <>
void Shader::SetUniform<glm::mat4>(const char* name,
                                   const glm::mat4& matrix) const {
  auto location = glGetUniformLocation(mProgramId, name);
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ShaderCleanup() {
  spdlog::trace(__FUNCTION__);
  for (auto&& [shaderType, shaderId] : mShaderHandles) {
    glDeleteShader(shaderId);
  }
  mShaderHandles.clear();
}

void Shader::ProgramCleanup() {
  spdlog::trace(__FUNCTION__);
  glDeleteProgram(mProgramId);
}

Shader::~Shader() {
  spdlog::trace(__FUNCTION__);
  for (auto&& [_, shaderId] : mShaderHandles) {
    glDeleteShader(shaderId);
  }

  glDeleteProgram(mProgramId);
}

}  // namespace Peridot