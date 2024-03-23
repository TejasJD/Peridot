#pragma once

#include <cstdint>
#include <memory>
#include <unordered_set>

#include "Peridot/Buffer.h"

namespace Peridot {

class VertexArray {
 public:
  static std::shared_ptr<VertexArray> Create();
  VertexArray() = default;
  void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
  void RemoveVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
  void SetElementBuffer(const std::shared_ptr<ElementBuffer>& elementBuffer);
  void UnsetElementBuffer();
  void Bind() const;
  void Unbind() const;
  ~VertexArray();

 private:
  // probably hash function should assert be equal for different
  // shader_ptr's pointing to the same VertexBuffer
  std::unordered_set<std::shared_ptr<VertexBuffer>> mVertexBuffers;
  std::shared_ptr<ElementBuffer> mElementBuffer;
  uint32_t mRendererId = 0;
};

}  // namespace Renderer