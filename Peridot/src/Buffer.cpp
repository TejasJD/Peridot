#include <cassert>
#include <cstdint>

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include "Peridot/Buffer.h"

namespace Peridot {

BufferElement::BufferElement(const Utils::Type elementType_, const char* name_)
    : elementType(elementType_),
      name(name_),
      sizeInBytes(Utils::SizeInBytes(elementType_)),
      offset(0) {}

BufferLayout::BufferLayout(
    const std::initializer_list<BufferElement>& layout_) {
  layout = std::vector<BufferElement>(layout_.begin(), layout_.end());
  spdlog::trace(__FUNCTION__ " Creating buffer layout");
  for (auto& item : layout) {
    item.offset = stride;
    stride += item.sizeInBytes;
    spdlog::trace("item type: {}, offset: {}",
                  Utils::TypeName(item.elementType), item.offset);
  }
  spdlog::info("layout stride: {}", stride);
}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(const float* vertices,
                                                   const size_t sizeInBytes) {
  auto buffer = std::make_shared<VertexBuffer>();
  glGenBuffers(1, &buffer->mRendererId);
  glBindBuffer(GL_ARRAY_BUFFER, buffer->mRendererId);
  glBufferData(GL_ARRAY_BUFFER, sizeInBytes, vertices, GL_STATIC_DRAW);
  spdlog::trace(__FUNCTION__ " creating handle: {}", buffer->mRendererId);
  return buffer;
}

VertexBuffer::~VertexBuffer() {
  spdlog::trace(__FUNCTION__ " destroying handle: {}", mRendererId);
  glDeleteBuffers(1, &mRendererId);
}

void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, mRendererId); }

void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

std::shared_ptr<ElementBuffer> ElementBuffer::Create(const uint32_t* indices,
                                                     const size_t sizeInBytes) {
  auto buffer = std::make_shared<ElementBuffer>();
  glGenBuffers(1, &buffer->mRendererId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->mRendererId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, indices, GL_STATIC_DRAW);
  spdlog::trace(__FUNCTION__ " creating handle: {}", buffer->mRendererId);
  return buffer;
}

ElementBuffer::~ElementBuffer() {
  spdlog::trace(__FUNCTION__ " destroying handle: {}", mRendererId);
  glDeleteBuffers(1, &mRendererId);
}

void ElementBuffer::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererId);
}

void ElementBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

}  // namespace Peridot