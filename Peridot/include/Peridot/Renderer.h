#pragma once

#include "Peridot/Core.h"

namespace Peridot {

class Renderer {
 public:
  static std::shared_ptr<Renderer> Create(const std::shared_ptr<Context>& mCtx);

  Renderer() = default;
  ~Renderer();

 private:
  std::shared_ptr<Context> mCtx;
  std::shared_ptr<VertexArray> mVertexArray;
  std::shared_ptr<Shader> mShader;
};

}  // namespace Peridot
