#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

#include <Peridot/OrthographicCamController.h>
#include <Peridot/PerspectiveCamController.h>
#include <Peridot/Core.h>
#include <Peridot/Input.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

struct App {
  App() = default;
  ~App() { spdlog::trace(__FUNCTION__); }

  // Ideally, App::Create should only be called with a valid context by the
  // runner.
  static std::shared_ptr<App> Create(std::shared_ptr<Peridot::Context> ctx) {
    spdlog::trace(__FUNCTION__);

    auto app = std::make_shared<App>();

    // give app to access to context
    app->ctx = ctx;

    app->input = std::make_shared<Peridot::PollModeInput>(app->ctx);

    auto shader = Peridot::Shader::Create(
        {{Peridot::ShaderType::VertexShader, "shader.vert"},
         {Peridot::ShaderType::FragmentShader, "shader.frag"}});

    if (!shader) {
      return nullptr;
    }

    app->shader = shader;

    auto vertexBuffer = Peridot::VertexBuffer::Create(
        app->vertices.data(), app->vertices.size() * sizeof(float));

    Peridot::BufferLayout bufferLayout = {{Peridot::Utils::Type::Vec3, "aPos"}};

    vertexBuffer->SetBufferLayout(bufferLayout);

    auto elementBuffer = Peridot::ElementBuffer::Create(
        app->indices.data(), app->indices.size() * sizeof(uint32_t));

    app->vertexArray = Peridot::VertexArray::Create();

    app->vertexArray->AddVertexBuffer(vertexBuffer);
    app->vertexArray->SetElementBuffer(elementBuffer);
    Peridot::RenderCall::SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    app->controller = std::make_shared<Peridot::PerspectiveCameraController>(
        1.0f, 1.5f, 3.0f);

    app->controller->GetCamera().SetAspectRatio(ctx->GetAspectRatio());
    app->controller->GetCamera().SetPosition({0.0f, 0.0f, 3.0f});

    for (auto key :
         {Peridot::KeyCode::Up, Peridot::KeyCode::Down, Peridot::KeyCode::Left,
          Peridot::KeyCode::Right, Peridot::KeyCode::Q, Peridot::KeyCode::E,
          Peridot::KeyCode::W, Peridot::KeyCode::S}) {
      app->input->RegisterKeyCallback(
          key, [key, app](Peridot::ButtonState state) {
            if (!app->controller) {
              return;
            }

            if (state == Peridot::ButtonState::Released) {
              return;
            }

            switch (key) {
              case Peridot::KeyCode::Up:
                app->controller->MoveForward();
                return;
              case Peridot::KeyCode::Down:
                app->controller->MoveBack();
                return;
              case Peridot::KeyCode::Left:
                app->controller->MoveLeft();
                return;
              case Peridot::KeyCode::Right:
                app->controller->MoveRight();
                return;
              case Peridot::KeyCode::W:
                app->controller->MoveUp();
                return;
              case Peridot::KeyCode::S:
                app->controller->MoveDown();
                return;
              case Peridot::KeyCode::Q:
                app->controller->RotateLeft();
                return;
              case Peridot::KeyCode::E:
                app->controller->RotateRight();
                return;
              default:
                break;
            }
          });
      app->input->RegisterCursorCallback([app](double prevX, double prevY,
                                               double newX, double newY) {
        if (app->input->GetMouseButtonState(Peridot::MouseCode::ButtonRight) ==
            Peridot::ButtonState::Released) {
          return;
        }
        app->controller->MoveWithCursor(prevX, prevY, newX, newY);
      });
    }
    return app;
  }

  void Update(float delta) {
    Peridot::RenderCall::ClearColorAndDepth();

    // spdlog::info("Aspect ratio: {}", ctx->GetAspectRatio());

    controller->SetDelta(delta);
    controller->GetCamera().SetAspectRatio(ctx->GetAspectRatio());

    shader->Bind();
    shader->SetUniform<glm::mat4>(
        "uMVP", controller->GetCamera().GetProjectionMatrix() *
                    controller->GetCamera().GetViewMatrix());

    vertexArray->Bind();

    Peridot::RenderCall::DrawElements(indices.size());
    input->PollAndInvokeCallbacks();
  }

  bool ShouldRun() const { return true; }

  float aspectRatio = 1.0f;
  std::shared_ptr<Peridot::PerspectiveCameraController> controller;
  std::shared_ptr<Peridot::Context> ctx;
  std::vector<float> vertices = {
      0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f, -0.5f, -0.5f,
      0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
  };

  std::vector<uint32_t> indices = {0, 1, 2, 0, 3, 2,

                                   4, 5, 6, 4, 7, 6,

                                   0, 4, 5, 0, 1, 5,

                                   3, 7, 6, 3, 2, 6,

                                   0, 4, 7, 0, 3, 7,

                                   1, 5, 6, 1, 2, 6};

  std::shared_ptr<Peridot::VertexArray> vertexArray;
  std::shared_ptr<Peridot::Shader> shader;
  std::shared_ptr<Peridot::PollModeInput> input;
};

int main() {
#ifndef NDEBUG
  spdlog::set_level(spdlog::level::trace);
#endif
  auto spec = Peridot::ContextSpecification{};
  spec.width = 1366;
  spec.height = 768;

  auto runner = Peridot::AppRunner<App>::Create(spec);
  runner->RunApp();
  return 0;
}