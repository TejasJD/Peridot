#include <fstream>

#include "Peridot/Utils.h"

namespace Peridot {

namespace Utils {

std::string ReadFileIntoStringBuffer(const char* filePath) {
  std::ifstream file(filePath, std::ios::binary | std::ios::ate);
  if (!file) {
    return std::string();
  }
  auto end = file.tellg();
  file.seekg(0);
  std::string buffer;
  buffer.resize(end);
  file.read(buffer.data(), end);
  return buffer;
}

}  // namespace Utils

}  // namespace Peridot