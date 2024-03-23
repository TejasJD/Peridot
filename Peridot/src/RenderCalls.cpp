#include <cstdint>

#include <glad/glad.h>

#include "Peridot/RenderCalls.h"

namespace Peridot {

void RenderCall::SetClearColor(const float r, const float g, const float b,
                               const float a) {
  glClearColor(r, g, b, a);
}

void RenderCall::ClearColor() { glClear(GL_COLOR_BUFFER_BIT); }

void RenderCall::DrawElements(const size_t count) {
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

}  // namespace Peridot