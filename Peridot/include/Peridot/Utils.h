#pragma once

#include <cassert>
#include <string>

namespace Peridot {

namespace Utils {

enum class Type {
  None,
  Bool,
  Int,
  Vec2i,
  Vec3i,
  Vec4i,
  Mat3x3i,
  Mat4x3i,
  Mat4x4i,
  Float,
  Vec2,
  Vec3,
  Vec4,
  Mat3x3,
  Mat4x3,
  Mat4x4
};

inline constexpr const char* TypeName(const Type elementType) {
  switch (elementType) {
    case Peridot::Utils::Type::None:
      return "Type::None";
    case Peridot::Utils::Type::Bool:
      return "Type::Bool";
    case Peridot::Utils::Type::Int:
      return "Type::Int";
    case Peridot::Utils::Type::Vec2i:
      return "Type::Vec2i";
    case Peridot::Utils::Type::Vec3i:
      return "Type::Vec3i";
    case Peridot::Utils::Type::Vec4i:
      return "Type::Vec4i";
    case Peridot::Utils::Type::Mat3x3i:
      return "Type::Mat3x3i";
    case Peridot::Utils::Type::Mat4x3i:
      return "Type::Mat4x3i";
    case Peridot::Utils::Type::Mat4x4i:
      return "Type::Mat4x4i";
    case Peridot::Utils::Type::Float:
      return "Type::Float";
    case Peridot::Utils::Type::Vec2:
      return "Type::Vec2";
    case Peridot::Utils::Type::Vec3:
      return "Type::Vec3";
    case Peridot::Utils::Type::Vec4:
      return "Type::Vec4";
    case Peridot::Utils::Type::Mat3x3:
      return "Type::Mat3x3";
    case Peridot::Utils::Type::Mat4x3:
      return "Type::Mat4x3";
    case Peridot::Utils::Type::Mat4x4:
      return "Type::Mat4x4";
    default:
      return "InvalidType";
  }
  return "InvalidType";
}

inline constexpr size_t SizeInBytes(const Type elementType) {
  switch (elementType) {
    case Type::Bool:
    case Type::Int:
    case Type::Float:
      return 4ULL;
    case Type::Vec2:
    case Type::Vec2i:
      return 8ULL;
    case Type::Vec3:
    case Type::Vec3i:
      return 12ULL;
    case Type::Vec4:
    case Type::Vec4i:
      return 16ULL;
    case Type::Mat3x3:
    case Type::Mat3x3i:
      return 36ULL;
    case Type::Mat4x3:
    case Type::Mat4x3i:
      return 48ULL;
    case Type::Mat4x4:
    case Type::Mat4x4i:
      return 64ULL;
    case Type::None:
    default:
      assert(false && "None or Invalid type not supported");
      return 0ULL;
  }
  return 0ULL;
}

inline constexpr size_t TypeSize(const Type elementType) {
  switch (elementType) {
    case Type::Bool:
    case Type::Int:
    case Type::Float:
      return 1ULL;
    case Type::Vec2:
    case Type::Vec2i:
      return 2ULL;
    case Type::Vec3:
    case Type::Vec3i:
      return 3ULL;
    case Type::Vec4:
    case Type::Vec4i:
      return 4ULL;
    case Type::Mat3x3:
    case Type::Mat3x3i:
      return 9ULL;
    case Type::Mat4x3:
    case Type::Mat4x3i:
      return 12ULL;
    case Type::Mat4x4:
    case Type::Mat4x4i:
      return 16ULL;
    case Type::None:
    default:
      assert(false && "None or Invalid type not supported");
      return 0ULL;
  }
  return 0ULL;
}

std::string ReadFileIntoStringBuffer(const char* filePath);

}  // namespace Utils

}  // namespace Peridot
