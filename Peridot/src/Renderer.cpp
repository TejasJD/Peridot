
#include <spdlog/spdlog.h>

#include "Peridot/Renderer.h"

namespace Peridot {

std::shared_ptr<Renderer> Renderer::Create(
    const std::shared_ptr<Context>& context) {
  spdlog::trace(__FUNCTION__);
  auto renderer = std::make_shared<Renderer>();

  if (!context) {
    spdlog::error("no context");
    return nullptr;
  }

  renderer->mCtx = context;
  renderer->mVertexArray = VertexArray::Create();

  return renderer;
}

}  // namespace Peridot