#pragma once

namespace Peridot {

struct RenderCall {
  static void SetClearColor(const float r, const float g, const float b,
                            const float a);
  static void ClearColorAndDepth();

  static void DrawElements(const size_t count);
};

}  // namespace Peridot