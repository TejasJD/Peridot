#include <GLFW/glfw3.h>

#include "Peridot/TimeTracker.h"

namespace Peridot {

TimeTracker::TimeTracker() : mTimePoint(static_cast<float>(glfwGetTime())) {}

float TimeTracker::Delta() {
  float newTimePoint = static_cast<float>(glfwGetTime());
  float delta = newTimePoint - mTimePoint;
  mTimePoint = newTimePoint;
  return delta;
}

}  // namespace Peridot