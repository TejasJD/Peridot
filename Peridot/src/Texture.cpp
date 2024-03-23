#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>

#include "Peridot/Texture.h"

namespace Peridot {

Texture::Texture(const char* filePath) {
  int width, height, comp;
  width = height = comp = 0;

  auto data = stbi_load(filePath, &width, &height, &comp, 0);

  if (comp == 0) {
    std::cout << "Failed to load texture" << std::endl;
    
  }
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


}

Texture::~Texture() {}

}  // namespace Peridot
