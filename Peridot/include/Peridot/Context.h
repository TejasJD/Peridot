#pragma once

#include <memory>

struct GLFWwindow;

namespace Peridot {

struct ContextSpecification {
  int32_t width = 1;
  int32_t height = 1;
  const char* title = "LearnOpenGL";
};

class Context {
 public:
  static std::shared_ptr<Context> Create(const ContextSpecification& ctxSpec);
  Context() = default;
  ~Context();

  int32_t GetWidth() const { return currentWidth; }
  int32_t GetHeight() const { return currentHeight; }
  float GetAspectRatio() const { return mAspectRatio; }

  bool ShouldRun() const;
  void Update(float delta);
  GLFWwindow* GetRawWindow() const { return mWindow; };

 private:
  static void WindowSizeCallback(GLFWwindow* window, int32_t width,
                                 int32_t height);
  static void SetCallbacks(GLFWwindow* window);

  GLFWwindow* mWindow = nullptr;
  int32_t currentWidth = 0;
  int32_t currentHeight = 0;
  float mAspectRatio = 1.0f;
};

}  // namespace Peridot