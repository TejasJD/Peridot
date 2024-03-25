#pragma once

namespace Peridot {
class TimeTracker {
 public:
  TimeTracker();
  void Update();
  float Delta();

 private:
  float mCurrentTime = 0.0f;
  float mPreviousTime = 0.0f;
};
}  // namespace Peridot