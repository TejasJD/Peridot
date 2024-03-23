#pragma once

#include <cstdint>
#include <initializer_list>
#include <map>
#include <vector>

#include "Peridot/RenderCalls.h"

namespace Peridot {

enum class ShaderType { VertexShader = 1, FragmentShader };

struct ShaderSpecification {
  ShaderType shaderType;
  const char* filePath;
};

class Shader {
 public:
  enum class State { Init, CompileError, LinkError, Success };
  static std::shared_ptr<Shader> Create(
      const std::vector<ShaderSpecification>& shaderSpec);
  Shader() = default;
  ~Shader();
  State ShaderState() const { return mShaderState; }
  void Bind() const;
  void Unbind() const;
  
  template<typename T>
  void SetUniform(const char* name, const T& t) const;

 private:
  void ShaderCleanup();
  void ProgramCleanup();

  std::map<ShaderType, uint32_t> mShaderHandles;
  uint32_t mProgramId = 0;
  State mShaderState = State::Init;
};

}  // namespace Peridot