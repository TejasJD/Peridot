#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Peridot/Utils.h"

namespace Peridot {

struct BufferElement {
  BufferElement(const Utils::Type elementType_, const char* name_);
  Utils::Type elementType = Utils::Type::None;
  const char* name = nullptr;
  size_t sizeInBytes = 0;
  size_t offset = 0;
};

struct BufferLayout {
  BufferLayout() = default;
  BufferLayout(const std::initializer_list<BufferElement>& layout_);
  std::vector<BufferElement> layout;
  size_t stride = 0;
};

class VertexBuffer {
 public:
  static std::shared_ptr<VertexBuffer> Create(const float* vertices,
                                              const size_t sizeInBytes);
  VertexBuffer() = default;
  ~VertexBuffer();
  void Bind() const;
  void SetBufferLayout(const BufferLayout& bufferLayout) {
    mBufferLayout = bufferLayout;
  }
  const BufferLayout& GetBufferLayout() const { return mBufferLayout; }
  void Unbind() const;

 private:
  BufferLayout mBufferLayout;
  uint32_t mRendererId = 0;
};

class ElementBuffer {
 public:
  static std::shared_ptr<ElementBuffer> Create(const uint32_t* indices,
                                               const size_t sizeInBytes);
  ElementBuffer() = default;
  ~ElementBuffer();
  void Bind() const;
  void Unbind() const;

 private:
  uint32_t mRendererId = 0;
};

}  // namespace Peridot