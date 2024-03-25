#include <GLFW/glfw3.h>

#include "Peridot/TimeTracker.h"

namespace Peridot {

TimeTracker::TimeTracker()
    : mCurrentTime(static_cast<float>(glfwGetTime())),
      mPreviousTime(static_cast<float>(glfwGetTime())) {}

void TimeTracker::Update() {
  mCurrentTime = static_cast<float>(glfwGetTime());
}

float TimeTracker::Delta() {
  auto delta = mCurrentTime - mPreviousTime;
  mPreviousTime = mCurrentTime;
  return delta;
}

}  // namespace Peridot