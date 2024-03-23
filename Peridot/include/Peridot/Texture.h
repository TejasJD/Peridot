#pragma once

#include <cstdint>

namespace Peridot {

struct Texture {
 public:
  Texture(const char* filePath);
  ~Texture();
 private:
  uint32_t mHeight = 0;
  uint32_t mWidth = 0;
  uint32_t mTextureId = 0;
};

}  // namespace Peridot