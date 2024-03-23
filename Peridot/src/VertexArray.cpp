#include <cassert>

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include "Peridot/Utils.h"
#include "Peridot/VertexArray.h"

namespace Peridot {

namespace Utils {
inline static GLenum GLDataType(const Type elementType) {
  switch (elementType) {
    case Type::Bool:
      return GL_BOOL;
    case Type::Int:
    case Type::Vec2i:
    case Type::Vec3i:
    case Type::Vec4i:
    case Type::Mat3x3i:
    case Type::Mat4x3i:
    case Type::Mat4x4i:
      return GL_INT;
    case Type::Float:
    case Type::Vec2:
    case Type::Vec3:
    case Type::Vec4:
    case Type::Mat3x3:
    case Type::Mat4x3:
    case Type::Mat4x4:
      return GL_FLOAT;
    case Type::None:
    default:
      assert(false && "None or Invalid type not supported");
      return GL_NONE;
  }

  return GL_NONE;
}
}  // namespace Utils

std::shared_ptr<VertexArray> VertexArray::Create() {
  auto vertexArray = std::make_shared<VertexArray>();
  glGenVertexArrays(1, &vertexArray->mRendererId);
  glBindVertexArray(vertexArray->mRendererId);
  spdlog::trace(__FUNCTION__ " creating handle: {}", vertexArray->mRendererId);
  return vertexArray;
}

VertexArray::~VertexArray() {
  spdlog::trace(__FUNCTION__ " destroying handle: {}", mRendererId);
  glDeleteVertexArrays(1, &mRendererId);
}

void VertexArray::Bind() const { glBindVertexArray(mRendererId); }

void VertexArray::Unbind() const { glBindVertexArray(0); }

void VertexArray::AddVertexBuffer(
    const std::shared_ptr<VertexBuffer>& vertexBuffer) {
  Bind();
  vertexBuffer->Bind();
  uint32_t index = 0;
  const auto& bufferLayout = vertexBuffer->GetBufferLayout();
  for (const auto& item : bufferLayout.layout) {
    glVertexAttribPointer(index, (int32_t)Utils::TypeSize(item.elementType),
                          Utils::GLDataType(item.elementType), GL_FALSE,
                          bufferLayout.stride, (const void*)item.offset);
    glEnableVertexAttribArray(index);
    index += 1;
  }
  mVertexBuffers.insert(vertexBuffer);
}

void VertexArray::RemoveVertexBuffer(
    const std::shared_ptr<VertexBuffer>& vertexBuffer) {
  Bind();
  auto it = mVertexBuffers.find(vertexBuffer);
  if (it == mVertexBuffers.end()) {
    return;
  }
  (*it)->Unbind();
  mVertexBuffers.erase(it);
}

void VertexArray::SetElementBuffer(
    const std::shared_ptr<ElementBuffer>& elementBuffer) {
  Bind();
  elementBuffer->Bind();
  mElementBuffer = elementBuffer;
}

void VertexArray::UnsetElementBuffer() {
  Bind();
  mElementBuffer->Unbind();
  mElementBuffer = nullptr;
}

}  // namespace Peridot