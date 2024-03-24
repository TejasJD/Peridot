#pragma once

#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <tuple>

#include "Peridot/MouseCodes.h"
#include "Peridot/KeyCodes.h"
#include "Peridot/Context.h"

namespace Peridot {

enum class ButtonState { Released = 0, Pressed = 1 };

class PollModeInput {
 public:
  PollModeInput(const std::shared_ptr<const Context>& context)
      : mContext(context) {}

  using ButtonStateCallback = std::function<void(ButtonState)>;
  using CursorCallback = std::function<void(double, double, double, double)>;

  void RegisterKeyCallback(const KeyCode key,
                           const ButtonStateCallback& callback);
  void RegisterMouseCallback(const MouseCode button,
                             const ButtonStateCallback& callback);
  void RegisterCursorCallback(const CursorCallback& callback);
  void UnregisterKeyCallback(const KeyCode key);
  void UnregisterMouseCallback(const MouseCode button);
  void UnregisterCursorCallback();

  void PollAndInvokeCallbacks() const {
    auto [prevX, prevY, newX, newY] = GetCursorPos();

    if (mCursorCallback) {
      (*mCursorCallback)(prevX, prevY, newX, newY);
    } 

    for (auto&& [key, callback] : mKeyMappings) {
      auto buttonState = GetKeyState(key);
      callback(buttonState);
    }

    for (auto&& [mouseButton, callback] : mMouseButtonMappings) {
      auto buttonState = GetMouseButtonState(mouseButton);
      callback(buttonState);
    }
  }

  ButtonState GetKeyState(const KeyCode key) const;
  ButtonState GetMouseButtonState(const MouseCode mouseButton) const;
  std::tuple<double, double, double, double> GetCursorPos() const;

 private:

  std::unordered_map<KeyCode, ButtonStateCallback> mKeyMappings;
  std::unordered_map<MouseCode, ButtonStateCallback> mMouseButtonMappings;
  std::shared_ptr<CursorCallback> mCursorCallback;
  std::shared_ptr<const Context> mContext;
};

}  // namespace Peridot