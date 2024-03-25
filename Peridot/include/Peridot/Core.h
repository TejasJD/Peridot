#pragma once

#include <exception>
#include <memory>

#include "Peridot/Buffer.h"
#include "Peridot/Context.h"
#include "Peridot/Shader.h"
#include "Peridot/TimeTracker.h"
#include "Peridot/VertexArray.h"
#include "Peridot/RenderCalls.h"

namespace Peridot {

template <typename App>
class AppRunner {
 public:
  static std::shared_ptr<AppRunner<App>> Create(const ContextSpecification& ctxSpec);
  inline AppRunner() = default;
  inline void RunApp();

 private:
  std::shared_ptr<Context> mCtx;
  std::shared_ptr<App> mApp;
};

template<typename App>
std::shared_ptr<AppRunner<App>> AppRunner<App>::Create(const ContextSpecification& ctxSpec) {
  auto runner = std::make_shared<AppRunner<App>>();
  runner->mCtx = Context::Create(ctxSpec);

  if (!runner->mCtx) {
    return nullptr;
  }
  runner->mApp = App::Create(runner->mCtx);

  if (!runner->mApp) {
    return nullptr;
  }

  return runner;
}

template<typename App>
void AppRunner<App>::RunApp() {
  TimeTracker tracker;

  while (mCtx->ShouldRun() && mApp->ShouldRun()) {
    tracker.Update();
    mApp->Update(tracker.Delta());
    mCtx->Update(tracker.Delta());
  }
}

}  // namespace Peridot
