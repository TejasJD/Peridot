#include <cassert>
#include <cstdio>
#include <exception>
#include <iostream>

#include <spdlog/spdlog.h>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "Peridot/Context.h"

// make use of dedicated GPU on windows
#ifdef _WIN32

extern "C" {
_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

#endif

namespace Peridot {

namespace Utils {

static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                       GLenum severity, GLsizei length,
                                       const GLchar* message,
                                       const void* userParam) {
  fprintf(stderr,
          "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
          (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity,
          message);
}

}  // namespace Utils

void Context::WindowSizeCallback(GLFWwindow* window, int32_t width,
                                 int32_t height) {
  Peridot::Context* ctx = (Peridot::Context*)glfwGetWindowUserPointer(window);
  ctx->currentWidth = width;
  ctx->currentHeight = height;
  ctx->mAspectRatio = static_cast<float>(width) / height;
  glViewport(0, 0, width, height);
}

void Context::SetCallbacks(GLFWwindow* window) {
  glfwSetWindowSizeCallback(window, WindowSizeCallback);
}

std::shared_ptr<Context> Context::Create(const ContextSpecification& ctxSpec) {
  spdlog::trace(__FUNCTION__);
  std::shared_ptr<Context> ctx = std::make_shared<Context>();

  int32_t success = 0;

  success = glfwInit();

  if (!success) {
    spdlog::error("Failed to initialize glfw");
    return nullptr;
  }

  ctx->mWindow = glfwCreateWindow(ctxSpec.width, ctxSpec.height, ctxSpec.title,
                                  nullptr, nullptr);

  glfwMakeContextCurrent(ctx->mWindow);

  if (!ctx->mWindow) {
    spdlog::error("Failed to create window");
    return nullptr;
  }

  success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  if (!success) {
    spdlog::error("Glad failed to load functions");
    return nullptr;
  }

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(Utils::MessageCallback, nullptr);
  glEnable(GL_DEPTH_TEST);

  auto cardString = (const char*)glGetString(GL_RENDERER);
  auto glVersion = (const char*)glGetString(GL_VERSION);
  auto vendor = (const char*)glGetString(GL_VENDOR);
  spdlog::info("graphics card: {}", cardString);
  spdlog::info("open gl version: {}", glVersion);
  spdlog::info("gl vendor: {}", vendor);

  ctx->currentWidth = ctxSpec.width;
  ctx->currentHeight = ctxSpec.height;
  ctx->mAspectRatio = static_cast<float>(ctxSpec.width) / ctxSpec.height;

  glfwSetWindowUserPointer(ctx->mWindow, (void*)ctx.get());
  SetCallbacks(ctx->mWindow);
  return ctx;
}

Context::~Context() {
  spdlog::trace(__FUNCTION__);
  glfwDestroyWindow(mWindow);
  glfwTerminate();
}

bool Context::ShouldRun() const { return !glfwWindowShouldClose(mWindow); }

void Context::Update(float delta) {
  glfwPollEvents();
  glfwSwapBuffers(mWindow);
}

}  // namespace Peridot